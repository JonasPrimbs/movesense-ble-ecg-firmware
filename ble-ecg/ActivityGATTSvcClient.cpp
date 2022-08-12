#include "movesense.h"

#include <meas_ecg/resources.h>
#include <meas_acc/resources.h>
#include <meas_gyro/resources.h>
#include <meas_magn/resources.h>

#include "ActivityGATTSvcClient.h"
#include "common/core/debug.h"
#include "oswrapper/thread.h"
#include "component_led/resources.h"
#include "ui_ind/resources.h"

#include "comm_ble_gattsvc/resources.h"
#include "comm_ble/resources.h"


// Activity GATT Service implementations:

const char* const ActivityGATTSvcClient::LAUNCHABLE_NAME = "ActivityGattSvc";

ActivityGATTSvcClient::ActivityGATTSvcClient() :
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mActivitySvcHandle(0),
    mEcgVoltageCharHandle(0),
    mEcgVoltageCharResource(wb::ID_INVALID_RESOURCE),
    mEcgMeasurementIntervalCharHandle(0),
    mEcgMeasurementIntervalCharResource(wb::ID_INVALID_RESOURCE),
    mMovCharHandle(0),
    mMovCharResource(wb::ID_INVALID_RESOURCE),
    mMovMeasurementIntervalCharHandle(0),
    mMovMeasurementIntervalCharResource(wb::ID_INVALID_RESOURCE),
    movAccCount(0),
    movGyrCount(0),
    movMagCount(0),
    ecgMeasurementInterval(DEFAULT_ECG_MEASUREMENT_INTERVAL),
    movMeasurementInterval(DEFAULT_MOV_MEASUREMENT_INTERVAL)
{
    this->ecgBuffer = new SeriesBuffer<ecg_t>(DEFAULT_ECG_OBJECT_SIZE, numberOfEcgBuffers);
    this->movBuffer = new SeriesBuffer<mov_t>(movBufferSize, numberOfMovBuffers);
    this->currentMovementSamples = new mov_t[DEFAULT_MOV_OBJECT_SIZE];
}

ActivityGATTSvcClient::~ActivityGATTSvcClient()
{
    delete this->ecgBuffer;
    delete this->movBuffer;
}

