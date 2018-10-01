#include "movesense.h"

#include "SensorOutputService.h"

#include "meas_acc/resources.h"
#include "meas_gyro/resources.h"
#include "meas_magn/resources.h"

#include "common/core/debug.h"

#define DEFAULT_ACC_SAMPLERATE (13)
#define DEFAULT_PERIOD_MS (10000)
#define DEFAULT_SENSOR (WB_RES::SensorType::ACC_SENSOR)
#define DEFAULT_PATH ("UNKNOWN")

// Values of range which the probes should fit when checking the timestamps
#define TRIM_RANGE_LOW (0.95)
#define TRIM_RANGE_HIGH (1.05)

const char* const SensorOutputService::LAUNCHABLE_NAME = "SensOutput";

static const whiteboard::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::SAMPLE_SENSOROUTPUT::LID,
    WB_RES::LOCAL::SAMPLE_SENSOROUTPUT_CONFIG::LID};

SensorOutputService::SensorOutputService()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_RES::LOCAL::SAMPLE_SENSOROUTPUT::EXECUTION_CONTEXT),
      ResourceProvider(WBDEBUG_NAME(__FUNCTION__), WB_RES::LOCAL::SAMPLE_SENSOROUTPUT::EXECUTION_CONTEXT),
      LaunchableModule(LAUNCHABLE_NAME, WB_RES::LOCAL::SAMPLE_SENSOROUTPUT::EXECUTION_CONTEXT)
{
    mTimestampGaps = 0;
    mBadSensorSamples = 0;
    mGoodSensorSamples = 0;
    mTestConfig.period = (DEFAULT_PERIOD_MS / timerIntervalInMs);
    mTestConfig.sampleRate = DEFAULT_ACC_SAMPLERATE;
    mTestConfig.path = DEFAULT_PATH;
    mTestPeriodTimer = whiteboard::ID_INVALID_TIMER;
}

SensorOutputService::~SensorOutputService()
{
}

bool SensorOutputService::initModule()
{
    if (registerProviderResources(sProviderResources) != whiteboard::HTTP_CODE_OK)
    {
        return false;
    }

    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void SensorOutputService::deinitModule()
{
    // Make sure that timer is already stopped when module is being deinitialized
    if (mTestPeriodTimer != whiteboard::ID_INVALID_TIMER)
    {
        mTestPeriodTimer = whiteboard::ID_INVALID_TIMER;
    }

    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool SensorOutputService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    return true;
}

void SensorOutputService::onGetRequest(const whiteboard::Request& request,
                                       const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_SENSOROUTPUT_CONFIG::ID:
    {
        DEBUGLOG("SensorOutputService::onGetRequest() called.");
        returnResult(request, whiteboard::HTTP_CODE_OK, ResponseOptions::Empty, mTestConfig);
    }
    break;

    default:
    {
        DEBUGLOG("ERROR: Resource not recognized!");
    }
    break;
    }
}

void SensorOutputService::onPutRequest(const whiteboard::Request& request,
                                       const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_SENSOROUTPUT_CONFIG::ID:
    {
        DEBUGLOG("SensorOutputService::onPutRequest() called.");

        if (isResourceSubscribed(WB_RES::LOCAL::SAMPLE_SENSOROUTPUT::ID) == wb::HTTP_CODE_OK)
        {
            DEBUGLOG("SensorOutputService::onPutRequest() skipping. Subscribers still exist.");
            returnResult(request, whiteboard::HTTP_CODE_SERVICE_UNAVAILABLE);
        }
        else
        {
            // Parse and gather the specified settings
            auto testConfig = WB_RES::LOCAL::SAMPLE_SENSOROUTPUT_CONFIG::PUT::ParameterListRef(parameters).getConfig();
            if (0 < testConfig.period)
            {
                mTestConfig.period = testConfig.period;
            }
            else
            {
                DEBUGLOG("ERROR: Period [ms] cannot be 0!");
                returnResult(request, whiteboard::HTTP_CODE_BAD_REQUEST);
                break;
            }

            // Parse the sensor path requested and check for errors
            if (true != getSensorRequestedByUser(testConfig.path))
            {
                DEBUGLOG("ERROR: Wrong or no path was given");
                returnResult(request, whiteboard::HTTP_CODE_BAD_REQUEST);
                break;
            }

            // Parse the sample rate and check for errors
            if (true == checkSampleRateRequestedByUser(testConfig.sampleRate))
            {
                mTestConfig.sampleRate = testConfig.sampleRate;
            }
            else
            {
                DEBUGLOG("ERROR: Wrong or no sample rate specified!");
                returnResult(request, whiteboard::HTTP_CODE_BAD_REQUEST);
                break;
            }

            returnResult(request, whiteboard::HTTP_CODE_OK);

            // Configuration has been changed - reset current measurements
            resetCurrentMeasurement();
        }
    }
    break;

    default:
    {
        returnResult(request, whiteboard::HTTP_CODE_SERVICE_UNAVAILABLE);
    }
    break;
    }
}

