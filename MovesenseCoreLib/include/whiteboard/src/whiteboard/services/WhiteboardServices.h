// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/LaunchableModule.h"
#include "whiteboard/services/StartupClient.h"

namespace whiteboard
{
namespace services
{

/**
*   Sends command to all launchable modules listed in the given array.
*   If start command if given, init command is sent first by the launcher itself.
*   @param ctx Execution context in which the StartupClient should run (mostly irrelevant).
*   @param modules NULL terminated array of launchable module names.
*   @param cmd Launchable command to be sent.
*/
WB_API void startupCommand(ExecutionContextId ctx, const char* const modules[], WB_RES::ModuleCommand cmd);

/**
*   Sends command to all launchable modules listed in the given array.
*   If start command if given, init command is sent first by the launcher itself.
*   StartupClient is run in the current execution context.
*   @param modules NULL terminated array of launchable module names.
*   @param cmd Launchable command to be sent.
*/
WB_API void startupCommand(const char* const modules[], WB_RES::ModuleCommand cmd);

// Forward declare internal services not to leak internal headers
class StartupProvider;
class InternalServices;

/**
*   Instantiation wrapper for whiteboard services. This is the first module to start from the launchable modules as
*   StartupProvider is constructed first.
*/
class WB_API WhiteboardServicesInstance FINAL
{
public:

    // This is the name of the launchable module to start whiteboard internal services
    static const char* LAUNCHABLE_NAME;

    /** Default constructor */
    WhiteboardServicesInstance();

    /** Destructor */
    ~WhiteboardServicesInstance();

private:
    WhiteboardServicesInstance(const WhiteboardServicesInstance&) DELETED;

    /** StartupProvider instance */
    StartupProvider* mpStartupProvider;

    /** Internal services */
    InternalServices* mpServices;
};


} // namespace services
} // namespace whiteboard
