#include "AccelerometerSampleService.h"
#include "app-resources/resources.h"
#include "common/core/debug.h"
#include "suunto_device_measurement_linearacceleration/resources.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"
#include "whiteboard/integration/bsp/shared/debug.h"

#include <float.h>
#include <math.h>

#define ASSERT WB_DEBUG_ASSERT

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
      LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId)
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
    DEBUGLOG("AccelerometerSampleService::startRunning()");

    // Reset max acceleration members
    mMaxAccelerationSq = FLT_MIN;
    mSamplesIncluded = 0;

    // Subscribe to LinearAcceleration resource (updates at 13Hz), when subscribe is done, we get callback
    wb::Result result = asyncSubscribe(WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID, AsyncRequestOptions(&remoteRequestId, 0, true), SAMPLE_RATE);
    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncSubscribe threw error: %u", result);
        return whiteboard::HTTP_CODE_BAD_REQUEST;
    }

    return whiteboard::HTTP_CODE_OK;
}

whiteboard::Result AccelerometerSampleService::stopRunning()
{
    DEBUGLOG("AccelerometerSampleService::stopRunning()");

    // Unsubscribe the LinearAcceleration resource, when unsubscribe is done, we get callback
    wb::Result result = asyncUnsubscribe(WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID, NULL, SAMPLE_RATE);
    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncUnsubscribe threw error: %u", result);
    }

    return whiteboard::HTTP_CODE_OK;
}

// This callback is called when the resource we have subscribed notifies us
void AccelerometerSampleService::onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                                          const whiteboard::ParameterList& parameters)
{
    DEBUGLOG("onNotify() called.");

    // Confirm that it is the correct resource
    switch (resourceId.getConstId())
    {
    case WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID:
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
            updateResource(WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::ID,
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

        if (!whiteboard::RETURN_OK(result))
        {
            return returnResult(request, result);
        }

        bool queueResult = mOngoingRequests.put(remoteRequestId, request);
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
