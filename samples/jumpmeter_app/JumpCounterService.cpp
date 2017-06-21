#include "JumpCounterService.h"
#include "app-resources/resources.h"
#include "common/core/debug.h"
#include "suunto_device_measurement_linearacceleration/resources.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"
#include "whiteboard/integration/bsp/shared/debug.h"

#include <float.h>
#include <math.h>

#define ASSERT WB_DEBUG_ASSERT
#define ACC_SAMPLERATE 104

const char* const JumpCounterService::LAUNCHABLE_NAME = "JumpC";

static const whiteboard::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::EXECUTION_CONTEXT;

static const whiteboard::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::LID,
    WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::LID,
};

JumpCounterService::JumpCounterService()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId)
{
    mJumpCounter = 0;
    mLastJumpHeight = 0.0f;
    mSubscriptionCount = 0;
}

JumpCounterService::~JumpCounterService()
{
}

bool JumpCounterService::initModule()
{
    if (registerProviderResources(sProviderResources) != whiteboard::HTTP_CODE_OK)
    {
        return false;
    }

    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void JumpCounterService::deinitModule()
{
    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

/** @see whiteboard::ILaunchableModule::startModule */
bool JumpCounterService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void JumpCounterService::onGetRequest(const whiteboard::Request& request,
                                      const whiteboard::ParameterList& parameters)
{
    DEBUGLOG("JumpCounterService::onGetRequest() called.");

    if (mModuleState != WB_RES::ModuleStateValues::STARTED)
    {
        return returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);
    }

    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::ID:
    {
        return returnResult(request, whiteboard::HTTP_CODE_OK,
                            ResponseOptions::Empty, mJumpCounter);
    }

    break;

    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::ID:
    {
        return returnResult(request, whiteboard::HTTP_CODE_OK,
                            ResponseOptions::Empty, mLastJumpHeight);
    }

    break;

    default:
        // Return error
        return returnResult(request, whiteboard::HTTP_CODE_NOT_IMPLEMENTED);
    }
}

void JumpCounterService::onUnsubscribeResult(whiteboard::RequestId requestId,
                                             whiteboard::ResourceId resourceId,
                                             whiteboard::Result resultCode,
                                             const whiteboard::Value& rResultData)
{
    DEBUGLOG("JumpCounterService::onUnsubscribeResult() called.");
}

void JumpCounterService::onSubscribeResult(whiteboard::RequestId requestId,
                                           whiteboard::ResourceId resourceId,
                                           whiteboard::Result resultCode,
                                           const whiteboard::Value& rResultData)
{
    DEBUGLOG("JumpCounterService::onSubscribeResult() called. resourceId: %u, result: %d", resourceId.localResourceId, (uint32_t)resultCode);

    whiteboard::Request relatedIncomingRequest;
    bool relatedRequestFound = mOngoingRequests.get(requestId, relatedIncomingRequest);

    if (relatedRequestFound)
    {
        returnResult(relatedIncomingRequest, wb::HTTP_CODE_OK);
    }
}

whiteboard::Result JumpCounterService::startRunning(whiteboard::RequestId& remoteRequestId)
{
    DEBUGLOG("JumpCounterService::startRunning()");

    // Reset 0G detection count
    mSamplesSince0GStart = 0;

    // Subscribe to LinearAcceleration resource (updates at ACC_SAMPLERATE Hz), when subscribe is done, we get callback
    wb::Result result = asyncSubscribe(WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID, AsyncRequestOptions(&remoteRequestId, 0, true), ACC_SAMPLERATE);
    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncSubscribe threw error: %u", result);
        return whiteboard::HTTP_CODE_BAD_REQUEST;
    }

    return whiteboard::HTTP_CODE_OK;
}

whiteboard::Result JumpCounterService::stopRunning()
{
    DEBUGLOG("JumpCounterService::stopRunning()");

    // Unsubscribe the LinearAcceleration resource, when unsubscribe is done, we get callback
    wb::Result result = asyncUnsubscribe(WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID, NULL, ACC_SAMPLERATE);
    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncUnsubscribe threw error: %u", result);
    }

    return whiteboard::HTTP_CODE_OK;
}

void JumpCounterService::jumpFound(size_t nJumpSamples)
{
    mJumpCounter++;

    // Calc jump length
    // TODO: Add support for flexible samplerate
    const float ONE_PER_SAMPLE_RATE = 1.0f / (float)ACC_SAMPLERATE;
    const float EARTH_G = 9.81f;

    float jumpDuration = (float)nJumpSamples * ONE_PER_SAMPLE_RATE;
    // s = 1/2 * a * t^2, a= 1G, and t is half time of 0-G (we go up and down in jump)
    float t = jumpDuration * 0.5f;
    float jumpHeight = 0.5f * (t * t) * EARTH_G;

    // Save jump height
    mLastJumpHeight = jumpHeight;

    // update jump count
    updateResource(WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::ID,
                   ResponseOptions::Empty, mJumpCounter);

    // update jump height
    updateResource(WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::ID,
                   ResponseOptions::Empty, mLastJumpHeight);
}

// This callback is called when the acceleration resource notifies us of new data
void JumpCounterService::onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                                  const whiteboard::ParameterList& parameters)
{
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
            whiteboard::FloatVector3D accValue = arrayData[i];
            float accelerationSq = accValue.mX * accValue.mX +
                                   accValue.mY * accValue.mY +
                                   accValue.mZ * accValue.mZ;

            // Use ~0.1G as 0-G limit. 1.0 m/s2; squared = 1.0 m^2/s^4
            const float LIMIT_OF_0G_SQUARED = 5.0f; // Unit: m^2/s^4 !
            const size_t MIN_JUMP_LENGTH_SAMPLES = 5;

            if (accelerationSq < LIMIT_OF_0G_SQUARED)
            {
                // We're in 0 G. Increment counter
                mSamplesSince0GStart++;
            }
            else
            {
                // Not in 0G

                // See if we have been long enough to be a jump
                if (mSamplesSince0GStart >= MIN_JUMP_LENGTH_SAMPLES)
                {
                    // Mark new jump!
                    jumpFound(mSamplesSince0GStart);
                }
                // reset counter
                mSamplesSince0GStart = 0;
            }
        }
    }
    break;
    }
}

void JumpCounterService::onSubscribe(const whiteboard::Request& request,
                                     const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::ID:
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::ID:
    {
        // Someone subscribed to our service. If no subscribers before, start running
        if (mSubscriptionCount == 0)
        {
            mSubscriptionCount++;
            whiteboard::RequestId remoteRequestId;
            whiteboard::Result result = startRunning(remoteRequestId);

            if (!whiteboard::RETURN_OK(result))
            {
                return returnResult(request, result);
            }

            bool queueResult = mOngoingRequests.put(remoteRequestId, request);
            WB_ASSERT(queueResult);
        }

        break;
    }
    default:
        ASSERT(0); // Should not happen
    }
}

void JumpCounterService::onUnsubscribe(const whiteboard::Request& request,
                                       const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::ID:
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::ID:
        stopRunning();
        mSubscriptionCount--;
        returnResult(request, wb::HTTP_CODE_OK);
        break;

    default:
        returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
        break;
    }
}
