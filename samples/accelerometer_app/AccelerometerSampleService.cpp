#include "AccelerometerSampleService.h"
#include "app-resources/resources.h"
#include "common/core/debug.h"
#include "meas_acc/resources.h"

#include <float.h>

#define ACC_PATH_WITH_SAMPLERATE "Meas/Acc/13"

const char* const AccelerometerSampleService::LAUNCHABLE_NAME = "AccSample";

static const wb::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::EXECUTION_CONTEXT;

static const wb::LocalResourceId sProviderResources[] =
{
    WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::LID,
};

AccelerometerSampleService::AccelerometerSampleService()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId),
      mOngoingRequests(),
      mMeasAccResourceId(wb::ID_INVALID_RESOURCE),
      isRunning(false),
      mSamplesIncluded(0),
      mMaxAccelerationSq(0.0f)
{
}

AccelerometerSampleService::~AccelerometerSampleService()
{
}

bool AccelerometerSampleService::initModule()
{
    if (registerProviderResources(sProviderResources) != wb::HTTP_CODE_OK)
    {
        return false;
    }

    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void AccelerometerSampleService::deinitModule()
{
    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool AccelerometerSampleService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void AccelerometerSampleService::stopModule()
{
    stopRunning();
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void AccelerometerSampleService::onUnsubscribeResult(wb::RequestId requestId,
                                                     wb::ResourceId resourceId,
                                                     wb::Result resultCode,
                                                     const wb::Value& rResultData)
{
    DEBUGLOG("AccelerometerSampleService::onUnsubscribeResult() called.");
}

void AccelerometerSampleService::onSubscribeResult(wb::RequestId requestId,
                                                   wb::ResourceId resourceId,
                                                   wb::Result resultCode,
                                                   const wb::Value& rResultData)
{
    DEBUGLOG("AccelerometerSampleService::onSubscribeResult() called. resourceId: %u, result: %u",
             resourceId.localResourceId, resultCode);

    wb::Request relatedIncomingRequest;
    bool relatedRequestFound = mOngoingRequests.get(requestId, relatedIncomingRequest);

    if (relatedRequestFound)
    {
        returnResult(relatedIncomingRequest, wb::HTTP_CODE_OK);
    }
}

wb::Result AccelerometerSampleService::startRunning(wb::RequestId& remoteRequestId)
{
    if (isRunning)
    {
        return wb::HTTP_CODE_OK;
    }

    DEBUGLOG("AccelerometerSampleService::startRunning()");

    // Reset max acceleration members
    mMaxAccelerationSq = FLT_MIN;
    mSamplesIncluded = 0;

    wb::Result result = getResource(ACC_PATH_WITH_SAMPLERATE, mMeasAccResourceId);
    if (!wb::RETURN_OKC(result))
    {
        return result;
    }

    result = asyncSubscribe(mMeasAccResourceId, AsyncRequestOptions(&remoteRequestId, 0, true));

    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncSubscribe threw error: %u", result);
        return wb::HTTP_CODE_BAD_REQUEST;
    }
    isRunning = true;

    return wb::HTTP_CODE_OK;
}

wb::Result AccelerometerSampleService::stopRunning()
{
    if (!isRunning)
    {
        return wb::HTTP_CODE_OK;
    }

    if (isResourceSubscribed(WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::ID) == wb::HTTP_CODE_OK)
    {
        DEBUGLOG("AccelerometerSampleService::stopRunning() skipping. Subscribers still exist.");
        return wb::HTTP_CODE_OK;
    }

    DEBUGLOG("AccelerometerSampleService::stopRunning()");

    // Unsubscribe the LinearAcceleration resource, when unsubscribe is done, we get callback
    wb::Result result = asyncUnsubscribe(mMeasAccResourceId, NULL);
    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncUnsubscribe threw error: %u", result);
    }
    isRunning = false;
    releaseResource(mMeasAccResourceId);

    return wb::HTTP_CODE_OK;
}

void AccelerometerSampleService::onNotify(wb::ResourceId resourceId,
                                          const wb::Value& value,
                                          const wb::ParameterList& parameters)
{
    DEBUGLOG("onNotify() called.");

    // Confirm that it is the correct resource
    switch (resourceId.localResourceId)
    {
    case WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::LID:
    {
        const WB_RES::AccData& linearAccelerationValue =
            value.convertTo<const WB_RES::AccData&>();

        if (linearAccelerationValue.arrayAcc.size() <= 0)
        {
            // No value, do nothing...
            return;
        }

        const wb::Array<wb::FloatVector3D>& arrayData = linearAccelerationValue.arrayAcc;

        uint32_t relativeTime = linearAccelerationValue.timestamp;

        for (size_t i = 0; i < arrayData.size(); i++)
        {
            mSamplesIncluded++;

            wb::FloatVector3D accValue = arrayData[i];
            float accelerationSq = accValue.mX * accValue.mX +
                                   accValue.mY * accValue.mY +
                                   accValue.mZ * accValue.mZ;

            if (mMaxAccelerationSq < accelerationSq)
                mMaxAccelerationSq = accelerationSq;
        }

        // 13Hz 5sec
        if (mSamplesIncluded > 5 * 13)
        {
            // Reset counter and notify our subscribers
            WB_RES::SampleDataValue sampleDataValue;
            sampleDataValue.relativeTime = relativeTime;
            sampleDataValue.value = sqrtf(mMaxAccelerationSq);

            // Reset members
            mSamplesIncluded = 0;
            mMaxAccelerationSq = FLT_MIN;

            // and update our WB resource. This causes notification to be fired to our subscribers
            updateResource(WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA(), ResponseOptions::Empty, sampleDataValue);
        }
        break;
    }
    }
}

void AccelerometerSampleService::onSubscribe(const wb::Request& request,
                                             const wb::ParameterList& parameters)
{
    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::LID:
    {
        // Someone subscribed to our service. Start collecting data and notifying when our service changes state (every 10 seconds)
        wb::RequestId remoteRequestId;
        wb::Result result = startRunning(remoteRequestId);

        if (isRunning)
        {
            returnResult(request, wb::HTTP_CODE_OK);
            return;
        }

        if (!wb::RETURN_OK(result))
        {
            returnResult(request, result);
            return;
        }
        bool queueResult = mOngoingRequests.put(remoteRequestId, request);
        (void)queueResult;
        WB_ASSERT(queueResult);
        break;
    }
    default:
        ASSERT(0); // Should not happen
    }
}

void AccelerometerSampleService::onUnsubscribe(const wb::Request& request,
                                               const wb::ParameterList& parameters)
{
    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::LID:
        stopRunning();
        returnResult(request, wb::HTTP_CODE_OK);
        break;

    default:
        ASSERT(0); // Should not happen
    }
}

void AccelerometerSampleService::onRemoteWhiteboardDisconnected(wb::WhiteboardId whiteboardId)
{
    DEBUGLOG("AccelerometerSampleService::onRemoteWhiteboardDisconnected()");
    stopRunning();
}

void AccelerometerSampleService::onClientUnavailable(wb::ClientId clientId)
{
    DEBUGLOG("AccelerometerSampleService::onClientUnavailable()");
    stopRunning();
}