#include "movesense.h"

#include "HelloWorldService.h"
#include "common/core/debug.h"

#include "app-resources/resources.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"
#include <float.h>
#include <math.h>

const size_t HELLO_WORLD_PERIOD_MS = 1000;

const char* const HelloWorldService::LAUNCHABLE_NAME = "HelloSvc";

static const whiteboard::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::SAMPLE_HELLOWORLD::LID,
};

HelloWorldService::HelloWorldService()
    : ResourceProvider(WBDEBUG_NAME(__FUNCTION__), WB_RES::LOCAL::SAMPLE_HELLOWORLD::EXECUTION_CONTEXT),
      LaunchableModule(LAUNCHABLE_NAME, WB_RES::LOCAL::SAMPLE_HELLOWORLD::EXECUTION_CONTEXT)
{
    mCounter = 0;
    mTimer = whiteboard::ID_INVALID_TIMER;
}

HelloWorldService::~HelloWorldService()
{
}

bool HelloWorldService::initModule()
{
    if (registerProviderResources(sProviderResources) != whiteboard::HTTP_CODE_OK)
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

/** @see whiteboard::ILaunchableModule::startModule */
bool HelloWorldService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    return true;
}

void HelloWorldService::onGetRequest(const whiteboard::Request& request,
                                     const whiteboard::ParameterList& parameters)
{
    DEBUGLOG("HelloWorldService::onGetRequest() called.");

    if (mModuleState != WB_RES::ModuleStateValues::STARTED)
    {
        return returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);
    }

    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_HELLOWORLD::ID:
    {
        WB_RES::HelloWorldValue hello;
        hello.greeting = "Hello World!";
        return returnResult(request, whiteboard::HTTP_CODE_OK, ResponseOptions::Empty, hello);
    }

    break;

    default:
        // Return error
        return returnResult(request, whiteboard::HTTP_CODE_NOT_FOUND);
    }
}

void HelloWorldService::onTimer(whiteboard::TimerId timerId)
{
    if (isResourceSubscribed(WB_RES::LOCAL::SAMPLE_HELLOWORLD::ID) != wb::HTTP_CODE_OK)
    {
        DEBUGLOG("Stop timer.");
        stopTimer(mTimer);
        mTimer = whiteboard::ID_INVALID_TIMER;
        return;
    }

    if (timerId != mTimer)
    {
        return;
    }

    // Create new Greeting with incrementing counter
    char buf[32];
    mCounter++;
    snprintf(buf, 32, "Hello World #%u!", mCounter);

    WB_RES::HelloWorldValue hello;
    hello.greeting = buf;

    // and update the resources/resources
    updateResource(WB_RES::LOCAL::SAMPLE_HELLOWORLD(),
                   ResponseOptions::Empty, hello);
}

void HelloWorldService::onSubscribe(const whiteboard::Request& request,
                                    const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_HELLOWORLD::ID:
    {
        // Someone subscribed to our service. Start timer that greets once every 5 seconds
        if (mTimer == whiteboard::ID_INVALID_TIMER)
        {
            mTimer = startTimer(HELLO_WORLD_PERIOD_MS, true);
        }

        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty);
        break;
    }
    default:
        // Return error
        returnResult(request, whiteboard::HTTP_CODE_NOT_FOUND);
    }
}

void HelloWorldService::onUnsubscribe(const whiteboard::Request& request,
                                      const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_HELLOWORLD::ID:
        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty);
        break;

    default:
        returnResult(request, wb::HTTP_CODE_NOT_FOUND);
    }
}
