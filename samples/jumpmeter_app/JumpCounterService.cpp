#include "JumpCounterService.h"
#include "app-resources/resources.h"
#include "common/core/debug.h"
#include "common/core/dbgassert.h"
#include "meas_acc/resources.h"

const char* const JumpCounterService::LAUNCHABLE_NAME = "JumpC";

static const wb::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::EXECUTION_CONTEXT;

static const wb::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::LID,
    WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::LID,
};

JumpCounterService::JumpCounterService():
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
    ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
    LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId),
    mMeasAccResourceId(wb::ID_INVALID_RESOURCE),
    mOngoingRequests(),
    isRunning(false),
    mLastJumpHeight(0.0f),
    mJumpCounter(0),
    mSamplesSince0GStart(0)
{
}

JumpCounterService::~JumpCounterService()
{
}

bool JumpCounterService::initModule()
{
    if (registerProviderResources(sProviderResources) != wb::HTTP_CODE_OK)
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

bool JumpCounterService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void JumpCounterService::stopModule()
{
    stopRunning();
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void JumpCounterService::onGetRequest(const wb::Request& request,
                                      const wb::ParameterList& parameters)
{
    DEBUGLOG("JumpCounterService::onGetRequest() called.");

    if (mModuleState != WB_RES::ModuleStateValues::STARTED)
    {
        returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);
        return;
    }

    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::LID:
    {
        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, mJumpCounter);
        break;
    }

    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::LID:
    {
        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, mLastJumpHeight);
        break;
    }

    default:
        ASSERT(0); // would be a system error if we got here, trust the system and save rom.
    }
}

void JumpCounterService::onUnsubscribeResult(wb::RequestId requestId,
                                             wb::ResourceId resourceId,
                                             wb::Result resultCode,
                                             const wb::Value& rResultData)
{
    DEBUGLOG("JumpCounterService::onUnsubscribeResult() called.");
}

void JumpCounterService::onSubscribeResult(wb::RequestId requestId,
                                           wb::ResourceId resourceId,
                                           wb::Result resultCode,
                                           const wb::Value& rResultData)
{
    DEBUGLOG("JumpCounterService::onSubscribeResult() called. resourceId: %u, result: %d", resourceId.localResourceId, (uint32_t)resultCode);

    wb::Request relatedIncomingRequest;
    bool relatedRequestFound = mOngoingRequests.get(requestId, relatedIncomingRequest);

    if (relatedRequestFound)
    {
        returnResult(relatedIncomingRequest, wb::HTTP_CODE_OK);
    }
}

wb::Result JumpCounterService::startRunning(wb::RequestId& remoteRequestId)
{
    DEBUGLOG("JumpCounterService::startRunning()");

    // Reset 0G detection count
    mSamplesSince0GStart = 0;

    // Subscribe to LinearAcceleration resource (updates at ACC_SAMPLERATE Hz), when subscribe is done, we get callback
    wb::Result result = getResource("Meas/Acc/104", mMeasAccResourceId);
    if (!wb::RETURN_OKC(result))
    {
        return result;
    }
    result = asyncSubscribe(mMeasAccResourceId, AsyncRequestOptions(&remoteRequestId, 0, true));

    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncSubscribe threw error: %u", result);
        releaseResource(mMeasAccResourceId);
        return result;
    }
    isRunning = true;
    return wb::HTTP_CODE_ACCEPTED;
}

wb::Result JumpCounterService::stopRunning()
{
    if (!isRunning)
    {
        return wb::HTTP_CODE_ACCEPTED;
    }

    DEBUGLOG("JumpCounterService::stopRunning()");
    if (isResourceSubscribed(WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::ID) == wb::HTTP_CODE_OK ||
        isResourceSubscribed(WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::ID) == wb::HTTP_CODE_OK)
    {
        DEBUGLOG("JumpCounterService::stopRunning() skipping. Subscribers still exist.");
        return wb::HTTP_CODE_ACCEPTED;
    }

    // Unsubscribe the LinearAcceleration resource, when unsubscribe is done, we get callback
    wb::Result result = asyncUnsubscribe(mMeasAccResourceId, NULL);
    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncUnsubscribe threw error: %u", result);
    }
    isRunning = false;
    releaseResource(mMeasAccResourceId);

    return wb::HTTP_CODE_ACCEPTED;
}

void JumpCounterService::jumpFound(size_t nJumpSamples)
{
    mJumpCounter++;

    // Calc jump length
    // TODO: Add support for flexible samplerate
    const float ONE_PER_SAMPLE_RATE = 1.0f / (float)104;
    const float EARTH_G = 9.81f;

    float jumpDuration = (float)nJumpSamples * ONE_PER_SAMPLE_RATE;
    // s = 1/2 * a * t^2, a= 1G, and t is half time of 0-G (we go up and down in jump)
    float t = jumpDuration * 0.5f;
    float jumpHeight = 0.5f * (t * t) * EARTH_G;

    // Save jump height
    mLastJumpHeight = jumpHeight;

    // update jump count
    updateResource(WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT(), ResponseOptions::Empty, mJumpCounter);

    // update jump height
    updateResource(WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT(), ResponseOptions::Empty, mLastJumpHeight);
}

// This callback is called when the acceleration resource notifies us of new data
void JumpCounterService::onNotify(wb::ResourceId resourceId,
                                  const wb::Value& value,
                                  const wb::ParameterList& parameters)
{
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

        for (size_t i = 0; i < arrayData.size(); i++)
        {
            wb::FloatVector3D accValue = arrayData[i];
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
        break;
    }

    default:
        ASSERT(0); // would be a system error if we got here, trust the system and save rom.
    }
}

void JumpCounterService::onSubscribe(const wb::Request& request,
                                     const wb::ParameterList& parameters)
{
    DEBUGLOG("JumpCounterService::onSubscribe()");

    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::LID:
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::LID:
    {
        // Someone subscribed to our service. If no subscribers before, start running
        if (!isRunning)
        {
            wb::RequestId remoteRequestId;
            wb::Result result = startRunning(remoteRequestId);

            if (result == wb::HTTP_CODE_ACCEPTED)
            {
                bool queueResult = mOngoingRequests.put(remoteRequestId, request);
                (void)queueResult;
                WB_ASSERT(queueResult);
            }
            else
            {
                returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
            }
        }
        else
        {
            returnResult(request, wb::HTTP_CODE_OK);
        }
        break;
    }
    default:
        ASSERT(0); // would be a system error if we got here, trust the system and save rom.
    }
}

void JumpCounterService::onUnsubscribe(const wb::Request& request,
                                       const wb::ParameterList& parameters)
{
    DEBUGLOG("JumpCounterService::onUnsubscribe()");

    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_JUMPCOUNT::LID:
    case WB_RES::LOCAL::SAMPLE_JUMPCOUNTER_LASTJUMPHEIGHT::LID:
        returnResult(request, stopRunning());
        break;

    default:
        ASSERT(0); // would be a system error if we got here, trust the system and save rom.
    }
}

void JumpCounterService::onRemoteWhiteboardDisconnected(wb::WhiteboardId whiteboardId)
{
    DEBUGLOG("JumpCounterService::onRemoteWhiteboardDisconnected()");
    stopRunning();
}

void JumpCounterService::onClientUnavailable(wb::ClientId clientId)
{
    DEBUGLOG("JumpCounterService::onClientUnavailable()");
    stopRunning();
}