bool ActivityGATTSvcClient::initModule()
{
    DEBUGLOG("ActivityGATTSvcClient::initModule");

    this->mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void ActivityGATTSvcClient::deinitModule()
{
    DEBUGLOG("ActivityGATTSvcClient::deinitModule");

    this->mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool ActivityGATTSvcClient::startModule()
{
    DEBUGLOG("ActivityGATTSvcClient::startModule");

    this->mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Set ECG measurement interval to compute ECG sampling frequency.
    this->setEcgMeasurementInterval(DEFAULT_ECG_MEASUREMENT_INTERVAL);
    // Set Movement measurement interval to compute ECG sampling frequency.
    this->setMovMeasurementInterval(DEFAULT_MOV_MEASUREMENT_INTERVAL);
    // Subscribe to ECG samples with computed ECG sampling frequency.
    this->subscribeToEcgSamples();
    // Subscribe to Movement samples with computed Movement sampling frequency.
    this->subscribeToAccSamples();
    this->subscribeToGyrSamples();
    this->subscribeToMagSamples();

    // Configure Activity GATT Service.
    this->configGattSvc();

    return true;
}

void ActivityGATTSvcClient::stopModule()
{
    DEBUGLOG("ActivityGATTSvcClient::stopModule");

    // Unsubscribe from ECG samples.
    this->unsubscribeFromEcgSamples();
    this->mEcgVoltageCharResource = wb::ID_INVALID_RESOURCE;
    // Unsubscribe from Movement samples.
    this->unsubscribeFromAccSamples();
    this->unsubscribeFromGyrSamples();
    this->unsubscribeFromMagSamples();
    this->mMovCharResource = wb::ID_INVALID_RESOURCE;

    // Unsubscribe and clear ECG Measurement Interval GATT characteristics.
    this->asyncUnsubscribe(this->mEcgMeasurementIntervalCharResource);
    this->mEcgMeasurementIntervalCharResource = wb::ID_INVALID_RESOURCE;
    // Unsubscribe and clear Movement Measurement Interval GATT characteristics.
    this->asyncUnsubscribe(this->mMovMeasurementIntervalCharResource);
    this->mMovMeasurementIntervalCharResource = wb::ID_INVALID_RESOURCE;

    this->mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void ActivityGATTSvcClient::onGetResult(wb::RequestId requestId,
                                   wb::ResourceId resourceId,
                                   wb::Result resultCode,
                                   const wb::Value& rResultData)
{
    DEBUGLOG("ActivityGATTSvcClient::onGetResult");

    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE::LID:
        {
            DEBUGLOG("ActivityGATTSvcClient::onGetResult - COMM_BLE_GATTSVC_SVCHANDLE");

            const WB_RES::GattSvc &svc = rResultData.convertTo<const WB_RES::GattSvc&>();
            for (size_t i = 0; i < svc.chars.size(); i++)
            {
                const WB_RES::GattChar &c = svc.chars[i];
                uint16_t uuid16 = *reinterpret_cast<const uint16_t*>(&(c.uuid[0]));
                
                switch (uuid16)
                {
                    case ecgVoltageCharUUID16:
                        this->mEcgVoltageCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case ecgMeasurementIntervalCharUUID16:
                        this->mEcgMeasurementIntervalCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case movCharUUID16:
                        this->mMovCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case movMeasurementIntervalCharUUID16:
                        this->mMovMeasurementIntervalCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                }
            }

            // Force subscriptions asynchronously to save stack (will have stack overflow if not) 

            // Subscribe to listen to ECG Voltage Characteristics notifications (someone enables/disables the NOTIFY characteristic)
            this->asyncSubscribe(this->mEcgVoltageCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to ECG Measurement Interval Characteristics notifications (someone writes new value to ecgMeasurementIntervalChar) 
            this->asyncSubscribe(this->mEcgMeasurementIntervalCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to Movement Characteristics notifications (someone enables/disables the NOTIFY characteristic)
            this->asyncSubscribe(this->mMovCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to Movement Measurement Interval Characteristics notifications (someone writes new value to movMeasurementIntervalChar) 
            this->asyncSubscribe(this->mMovMeasurementIntervalCharResource, AsyncRequestOptions(NULL, 0, true));
            break;
        }
    }
}

void ActivityGATTSvcClient::onPostResult(wb::RequestId requestId, 
                                    wb::ResourceId resourceId, 
                                    wb::Result resultCode, 
                                    const wb::Value& rResultData)
{
    DEBUGLOG("ActivityGATTSvcClient::onPostResult: %d", resultCode);

    if (resultCode == wb::HTTP_CODE_CREATED)
    {
        // Activity GATT service was created.
        this->mActivitySvcHandle = (int32_t)rResultData.convertTo<uint16_t>();
        DEBUGLOG("Activity GATT service was created. Handle: %d", this->mActivitySvcHandle);

        // Request more info about created GATT service so we get the characteristics handles.
        this->asyncGet(
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE(),
            AsyncRequestOptions::Empty,
            this->mActivitySvcHandle
        );
    }
}

void ActivityGATTSvcClient::onNotify(wb::ResourceId resourceId,
                                const wb::Value& value,
                                const wb::ParameterList& rParameters)
{
    DEBUGLOG("ActivityGATTSvcClient::onNotify");

    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE::LID:
        {
            // Get ECG Data.
            auto ecgData = value.convertTo<const WB_RES::ECGData&>();

            // Parse timestamp.
            timestamp_t timestamp = (timestamp_t)ecgData.timestamp;

            // Parse samples and put them into sample buffer.
            size_t numberOfSamples = ecgData.samples.getNumberOfItems();
            for (size_t i = 0; i < numberOfSamples; i++)
            {
                // Convert ECG sample.
                auto ecgSample = this->convertEcgSample(ecgData.samples[i]);
                // Add converted sample to ECG buffer.
                this->ecgBuffer->addSample(ecgSample);

                // If buffer is full, add timestamp and send samples.
                if (!this->ecgBuffer->canAddSample())
                {
                    // Compute timestamp.
                    timestamp_t t = timestamp - ((numberOfSamples - i - 1) * this->ecgMeasurementInterval);
                    // Set timestamp to timestamp of last sample in buffer.
                    this->ecgBuffer->setTimestamp(t);
                    // Send samples.
                    this->sendEcgBuffer();
                }
            }
            break;
        }
        case WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::LID:
        {
            // Get Acceleration Data.
            auto accData = value.convertTo<const WB_RES::AccData&>();

            // Parse timestamp.
            timestamp_t timestamp = (timestamp_t)accData.timestamp;

            // Parse samples and put them into sample buffer.
            size_t numberOfSamples = accData.arrayAcc.getNumberOfItems();
            for (size_t i = 0; i < numberOfSamples; i++)
            {
                // Convert Acceleration sample.
                auto accSample = this->convertAccSample(accData.arrayAcc[i]);

                // Verify that cache does not overflow.
                if (this->movAccCount >= DEFAULT_MOV_OBJECT_SIZE) {
                    // Reset cache.
                    this->movAccCount = 0;
                    this->movGyrCount = 0;
                    this->movMagCount = 0;

                    // Let LED blink.
                    const WB_RES::VisualIndType type = WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION;
                    this->asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty, type);
                }

                // Write new sample to sample cache.
                this->currentMovementSamples[this->movAccCount].acc = accSample;
                // Count up number of cached movement samples.
                this->movAccCount++;

                // Verify whether cache is filled.
                if (this->movAccCount >= DEFAULT_MOV_OBJECT_SIZE
                    && this->movGyrCount >= DEFAULT_MOV_OBJECT_SIZE
                    && this->movMagCount >= DEFAULT_MOV_OBJECT_SIZE)
                {
                    // All samples are filled -> Add current samples and timestamp, and send!
                    this->finishMovementSeries(timestamp, numberOfSamples, i);
                }
            }
            break;
        }
        case WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE::LID:
        {
            // Get Gyroscope Data.
            auto gyrData = value.convertTo<const WB_RES::GyroData&>();

            // Parse timestamp.
            timestamp_t timestamp = (timestamp_t)gyrData.timestamp;

            // Parse samples and put them into sample buffer.
            size_t numberOfSamples = gyrData.arrayGyro.getNumberOfItems();
            for (size_t i = 0; i < numberOfSamples; i++)
            {
                // Convert Gyroscope sample.
                auto gyrSample = this->convertGyrSample(gyrData.arrayGyro[i]);

                // Verify that cache does not overflow.
                if (this->movGyrCount >= DEFAULT_MOV_OBJECT_SIZE) {
                    // Reset cache.
                    this->movAccCount = 0;
                    this->movGyrCount = 0;
                    this->movMagCount = 0;

                    // Let LED blink.
                    const WB_RES::VisualIndType type = WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION;
                    this->asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty, type);
                }

                // Write new sample to sample cache.
                this->currentMovementSamples[this->movGyrCount].gyr = gyrSample;
                // Count up number of cached movement samples.
                this->movGyrCount++;

                // Verify whether cache is filled.
                if (this->movAccCount >= DEFAULT_MOV_OBJECT_SIZE
                    && this->movGyrCount >= DEFAULT_MOV_OBJECT_SIZE
                    && this->movMagCount >= DEFAULT_MOV_OBJECT_SIZE)
                {
                    // All samples are filled -> Add current samples and timestamp, and send!
                    this->finishMovementSeries(timestamp, numberOfSamples, i);
                }
            }
            break;
        }
        case WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE::LID:
        {
            // Get Magnetic Field Data.
            auto magData = value.convertTo<const WB_RES::MagnData&>();

            // Parse timestamp.
            timestamp_t timestamp = (timestamp_t)magData.timestamp;

            // Parse samples and put them into sample buffer.
            size_t numberOfSamples = magData.arrayMagn.getNumberOfItems();
            for (size_t i = 0; i < numberOfSamples; i++)
            {
                // Convert Magnetic Field sample.
                auto magSample = this->convertMagSample(magData.arrayMagn[i]);

                // Verify that cache does not overflow.
                if (this->movMagCount >= DEFAULT_MOV_OBJECT_SIZE) {
                    // Reset cache.
                    this->movAccCount = 0;
                    this->movGyrCount = 0;
                    this->movMagCount = 0;

                    // Let LED blink.
                    const WB_RES::VisualIndType type = WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION;
                    this->asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty, type);
                }

                // Write new sample to sample cache.
                this->currentMovementSamples[this->movMagCount].mag = magSample;
                // Count up number of cached movement samples.
                this->movMagCount++;

                // Verify whether cache is filled.
                if (this->movMagCount >= DEFAULT_MOV_OBJECT_SIZE
                    && this->movGyrCount >= DEFAULT_MOV_OBJECT_SIZE
                    && this->movMagCount >= DEFAULT_MOV_OBJECT_SIZE)
                {
                    // All samples are filled -> Add current samples and timestamp, and send!
                    this->finishMovementSeries(timestamp, numberOfSamples, i);
                }
            }
            break;
        }
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::LID:
        {
            // Get the GATT characteristic which is handled now.
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::SUBSCRIBE::ParameterListRef parameterRef(rParameters);
            auto charHandle = parameterRef.getCharHandle();

            // Set the current GATT characteristic.
            if (charHandle == this->mEcgMeasurementIntervalCharHandle)
            {
                // Set ECG Measurement Interval GATT Characteristic:

                // Parse received Measurement Interval.
                const WB_RES::Characteristic &charValue = value.convertTo<const WB_RES::Characteristic&>();
                uint16_t interval = *reinterpret_cast<const uint16_t*>(&charValue.bytes[0]);

                DEBUGLOG("onNotify: EcgMeasurementInterval: len: %d, new interval: %d", charValue.bytes.size(), interval);

                // Update the ECG Measurement Interval.
                this->setEcgMeasurementInterval(interval);
            }
            else if (charHandle == this->mMovMeasurementIntervalCharHandle)
            {
                // Set Movement Measurement Interval GATT Characteristic:

                // Parse received Measurement Interval.
                const WB_RES::Characteristic &charValue = value.convertTo<const WB_RES::Characteristic&>();
                uint16_t interval = *reinterpret_cast<const uint16_t*>(&charValue.bytes[0]);

                DEBUGLOG("onNotify: MovMeasurementInterval: len: %d, new interval: %d", charValue.bytes.size(), interval);

                // Update the Movement Measurement Interval.
                this->setMovMeasurementInterval(interval);
            }
            break;
        }
    }
}

void ActivityGATTSvcClient::configGattSvc()
{
    // Define Activity GATT Service and its Characteristics.
    WB_RES::GattSvc activityGattSvc;
    WB_RES::GattChar characteristics[4];
    WB_RES::GattChar &ecgVoltageChar = characteristics[0];
    WB_RES::GattChar &movChar = characteristics[1];
    WB_RES::GattChar &ecgMeasurementIntervalChar = characteristics[2];
    WB_RES::GattChar &movMeasurementIntervalChar = characteristics[3];

    // Specify Characteristics's properties.
    WB_RES::GattProperty ecgVoltageCharProp = WB_RES::GattProperty::NOTIFY;
    WB_RES::GattProperty movCharProp = WB_RES::GattProperty::NOTIFY;
    WB_RES::GattProperty ecgMeasurementIntervalCharProps[2] = {
        WB_RES::GattProperty::READ,
        WB_RES::GattProperty::WRITE
    };
    WB_RES::GattProperty movMeasurementIntervalCharProps[2] = {
        WB_RES::GattProperty::READ,
        WB_RES::GattProperty::WRITE
    };

    // Specify ECG Characteristic.
    ecgVoltageChar.props = wb::MakeArray<WB_RES::GattProperty>(&ecgVoltageCharProp, 1);
    ecgVoltageChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&ecgVoltageCharUUID16), sizeof(uint16_t));

    // Specify Movement Characteristic.
    movChar.props = wb::MakeArray<WB_RES::GattProperty>(&movCharProp, 1);
    movChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&movCharUUID16), sizeof(uint16_t));

    // Specify ECG Measurement Interval Characteristic.
    ecgMeasurementIntervalChar.props = wb::MakeArray<WB_RES::GattProperty>(ecgMeasurementIntervalCharProps, 2);
    ecgMeasurementIntervalChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&ecgMeasurementIntervalCharUUID16), sizeof(uint16_t));
    ecgMeasurementIntervalChar.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&this->ecgMeasurementInterval), sizeof(uint16_t));

    // Specify Movement Measurement Interval Characteristic.
    movMeasurementIntervalChar.props = wb::MakeArray<WB_RES::GattProperty>(movMeasurementIntervalCharProps, 2);
    movMeasurementIntervalChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&movMeasurementIntervalCharUUID16), sizeof(uint16_t));
    movMeasurementIntervalChar.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&this->movMeasurementInterval), sizeof(uint16_t));

    // Combine Characteristics to Service.
    activityGattSvc.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&activitySvcUUID16), sizeof(uint16_t));
    activityGattSvc.chars = wb::MakeArray<WB_RES::GattChar>(characteristics, 4);

    // Create custom GATT Service.
    this->asyncPost(WB_RES::LOCAL::COMM_BLE_GATTSVC(), AsyncRequestOptions::Empty, activityGattSvc);
}

