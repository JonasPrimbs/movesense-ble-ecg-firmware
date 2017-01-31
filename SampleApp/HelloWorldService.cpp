#include "movesense.h"


#include "HelloWorldService.h"
#include "common/core/debug.h"

#include "whiteboard/builtinTypes/UnknownStructure.h"
#include "app-resources/resources.h"
#include <float.h>
#include <math.h>

const size_t HELLO_WORLD_PERIOD_MS  = 5000;


const char* const HelloWorldService::LAUNCHABLE_NAME = "HelloSvc";

static const whiteboard::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::EXAMPLE_HELLOWORLD::EXECUTION_CONTEXT;

    
static const whiteboard::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::EXAMPLE_HELLOWORLD::LID,
};


HelloWorldService::HelloWorldService()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId)
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
    
    if (mModuleState != WB_RES::ModuleStateValues::STARTED) {
        return returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);
    }

    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::EXAMPLE_HELLOWORLD::ID:
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

void HelloWorldService::onSubscribe(const whiteboard::Request& request,
                                  const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
        case WB_RES::LOCAL::EXAMPLE_HELLOWORLD::ID:
        {
            // Someone subscribed to our service. Start timer that greets once every 5 seconds
            if (mTimer == whiteboard::ID_INVALID_TIMER)
            {
                mTimer = reinterpret_cast<ResourceProvider*>(this)->startTimer(HELLO_WORLD_PERIOD_MS, true);
            }

            break;
        }   
        default:
            // Return error
            return returnResult(request, whiteboard::HTTP_CODE_NOT_FOUND);
    }
}

void HelloWorldService::onUnsubscribe(const whiteboard::Request& request,
                                    const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
        case WB_RES::LOCAL::EXAMPLE_HELLOWORLD::ID:

            if (mTimer != whiteboard::ID_INVALID_TIMER)
            {
                reinterpret_cast<ResourceProvider*>(this)->stopTimer(mTimer);
                mTimer == whiteboard::ID_INVALID_TIMER;
            }
            break;
            
        default:
            returnResult(request, wb::HTTP_CODE_NOT_FOUND);
    }
}
