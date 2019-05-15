#include "movesense.h"

#include "HelloWorldService.h"
#include "common/core/debug.h"

#include "app-resources/resources.h"
#include "DebugLogger.hpp"

const size_t HELLO_WORLD_PERIOD_MS = 1000;

const char* const HelloWorldService::LAUNCHABLE_NAME = "HelloSvc";

static const wb::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::SAMPLE_HELLOWORLD::LID,
};

HelloWorldService::HelloWorldService():
    ResourceProvider(WBDEBUG_NAME(__FUNCTION__), WB_RES::LOCAL::SAMPLE_HELLOWORLD::EXECUTION_CONTEXT),
    LaunchableModule(LAUNCHABLE_NAME, WB_RES::LOCAL::SAMPLE_HELLOWORLD::EXECUTION_CONTEXT),
    mTimer(wb::ID_INVALID_TIMER),
    mCounter(0)
{
}

HelloWorldService::~HelloWorldService()
{
}

bool HelloWorldService::initModule()
{
    if (registerProviderResources(sProviderResources) != wb::HTTP_CODE_OK)
    {
        return false;
    }

    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void HelloWorldService::deinitModule()
{
    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool HelloWorldService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    return true;
}

void HelloWorldService::stopModule()
{
    stopTimer(mTimer);
    mTimer = wb::ID_INVALID_TIMER;
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void HelloWorldService::onGetRequest(const wb::Request& request,
                                     const wb::ParameterList& parameters)
{
    DEBUGLOG("HelloWorldService::onGetRequest() called.");

    if (mModuleState != WB_RES::ModuleStateValues::STARTED)
    {
        returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);
        return;
    }

    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_HELLOWORLD::LID:
    {
        WB_RES::HelloWorldValue hello;
        hello.greeting = "Hello World!";
        DebugLogger::info(hello.greeting);
        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, hello);
        break;
    }

    default:
        ASSERT(0); // would be a system error if we got here, trust the system and save rom.
    }
}

void HelloWorldService::onTimer(wb::TimerId timerId)
{
    if (isResourceSubscribed(WB_RES::LOCAL::SAMPLE_HELLOWORLD::ID) != wb::HTTP_CODE_OK)
    {
        DEBUGLOG("Stop timer.");
        stopTimer(mTimer);
        mTimer = wb::ID_INVALID_TIMER;
        return;
    }

    // Create new Greeting with incrementing counter
    char buf[32];
    mCounter++;
    snprintf(buf, 32, "Hello World #%u!", mCounter);

    WB_RES::HelloWorldValue hello;
    hello.greeting = buf;

    DebugLogger::info(buf);
    // and update the resources/resources
    updateResource(WB_RES::LOCAL::SAMPLE_HELLOWORLD(), ResponseOptions::Empty, hello);
}

void HelloWorldService::onSubscribe(const wb::Request& request,
                                    const wb::ParameterList& parameters)
{
    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_HELLOWORLD::LID:
    {
        // Someone subscribed to our service. Start timer that greets once every 5 seconds
        if (mTimer == wb::ID_INVALID_TIMER)
        {
            mTimer = startTimer(HELLO_WORLD_PERIOD_MS, true);
        }

        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty);
        break;
    }
    default:
        ASSERT(0); // would be a system error if we got here, trust the system and save rom.
    }
}

void HelloWorldService::onUnsubscribe(const wb::Request& request,
                                      const wb::ParameterList& parameters)
{
    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_HELLOWORLD::LID:
        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty);
        break;

    default:
        ASSERT(0); // would be a system error if we got here, trust the system and save rom.
    }
}