ecg_t ActivityGATTSvcClient::convertEcgSample(int32 ecgValue)
{
    if (ecgValue > ECG_MAX_VALUE)
    {
        return ECG_INVALID_VALUE;
    }
    else if (ecgValue < ECG_MIN_VALUE)
    {
        return ECG_INVALID_VALUE;
    }
    else
    {
        return (ecg_t)ecgValue;
    }
}

bool ActivityGATTSvcClient::sendEcgBuffer()
{
    // Get the current buffer and its size.
    size_t size = this->ecgBuffer->getSingleBufferSize();

    uint8_t* currentBuffer = this->ecgBuffer->getCurrentBuffer();

    // Move to next message buffer.
    this->ecgBuffer->switchBuffer();

    // Generate ECG Voltage Characteristics value to send.
    WB_RES::Characteristic ecgVoltageCharacteristic;
    ecgVoltageCharacteristic.bytes = wb::MakeArray<uint8_t>(currentBuffer, size);

    // Send ECG Voltage characteristics value.
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->mActivitySvcHandle,
        this->mEcgVoltageCharHandle,
        ecgVoltageCharacteristic
    );
    return true;
}

void ActivityGATTSvcClient::finishMovementSeries(timestamp_t t, size_t n, size_t i)
{
    // Compute timestamp of last added sample.
    timestamp_t timestamp = t - ((n - i - 1) * this->movMeasurementInterval);

    // Copy cached samples to cache.
    for (size_t j = 0; j < DEFAULT_MOV_OBJECT_SIZE; j++)
    {
        // Add sample to movement buffer.
        this->movBuffer->addSample(this->currentMovementSamples[j]);

        // If buffer is full, add timestamp and send movement data.
        if (!this->movBuffer->canAddSample())
        {
            // Compute timestamp of last sample in buffer.
            timestamp_t time = timestamp - ((DEFAULT_MOV_OBJECT_SIZE - j - 1) * this->movMeasurementInterval);

            // Set timestamp to timestamp of last sample in buffer.
            this->movBuffer->setTimestamp(time);

            // Send samples.
            bool result = this->sendMovBuffer();
        }
    }

    // Reset counters.
    this->movAccCount = 0;
    this->movGyrCount = 0;
    this->movMagCount = 0;
}

