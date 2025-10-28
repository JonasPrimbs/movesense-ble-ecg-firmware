#include "MeasurementProvider.h"

#include "app-resources/resources.h"
#include "meas_ecg/resources.h"
#include "meas_imu/resources.h"
#include "movesense_time/resources.h"
#include "ui_ind/resources.h"

const char* const MeasurementProvider::LAUNCHABLE_NAME = "MeasurementProvider";

// Set execution context to *MEAS*
constexpr wb::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_DATASTREAM::EXECUTION_CONTEXT;

// Array of all resources this Module provides.
static constexpr wb::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_DATASTREAM::LID,
    WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_INTERVAL::LID,
    WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_DATASTREAM::LID,
    WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_INTERVAL::LID,
};
MeasurementProvider::MeasurementProvider() :
    ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
    LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId),
    // ECG
    mEcgMeasurementInterval(ECG_DEFAULT_MEASUREMENT_INTERVAL),
    mEcgSampleCounter(0),
    mEcgSampleSkipCount(ECG_DEFAULT_SAMPLE_SKIP_COUNT),
    // IMU
    mImuMeasurementInterval(IMU_BASE_MEASUREMENT_INTERVAL),
    mImuSampleCounter(0),
    mImuSampleSkipCount(IMU_DEFAULT_SAMPLE_SKIP_COUNT),
    // Time
    mUnixBaseTimestampUs(0)
{
    // Create buffers.
    this->ecgBuffer = new AbsoluteSeriesBuffer<ecg_t>(DEFAULT_ECG_OBJECT_SIZE,
                                                      numberOfEcgBuffers);
    this->movBuffer = new AbsoluteSeriesBuffer<mov_t>(DEFAULT_MOV_OBJECT_SIZE,
                                                      numberOfMovBuffers);
}

MeasurementProvider::~MeasurementProvider()
{
    // Free buffer memory.
    delete ecgBuffer;
    delete movBuffer;
}

