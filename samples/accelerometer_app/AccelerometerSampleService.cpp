#include "AccelerometerSampleService.h"
#include "app-resources/resources.h"
#include "common/core/debug.h"
#include "meas_acc/resources.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"

#include <float.h>
#include <math.h>

const char* const AccelerometerSampleService::LAUNCHABLE_NAME = "SampleA";
#define SAMPLE_RATE 13
static const whiteboard::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::EXECUTION_CONTEXT;

static const whiteboard::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::LID,
};

AccelerometerSampleService::AccelerometerSampleService()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId),
      isRunning(false)
{
}

AccelerometerSampleService::~AccelerometerSampleService()
{
}

bool AccelerometerSampleService::initModule()
{
    if (registerProviderResources(sProviderResources) != whiteboard::HTTP_CODE_OK)
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

/** @see whiteboard::ILaunchableModule::startModule */
bool AccelerometerSampleService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    return true;
}

void AccelerometerSampleService::onUnsubscribeResult(whiteboard::RequestId requestId,
                                                     whiteboard::ResourceId resourceId,
                                                     whiteboard::Result resultCode,
                                                     const whiteboard::Value& rResultData)
{
    DEBUGLOG("AccelerometerSampleService::onUnsubscribeResult() called.");
}

void AccelerometerSampleService::onSubscribeResult(whiteboard::RequestId requestId,
                                                   whiteboard::ResourceId resourceId,
                                                   whiteboard::Result resultCode,
                                                   const whiteboard::Value& rResultData)
{
    DEBUGLOG("AccelerometerSampleService::onSubscribeResult() called. resourceId: %u, result: %d", resourceId.localResourceId, (uint32_t)resultCode);

    whiteboard::Request relatedIncomingRequest;
    bool relatedRequestFound = mOngoingRequests.get(requestId, relatedIncomingRequest);

    if (relatedRequestFound)
    {
        returnResult(relatedIncomingRequest, wb::HTTP_CODE_OK);
    }
}

whiteboard::Result AccelerometerSampleService::startRunning(whiteboard::RequestId& remoteRequestId)
{
    if (isRunning)
    {
        return whiteboard::HTTP_CODE_OK;
    }

    DEBUGLOG("AccelerometerSampleService::startRunning()");

    // Reset max acceleration members
    mMaxAccelerationSq = FLT_MIN;
    mSamplesIncluded = 0;

    wb::Result result = getResource("Meas/Acc/13", mMeasAccResourceId);
    if (!wb::RETURN_OKC(result))
    {
        return result;
    }

    result = asyncSubscribe(mMeasAccResourceId, AsyncRequestOptions(&remoteRequestId, 0, true));

    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncSubscribe threw error: %u", result);
        return whiteboard::HTTP_CODE_BAD_REQUEST;
    }
    isRunning = true;

    return whiteboard::HTTP_CODE_OK;
}

whiteboard::Result AccelerometerSampleService::stopRunning()
{
    if (!isRunning)
    {
        return whiteboard::HTTP_CODE_OK;
    }

    if (isResourceSubscribed(WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::ID) == wb::HTTP_CODE_OK)
    {
        DEBUGLOG("AccelerometerSampleService::stopRunning() skipping. Subscribers still exist.");
        return whiteboard::HTTP_CODE_OK;
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

    return whiteboard::HTTP_CODE_OK;
}

// This callback is called when the resource we have subscribed notifies us
void AccelerometerSampleService::onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                                          const whiteboard::ParameterList& parameters)
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

        const whiteboard::Array<whiteboard::FloatVector3D>& arrayData = linearAccelerationValue.arrayAcc;

        uint32_t relativeTime = linearAccelerationValue.timestamp;

        for (size_t i = 0; i < arrayData.size(); i++)
        {
            mSamplesIncluded++;

            whiteboard::FloatVector3D accValue = arrayData[i];
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
            updateResource(WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA(),
                           ResponseOptions::Empty, sampleDataValue);
        }
    }
    break;
    }
}

void AccelerometerSampleService::onSubscribe(const whiteboard::Request& request,
                                             const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::ID:
    {
        // Someone subscribed to our service. Start collecting data and notifying when our service changes state (every 10 seconds)
        whiteboard::RequestId remoteRequestId;
        whiteboard::Result result = startRunning(remoteRequestId);

        if (isRunning)
        {
            return returnResult(request, whiteboard::HTTP_CODE_OK);
        }

        if (!whiteboard::RETURN_OK(result))
        {
            return returnResult(request, result);
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

void AccelerometerSampleService::onUnsubscribe(const whiteboard::Request& request,
                                               const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::ID:
        stopRunning();
        returnResult(request, wb::HTTP_CODE_OK);
        break;

    default:
        returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
        ASSERT(0); // Should not happen
    }
}

void AccelerometerSampleService::onRemoteWhiteboardDisconnected(whiteboard::WhiteboardId whiteboardId)
{
    DEBUGLOG("AccelerometerSampleService::onRemoteWhiteboardDisconnected()");
    stopRunning();
}

void AccelerometerSampleService::onClientUnavailable(whiteboard::ClientId clientId)
{
    DEBUGLOG("AccelerometerSampleService::onClientUnavailable()");
    stopRunning();
}