acc_vec_t ActivityGATTSvcClient::convertAccSample(whiteboard::FloatVector3D accVector)
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

gyr_vec_t ActivityGATTSvcClient::convertGyrSample(whiteboard::FloatVector3D gyrVector)
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

mag_vec_t ActivityGATTSvcClient::convertMagSample(whiteboard::FloatVector3D magVector)
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

bool ActivityGATTSvcClient::sendMovBuffer()
{
    // Get the current buffer and its size.
    size_t size = this->movBuffer->getSingleBufferSize();

    uint8_t* currentBuffer = this->movBuffer->getCurrentBuffer();

    // Move to next message buffer.
    this->movBuffer->switchBuffer();

    // Generate Movement Characteristics value to send.
    WB_RES::Characteristic movCharacteristic;
    movCharacteristic.bytes = wb::MakeArray<uint8_t>(currentBuffer, size);

    // Send Movement characteristics value.
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->mActivitySvcHandle,
        this->mMovCharHandle,
        movCharacteristic
    );
    return true;
}

uint32_t ActivityGATTSvcClient::getEcgSampleRate()
{
    return (uint32_t)(1000 / this->ecgMeasurementInterval);
}

void ActivityGATTSvcClient::setEcgMeasurementInterval(uint16_t value)
{
    // Ensure that value is valid or fall back to `DEFAULT_ECG_MEASUREMENT_INTERVAL`.
    switch (value)
    {
        case 2: // 500 Hz
        case 4: // 250 Hz
        case 5: // 200 Hz
        case 8: // 125 Hz
            break;
        default:
            value = DEFAULT_ECG_MEASUREMENT_INTERVAL;
            break;
    }

    // Unsubscribe from current ECG subscription.
    this->unsubscribeFromEcgSamples();
    // Update measurement interval.
    this->ecgMeasurementInterval = value;
    // Set measurement interval to GATT Characteristics value.
    WB_RES::Characteristic measurementIntervalChar;
    measurementIntervalChar.bytes = wb::MakeArray<uint8_t>((uint8_t*)&this->ecgMeasurementInterval, sizeof(uint16_t));
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->mActivitySvcHandle,
        this->mEcgMeasurementIntervalCharHandle,
        measurementIntervalChar
    );
    // Reset current ECG buffer and start over.
    this->ecgBuffer->resetCurrentBuffer();
    // Subscribe to new ECG subscription.
    this->subscribeToEcgSamples();
}