bool MeasurementProvider::initModule()
{
    if (registerProviderResources(sProviderResources) != wb::HTTP_CODE_OK)
        return false;
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void MeasurementProvider::deinitModule()
{
    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool MeasurementProvider::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Subscribe to changes in the devices timestamp
    asyncSubscribe(WB_RES::LOCAL::TIME_DETAILED());

    // Subscribe to incoming measurement data from the beginning.
    asyncSubscribe(WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(),
                   AsyncRequestOptions::Empty, ECG_BASE_SAMPLE_RATE);
    asyncSubscribe(WB_RES::LOCAL::MEAS_IMU9_SAMPLERATE(),
                   AsyncRequestOptions::Empty, IMU9_BASE_SAMPLE_RATE);
    return true;
}

void MeasurementProvider::stopModule()
{
    // Unsubscribe incoming measurement data.
    asyncUnsubscribe(WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(),
                     AsyncRequestOptions::Empty, ECG_BASE_SAMPLE_RATE);
    asyncUnsubscribe(WB_RES::LOCAL::MEAS_IMU9_SAMPLERATE(),
                     AsyncRequestOptions::Empty, IMU9_BASE_SAMPLE_RATE);

    asyncUnsubscribe(WB_RES::LOCAL::TIME_DETAILED());
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void MeasurementProvider::onPutRequest(const wb::Request& request,
                                       const wb::ParameterList& parameters)
{
    if (mModuleState != WB_RES::ModuleStateValues::STARTED)
        return returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);

    switch (request.getResourceId().localResourceId)
    {
    // PUT request for new ECG interval.
    case WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_INTERVAL::LID: {
        if (parameters.getNumberOfParameters() <= 0)
            return returnResult(request, whiteboard::HTTP_CODE_BAD_REQUEST);

        // Get new interval value.
        uint8_t newEcgMeasInterval = parameters[0].convertTo<uint8_t>();

        // Set new interval.
        setEcgMeasurementInterval(newEcgMeasInterval);
        return returnResult(request, whiteboard::HTTP_CODE_OK);
    }
    // PUT request for new IMU9 interval.
    case WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_INTERVAL::LID: {
        if (parameters.getNumberOfParameters() <= 0)
            return returnResult(request, whiteboard::HTTP_CODE_BAD_REQUEST);

        // Get new interval value.
        uint8_t newImuMeasInterval = parameters[0].convertTo<uint8_t>();

        // Set new interval.
        setImuMeasurementInterval(newImuMeasInterval);
        return returnResult(request, whiteboard::HTTP_CODE_OK);
    }
    default:
        return returnResult(request, whiteboard::HTTP_CODE_NOT_FOUND);
    }
}

void MeasurementProvider::onNotify(whiteboard::ResourceId resourceId,
                                   const whiteboard::Value& rValue,
                                   const whiteboard::ParameterList& rParameters)
{
    switch (resourceId.localResourceId)
    {
    // Change in the devices timestamp
    case WB_RES::LOCAL::TIME_DETAILED::LID: {

        const WB_RES::TimeSync& ts =
            rValue.convertTo<const WB_RES::TimeSync&>();

        // Update base time to contain the time of the device-start in us.
        this->mUnixBaseTimestampUs = ts.utcTime - 1000 * ts.relativeTime;
        break;
    }
    // New ECG data received from the sensor native resource.
    case WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE::LID: {
        const WB_RES::ECGData& data =
            rValue.convertTo<const WB_RES::ECGData&>();
        // Down-sample the data according to the current sample rate setting.
        for (size_t i = 0; i < data.samples.getNumberOfItems(); ++i)
        {
            mEcgSampleCounter = (mEcgSampleCounter + 1) % mEcgSampleSkipCount;

            // Insert only every mEcgSampleSkipCount-th sample into buffer.
            if (mEcgSampleCounter != 0)
                continue;

            // Convert 32-bit sample to 16-bit.
            ecg_t sample = prepareEcgSample(data.samples[i]);
            // Add to the buffer.
            ecgBuffer->addSample(sample);

            // When buffer is full: send off ECG-packet.
            if (!ecgBuffer->canAddSample())
            {
                if (data.timestamp < mEcgCurrentSensorTimestampMs)
                {
                    mEcgSensorTimestampOverflowCounter++;
                }
                mEcgCurrentSensorTimestampMs = data.timestamp;
                // TODO: correct timestamp to last sample?
                ecgBuffer->setTimestamp(
                    this->mUnixBaseTimestampUs +
                    1000 *
                        (data.timestamp + mEcgSensorTimestampOverflowCounter *
                                              MAX_TIMESTAMP_VALUE));

                // Create Measurement Data packet.
                WB_RES::MeasurementBundle32 packet{wb::MakeArray(
                    reinterpret_cast<uint32_t*>(ecgBuffer->getCurrentBuffer()),
                    ECG_PACKET_SIZE_UINT32)};
                // Send the packet off to local listeners.
                updateResource(
                    WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_DATASTREAM(),
                    ResponseOptions::Empty, packet);
                ecgBuffer->switchBuffer();
            }
        }

        break;
    }
    // New IMU9 data received from the sensor native resource.
    case WB_RES::LOCAL::MEAS_IMU9_SAMPLERATE::LID: {

        const WB_RES::IMU9Data& data =
            rValue.convertTo<const WB_RES::IMU9Data&>();

        // Down-sample the data according to the current sample rate setting.
        for (size_t i = 0; i < data.arrayAcc.getNumberOfItems(); ++i)
        {
            mImuSampleCounter = (mImuSampleCounter + 1) % mImuSampleSkipCount;

            // Insert only every mImuSampleSkipCount-th sample into buffer.
            if (mImuSampleCounter != 0)
                continue;

            // Convert float values to 16-bit int by scaling.
            mov_t sample = prepareImuSample(data.arrayAcc[i], data.arrayGyro[i],
                                            data.arrayMagn[i]);
            movBuffer->addSample(sample);

            // When buffer is full: send off IMU9-packet
            if (!movBuffer->canAddSample())
            {
                // Check if the data-timestamp overflowed
                if (data.timestamp < mImuCurrentSensorTimestampMs)
                {
                    mImuSensorTimestampOverflowCounter++;
                }
                mImuCurrentSensorTimestampMs = data.timestamp;
                // TODO: correct timestamp to last sample?
                movBuffer->setTimestamp(
                    this->mUnixBaseTimestampUs +
                    1000 *
                        (data.timestamp + mImuSensorTimestampOverflowCounter *
                                              MAX_TIMESTAMP_VALUE));

                // Create measurement data packet.
                WB_RES::MeasurementBundle32 packet{wb::MakeArray(
                    reinterpret_cast<uint32_t*>(movBuffer->getCurrentBuffer()),
                    MOV_PACKET_SIZE_UINT32)};

                // Send the packet off to local listeners.
                updateResource(
                    WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_DATASTREAM(),
                    ResponseOptions::Empty, packet);
                movBuffer->switchBuffer();
            }
        }

        break;
    }
    }
}

void MeasurementProvider::setEcgMeasurementInterval(uint8_t newEcgInterval)
{
    // Update interval and skip-counters for down-sampling.
    this->mEcgMeasurementInterval = newEcgInterval;
    this->mEcgSampleCounter = 0;
    this->mEcgSampleSkipCount =
        (uint8_t)mEcgMeasurementInterval / ECG_BASE_MEASUREMENT_INTERVAL;
}

void MeasurementProvider::setImuMeasurementInterval(uint8_t newImuInterval)
{
    // Update interval and skip-counters for down-sampling.
    this->mImuMeasurementInterval = newImuInterval;
    this->mImuSampleCounter = 0;
    this->mImuSampleSkipCount =
        (uint8_t)mImuMeasurementInterval / IMU_BASE_MEASUREMENT_INTERVAL;
}

ecg_t MeasurementProvider::prepareEcgSample(int32 rawSample)
{
    if (rawSample > ECG_MAX_VALUE)
    {
        return ECG_INVALID_VALUE;
    }
    else if (rawSample < ECG_MIN_VALUE)
    {
        return ECG_INVALID_VALUE;
    }
    else
    {
        return (ecg_t)rawSample;
    }
}

mov_t MeasurementProvider::prepareImuSample(wb::FloatVector3D accSample,
                                            wb::FloatVector3D gyrSample,
                                            wb::FloatVector3D magSample)
{
    return mov_t{prepareAccSample(accSample), prepareGyrSample(gyrSample),
                 prepareMagSample(magSample)};
}

acc_vec_t MeasurementProvider::prepareAccSample(wb::FloatVector3D accVector)
{
    float accX = accVector.x * accScale;
    float accY = accVector.y * accScale;
    float accZ = accVector.z * accScale;

    if (accX > MAX_ACC || accX < MIN_ACC)
    {
        accX = ERR_ACC;
    }
    if (accY > MAX_ACC || accY < MIN_ACC)
    {
        accY = ERR_ACC;
    }
    if (accZ > MAX_ACC || accZ < MIN_ACC)
    {
        accZ = ERR_ACC;
    }

    acc_vec_t value;
    value.x = static_cast<acc_t>(accX);
    value.y = static_cast<acc_t>(accY);
    value.z = static_cast<acc_t>(accZ);
    return value;
}

gyr_vec_t MeasurementProvider::prepareGyrSample(wb::FloatVector3D gyrVector)
{
    float gyrX = gyrVector.x * gyrScale;
    float gyrY = gyrVector.y * gyrScale;
    float gyrZ = gyrVector.z * gyrScale;

    if (gyrX > MAX_GYR || gyrX < MIN_GYR)
    {
        gyrX = ERR_GYR;
    }
    if (gyrY > MAX_GYR || gyrY < MIN_GYR)
    {
        gyrY = ERR_GYR;
    }
    if (gyrZ > MAX_GYR || gyrZ < MIN_GYR)
    {
        gyrZ = ERR_GYR;
    }

    gyr_vec_t value;
    value.x = static_cast<gyr_t>(gyrX);
    value.y = static_cast<gyr_t>(gyrY);
    value.z = static_cast<gyr_t>(gyrZ);
    return value;
}

mag_vec_t MeasurementProvider::prepareMagSample(wb::FloatVector3D magVector)
{
    float magX = magVector.x * magScale;
    float magY = magVector.y * magScale;
    float magZ = magVector.z * magScale;

    if (magX > MAX_MAG || magX < MIN_MAG)
    {
        magX = ERR_MAG;
    }
    if (magY > MAX_MAG || magY < MIN_MAG)
    {
        magY = ERR_MAG;
    }
    if (magZ > MAX_MAG || magZ < MIN_MAG)
    {
        magZ = ERR_MAG;
    }

    mag_vec_t value;
    value.x = static_cast<mag_t>(magX);
    value.y = static_cast<mag_t>(magY);
    value.z = static_cast<mag_t>(magZ);
    return value;
}