void SensorOutputService::onSubscribe(const whiteboard::Request& request,
                                      const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_SENSOROUTPUT::ID:
    {
        DEBUGLOG("SensorOutput is being subscribed with the ID: %d", WB_RES::LOCAL::SAMPLE_SENSOROUTPUT::ID);
        whiteboard::RequestId remoteRequestId;
        wb::Result result = asyncSubscribe(subscribedSensor, AsyncRequestOptions(&remoteRequestId, 0, true), mTestConfig.sampleRate);

        // Check if subscribing the selected sensor went OK
        if (wb::RETURN_OKC(result))
        {
            // Clear temporary variables from last subscription
            lastDiff = 0;
            lastTimestamp = 0;
            returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty);
        }
        else
        {
            DEBUGLOG("ERROR: Subscribing the sensor failed with error: %u", result);
            returnResult(request, wb::HTTP_CODE_NOT_FOUND, ResponseOptions::Empty);
        }
    }
    break;

    default:
    {
        returnResult(request, whiteboard::HTTP_CODE_NOT_FOUND);
    }
    break;
    }
}

void SensorOutputService::onUnsubscribe(const whiteboard::Request& request,
                                        const whiteboard::ParameterList& parameters)
{
    DEBUGLOG("SensorOutputService::onUnsubscribe()");

    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_SENSOROUTPUT::ID:
    {
        if (wb::RETURN_OKC(asyncUnsubscribe(subscribedSensor, NULL, mTestConfig.sampleRate)))
        {
            returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty);
        }
        else
        {
            DEBUGLOG("ERROR: asyncUnsubscribe failed!");
        }
    }
    break;

    default:
    {
        returnResult(request, wb::HTTP_CODE_NOT_FOUND);
    }
    break;
    }
}

void SensorOutputService::onUnsubscribeResult(whiteboard::RequestId requestId,
                                              whiteboard::ResourceId resourceId,
                                              whiteboard::Result resultCode,
                                              const whiteboard::Value& rResultData)
{
    DEBUGLOG("SensorOutputService::onUnsubscribeResult() called.");

    // Unsubscribing went OK so stop the timer
    ResourceProvider::stopTimer(mTestPeriodTimer);
    mTestPeriodTimer = whiteboard::ID_INVALID_TIMER;
}

void SensorOutputService::onSubscribeResult(whiteboard::RequestId requestId,
                                            whiteboard::ResourceId resourceId,
                                            whiteboard::Result resultCode,
                                            const whiteboard::Value& rResultData)
{
    DEBUGLOG("SensorOutputService::onSubscribeResult() called.");
    if (whiteboard::ID_INVALID_TIMER == mTestPeriodTimer)
    {
        mTestPeriodTimer = ResourceProvider::startTimer(mTestConfig.period, true);
    }
}

void SensorOutputService::onTimer(whiteboard::TimerId timerId)
{
    if (timerId == mTestPeriodTimer)
    {
        // Gather and display all the required data
        WB_RES::TestResult testResult;
        testResult.gaps = mTimestampGaps;
        testResult.probesBad = mBadSensorSamples;
        testResult.probesGood = mGoodSensorSamples;
        updateResource(WB_RES::LOCAL::SAMPLE_SENSOROUTPUT(), ResponseOptions::Empty, testResult);
    }
    else
    {
        DEBUGLOG("ERROR: Wrong timer ID!");
    }
}