void ActivityGATTSvcClient::subscribeToEcgSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getEcgSampleRate();
    // Subscribe to ECG samples with the desired ECG sample rate.
    this->asyncSubscribe(
        WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void ActivityGATTSvcClient::unsubscribeFromEcgSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getEcgSampleRate();
    // Unsubscribe from ECG samples with desired ECG sample rate.
    this->asyncUnsubscribe(
        WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

uint32_t ActivityGATTSvcClient::getMovSampleRate()
{
    switch (this->movMeasurementInterval)
    {
        case 5:
            return 208;
        case 10:
            return 104;
        case 20:
            return 52;
        case 40:
            return 26;
        default:
            return 52;
    }
}

void ActivityGATTSvcClient::setMovMeasurementInterval(uint16_t value)
{
    // Unsubscribe from current Movement subscriptions.
    this->unsubscribeFromAccSamples();
    this->unsubscribeFromGyrSamples();
    this->unsubscribeFromMagSamples();
    // Update measurement interval.
    this->movMeasurementInterval = value;
    // Set measurement interval to GATT Characteristics value.
    WB_RES::Characteristic measurementIntervalChar;
    measurementIntervalChar.bytes = wb::MakeArray<uint8_t>((uint8_t*)&this->movMeasurementInterval, sizeof(uint16_t));
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->mActivitySvcHandle,
        this->mMovMeasurementIntervalCharHandle,
        measurementIntervalChar
    );
    // Reset current Movement buffers and start over.
    this->movBuffer->resetCurrentBuffer();
    this->movAccCount = 0;
    this->movGyrCount = 0;
    this->movMagCount = 0;
    // Subscribe to new Movement subscriptions.
    this->subscribeToAccSamples();
    this->subscribeToGyrSamples();
    this->subscribeToMagSamples();
}

void ActivityGATTSvcClient::subscribeToAccSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getMovSampleRate();
    // Subscribe to Acceleration samples with the desired Acceleration sample rate.
    this->asyncSubscribe(
        WB_RES::LOCAL::MEAS_ACC_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void ActivityGATTSvcClient::unsubscribeFromAccSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getMovSampleRate();
    // Unsubscribe from Acceleration samples with desired Acceleration sample rate.
    this->asyncUnsubscribe(
        WB_RES::LOCAL::MEAS_ACC_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void ActivityGATTSvcClient::subscribeToGyrSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getMovSampleRate();
    // Subscribe to Gyroscope samples with the desired Gyroscope sample rate.
    this->asyncSubscribe(
        WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void ActivityGATTSvcClient::unsubscribeFromGyrSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getMovSampleRate();
    // Unsubscribe from Gyroscope samples with desired Gyroscope sample rate.
    this->asyncUnsubscribe(
        WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void ActivityGATTSvcClient::subscribeToMagSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getMovSampleRate();
    // Subscribe to Magnetic Field samples with the desired Magnetic Field sample rate.
    this->asyncSubscribe(
        WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void ActivityGATTSvcClient::unsubscribeFromMagSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getMovSampleRate();
    // Unsubscribe from Magnetic Field samples with desired Magnetic Field sample rate.
    this->asyncUnsubscribe(
        WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}
