#include "ConnectionReliabilityService.h"
#include "app-resources/resources.h"
#include "common/core/debug.h"

#include <float.h>
#include <math.h>

#define PACKET_INTERVAL_MS 100

const char* const ConnectionReliabilityService::LAUNCHABLE_NAME = "ConnRel";

static const whiteboard::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::EXECUTION_CONTEXT;

static const whiteboard::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::LID,
    WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTLARGEPKG::LID,
};

ConnectionReliabilityService::ConnectionReliabilityService()
    : ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId),
      mCounterTimer(whiteboard::ID_INVALID_TIMER)
{
    mCounter = 0;
    mSubCounter = 0;
}

ConnectionReliabilityService::~ConnectionReliabilityService()
{
}

bool ConnectionReliabilityService::initModule()
{
    if (registerProviderResources(sProviderResources) != whiteboard::HTTP_CODE_OK)
    {
        return false;
    }

    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void ConnectionReliabilityService::deinitModule()
{
    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

/** @see whiteboard::ILaunchableModule::startModule */
bool ConnectionReliabilityService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void ConnectionReliabilityService::startRunning()
{
    if (mCounterTimer != whiteboard::ID_INVALID_TIMER)
        stopRunning();

    mCounter = 0;
    mCounterTimer = startTimer(PACKET_INTERVAL_MS, true);
}

void ConnectionReliabilityService::stopRunning()
{
    stopTimer(mCounterTimer);
    mCounterTimer = whiteboard::ID_INVALID_TIMER;
}

void ConnectionReliabilityService::onTimer(wb::TimerId timerId)
{
    if (mCounterTimer == timerId)
    {
        mCounter++;
        if (isResourceSubscribed(WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::ID))
        {
            updateResource(WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG(), ResourceProvider::ResponseOptions::Empty, mCounter);
        }
        
        if (isResourceSubscribed(WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTLARGEPKG::ID))
        {
            WB_RES::LargePkg largePkg;
            largePkg.counter1 = mCounter;
            largePkg.counter2 = mCounter;
            largePkg.counter3 = mCounter;
            largePkg.counter4 = mCounter;
            largePkg.counter5 = mCounter;
            largePkg.counter6 = mCounter;
            largePkg.counter7 = mCounter;
            largePkg.counter8 = mCounter;
            largePkg.counter9 = mCounter;
            largePkg.counter10 = mCounter;
            largePkg.counter11 = mCounter;
            largePkg.counter12 = mCounter;
            largePkg.counter13 = mCounter;
            largePkg.counter14 = mCounter;
            largePkg.counter15 = mCounter;
            largePkg.counter16 = mCounter;
            updateResource(WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTLARGEPKG(), ResourceProvider::ResponseOptions::Empty, largePkg);
        }
    }
}

void ConnectionReliabilityService::onSubscribe(const whiteboard::Request& request,
                                     const whiteboard::ParameterList& parameters)
{
    DEBUGLOG("ConnectionReliabilityService::onSubscribe()");

    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::ID:
    case WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTLARGEPKG::ID:
    {
        bool bHadSubsriptions = mSubCounter > 0;
        mSubCounter++;
        
        DEBUGLOG("bHadSubsriptions: %d", bHadSubsriptions);
        // Someone subscribed to our service. If no subscribers before, start running
        if (!bHadSubsriptions)
        {
            startRunning();
        }
        return returnResult(request, whiteboard::HTTP_CODE_OK);

        break;
    }
    default:
        ASSERT(0); // Should not happen
    }
}

void ConnectionReliabilityService::onUnsubscribe(const whiteboard::Request& request,
                                       const whiteboard::ParameterList& parameters)
{
    DEBUGLOG("ConnectionReliabilityService::onUnsubscribe()");

    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::ID:
    case WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTLARGEPKG::ID:
    {
        mSubCounter--;
        bool bHasSubsriptions = mSubCounter > 0;

        // Someone subscribed to our service. If no subscribers anymore, stop running
        DEBUGLOG("bHasSubsriptions: %d", bHasSubsriptions);
        if (!bHasSubsriptions)
        {
            stopRunning();
        }
        return returnResult(request, whiteboard::HTTP_CODE_OK);

        break;
    }
    default:
        returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
        break;
    }
}

void ConnectionReliabilityService::onRemoteWhiteboardDisconnected(whiteboard::WhiteboardId whiteboardId)
{
    DEBUGLOG("ConnectionReliabilityService::onRemoteWhiteboardDisconnected()");
    stopRunning();
}

void ConnectionReliabilityService::onClientUnavailable(whiteboard::ClientId clientId)
{
    DEBUGLOG("ConnectionReliabilityService::onClientUnavailable()");
    stopRunning();
}