void SensorOutputService::onNotify(whiteboard::ResourceId resourceId,
                                   const whiteboard::Value& value,
                                   const whiteboard::ParameterList& parameters)
{
    // Decide which sensor needs to be handled and verify its output
    switch (resourceId.localResourceId)
    {
    case WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::LID:
    {
        const WB_RES::AccData& currentSensorValue = value.convertTo<const WB_RES::AccData&>();

        if ((currentSensorValue.arrayAcc.size()))
        {
            const whiteboard::Array<whiteboard::FloatVector3D>& arrayData = currentSensorValue.arrayAcc;
            timeDiffCheck(currentSensorValue.timestamp);
            probesCheck(arrayData);
        }
    }
    break;

    case WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE::LID:
    {
        const WB_RES::GyroData& currentSensorValue = value.convertTo<const WB_RES::GyroData&>();

        if ((currentSensorValue.arrayGyro.size()))
        {
            const whiteboard::Array<whiteboard::FloatVector3D>& arrayData = currentSensorValue.arrayGyro;
            timeDiffCheck(currentSensorValue.timestamp);
            probesCheck(arrayData);
        }
    }
    break;

    case WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE::LID:
    {
        const WB_RES::MagnData& currentSensorValue = value.convertTo<const WB_RES::MagnData&>();

        if ((currentSensorValue.arrayMagn.size()))
        {
            const whiteboard::Array<whiteboard::FloatVector3D>& arrayData = currentSensorValue.arrayMagn;
            timeDiffCheck(currentSensorValue.timestamp);
            probesCheck(arrayData);
        }
    }
    break;

    default:
    {
        DEBUGLOG("ERROR: Not recognized sensor ID!");
    }
    break;
    }
}

void SensorOutputService::timeDiffCheck(const uint32_t currentTimestamp)
{
    uint32_t tempDiff = currentTimestamp - lastTimestamp;

    // Display the current timestamp information in the logs
    DEBUGLOG("TimeStampDiff: %d    %d    %d    %d", currentTimestamp, lastTimestamp, tempDiff, lastDiff);

    if (tempDiff < 0)
    {
        mTimestampGaps++;
    }
    else if (lastTimestamp != lastDiff)
    {
        // Gap spotted if the current difference is beyond specified range of last difference
        if ((tempDiff < (TRIM_RANGE_LOW * lastDiff)) || ((TRIM_RANGE_HIGH * lastDiff) < tempDiff))
        {
            mTimestampGaps++;
        }
    }

    // Overwrite the current values of timestamp information
    lastDiff = tempDiff;
    lastTimestamp = currentTimestamp;
}

void SensorOutputService::probesCheck(const whiteboard::Array<whiteboard::FloatVector3D>& arrayData)
{
    // Count the amount of samples received from each sensor
    mGoodSensorSamples += arrayData.size();

    for (size_t i = 0; i < arrayData.size(); i++)
    {
        whiteboard::FloatVector3D accValue = arrayData[i];
    }
}

bool SensorOutputService::getSensorRequestedByUser(const char* sensorPathSpecifiedByUser)
{
    // Keep the correctness status - success as default
    bool isPathCorrect = true;

    // Make sure that no NULL was given as parameter
    if (NULL != sensorPathSpecifiedByUser)
    {
        // Keep the matching types of sensor paths for the comparison
        const char* comparisonPaths[] = {"MEAS_ACC", "MEAS_GYRO", "MEAS_MAGN"};
        if (0 == strcmp(sensorPathSpecifiedByUser, comparisonPaths[0]))
        {
            subscribedSensor = WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID;
            mTestConfig.path = comparisonPaths[0];
        }
        else if (0 == strcmp(sensorPathSpecifiedByUser, comparisonPaths[1]))
        {
            subscribedSensor = WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE::ID;
            mTestConfig.path = comparisonPaths[1];
        }
        else if (0 == strcmp(sensorPathSpecifiedByUser, comparisonPaths[2]))
        {
            subscribedSensor = WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE::ID;
            mTestConfig.path = comparisonPaths[2];
        }
        else
        {
            isPathCorrect = false;
        }
    }
    else
    {
        // No or wrong path specified - return false as failure status
        isPathCorrect = false;
    }

    // Notify about the stauts
    return isPathCorrect;
}

bool SensorOutputService::checkSampleRateRequestedByUser(int32_t sampleRateSpecifiedByUser)
{
    const uint8_t ACC_SAMPLERATES_SIZE = 8;
    const uint16 LSM6DS3_SAMPLERATES_ARRAY[ACC_SAMPLERATES_SIZE] = {13, 26, 52, 104, 208, 416, 833, 1666};
    uint8_t checkedSampleRate = 0;

    // Go through the array to search for matching sample rate
    while (checkedSampleRate < ACC_SAMPLERATES_SIZE)
    {
        if (LSM6DS3_SAMPLERATES_ARRAY[checkedSampleRate] == sampleRateSpecifiedByUser)
        {
            return true;
        }
        else
        {
            // No match - continue comparing
            checkedSampleRate++;
        }
    }

    // No match till the end of available sample rates - notify about the wrong sample rate
    return false;
}

void SensorOutputService::resetCurrentMeasurement()
{
    mTimestampGaps = 0;
    mBadSensorSamples = 0;
    mGoodSensorSamples = 0;
    lastDiff = 0;
    lastTimestamp = 0;
}
