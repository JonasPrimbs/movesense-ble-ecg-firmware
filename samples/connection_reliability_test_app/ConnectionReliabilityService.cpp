#include "ConnectionReliabilityService.h"
#include "app-resources/resources.h"
#include "common/core/debug.h"

#define PACKET_INTERVAL_MS 100

const char* const ConnectionReliabilityService::LAUNCHABLE_NAME = "ConnRel";

static const wb::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::EXECUTION_CONTEXT;

static const wb::LocalResourceId sProviderResources[] =
{
    WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::LID,
    WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTLARGEPKG::LID,
};

ConnectionReliabilityService::ConnectionReliabilityService():
    ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
    LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId),
    mCounterTimer(wb::ID_INVALID_TIMER),
    mSubCounter(0),
    mCounter(0)
{
}

ConnectionReliabilityService::~ConnectionReliabilityService()
{
}

bool ConnectionReliabilityService::initModule()
{
    if (registerProviderResources(sProviderResources) != wb::HTTP_CODE_OK)
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

bool ConnectionReliabilityService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void ConnectionReliabilityService::stopModule()
{
    stopRunning();
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void ConnectionReliabilityService::startRunning()
{
    if (mCounterTimer != wb::ID_INVALID_TIMER)
        stopRunning();

    mCounter = 0;
    mCounterTimer = startTimer(PACKET_INTERVAL_MS, true);
}

void ConnectionReliabilityService::stopRunning()
{
    stopTimer(mCounterTimer);
    mCounterTimer = wb::ID_INVALID_TIMER;
}

void ConnectionReliabilityService::onTimer(wb::TimerId /*timerId*/)
{
    mCounter++;
    if (isResourceSubscribed(WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::ID))
    {
        updateResource(WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG(), ResponseOptions::Empty, mCounter);
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
        updateResource(WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTLARGEPKG(), ResponseOptions::Empty, largePkg);
    }
}

void ConnectionReliabilityService::onSubscribe(const wb::Request& request,
                                               const wb::ParameterList& parameters)
{
    DEBUGLOG("ConnectionReliabilityService::onSubscribe()");

    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::LID:
    case WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTLARGEPKG::LID:
    {
        bool bHadSubsriptions = mSubCounter > 0;
        mSubCounter++;
        
        DEBUGLOG("bHadSubsriptions: %d", bHadSubsriptions);
        // Someone subscribed to our service. If no subscribers before, start running
        if (!bHadSubsriptions)
        {
            startRunning();
        }
        returnResult(request, wb::HTTP_CODE_OK);
        break;
    }
    default:
        ASSERT(0); // would be a system error if we reached this, trust the system and safe rom
    }
}

void ConnectionReliabilityService::onUnsubscribe(const wb::Request& request,
                                                 const wb::ParameterList& parameters)
{
    DEBUGLOG("ConnectionReliabilityService::onUnsubscribe()");

    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTSMALLPKG::LID:
    case WB_RES::LOCAL::TEST_CONNECTIONRELIABILITY_INCREMENTLARGEPKG::LID:
    {
        mSubCounter--;
        bool bHasSubsriptions = mSubCounter > 0;

        // Someone subscribed to our service. If no subscribers anymore, stop running
        DEBUGLOG("bHasSubsriptions: %d", bHasSubsriptions);
        if (!bHasSubsriptions)
        {
            stopRunning();
        }
        returnResult(request, wb::HTTP_CODE_OK);
        break;
    }
    default:
        ASSERT(0); // would be a system error if we reached this, trust the system and safe rom
    }
}

void ConnectionReliabilityService::onRemoteWhiteboardDisconnected(wb::WhiteboardId whiteboardId)
{
    DEBUGLOG("ConnectionReliabilityService::onRemoteWhiteboardDisconnected()");
    stopRunning();
}

void ConnectionReliabilityService::onClientUnavailable(wb::ClientId clientId)
{
    DEBUGLOG("ConnectionReliabilityService::onClientUnavailable()");
    stopRunning();
}
