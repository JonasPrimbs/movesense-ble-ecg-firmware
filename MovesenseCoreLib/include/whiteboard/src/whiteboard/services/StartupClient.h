#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/ResourceClient.h"
#include "wb-resources/resources.h"
#include "whiteboard/DpcFunctor.h"

namespace whiteboard
{
namespace services
{

/** Helper interface for starting launchable modules synchronously */
class StartupClient FINAL : private ResourceClient
{
public:
    /** Initializes a new instance of StartupClient
    *
    * @param executionContextId Execution context of the client
    */
    WB_API StartupClient(ExecutionContextId executionContextId);

    /** Destructor */
    WB_API ~StartupClient();

    /**
    * Commands the launchable module of the given name
    * @param pModuleName pointer to the NULL terminated launchable module name
    * @param command for the launchable module (see StartupProvider::Command)
    */
    WB_API void executeCommand(const char* pModuleName, WB_RES::ModuleCommand command);

private:
    void doPut();

    /** @see ResourceClient::onPutResult */
    void onPutResult(RequestId /*requestId*/, ResourceId /*resourceId*/, Result resultCode, const Value& /*rResultData*/) OVERRIDE;

private:
    /** Name of the module */
    const char* mpModuleName;

    /** Command to execute */
    WB_RES::ModuleCommand mCommand;

    /** Semaphore that is signaled when put request has completed */
    WbSemaphoreHandle mSemaPutCompleted;
};

} // namespace services
} // namespace whiteboard
