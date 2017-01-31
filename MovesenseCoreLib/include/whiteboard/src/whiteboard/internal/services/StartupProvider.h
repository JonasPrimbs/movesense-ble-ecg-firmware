// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/ResourceProvider.h"
#include "whiteboard/LaunchableModule.h"

namespace whiteboard
{

// Forward declarations
class Dpc;

namespace services
{

/** Launchable Module Startup Provider */
class StartupProvider FINAL : private ResourceProvider, public LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    /* Define the maximum number of elements in LaunchableList */
    static const uint8 MAX_NUMBER_OF_MODULES = 32;

    /** Initializes a new instance of StartupProvider
    */
    StartupProvider();

    /** Deinitializes StartupProvider instance
    *
    *  All launchable modules must be deinitialized and unregistered before freeing StartupProvider
    */
    ~StartupProvider();

private:
    /** LaunchableModule will use these functions for auto registration */
    friend class whiteboard::LaunchableModule;

    /**
    *  Adds the given launchable module to the register.
    *
    *  @param rLaunchable launchable module
    */
    static void add(LaunchableModule& rLaunchable);

    /**
    *  Removes the given launchable module from the register.
    *
    *  @param rLaunchable launchable module
    */
    static void remove(LaunchableModule& rLaunchable);

    /**
    *  Finds the given launchable module from the register.
    *
    *  @param name pointer to launchable module name string
    *  @return pointer to the launchable module, of NULL if not registered
    */
    LaunchableModule* findModule(const char* name) const;

private:
    /** Builds command to move launchable from current state to target state
    *
    * @param rLaunchable Launchable module
    * @param cmd Module command
    * @return Command mask to execute
    */
    static uint8 makeCommand(LaunchableModule& rLaunchable, WB_RES::ModuleCommand cmd);

    /** Executes the command to move launchable to a different state
     *
     * @param rLaunchable Launchable module
     * @param commandMask Bitmask of commands to execute
     * @return A value indicating whether the state was successfully changed
     */
    bool executeCommand(LaunchableModule& rLaunchable, uint8 commandMask);

private:
    /**
    *  LaunchableModule interface methods
    */
    bool initModule() OVERRIDE;
    void deinitModule() OVERRIDE;
    bool startModule() OVERRIDE;
    void stopModule() OVERRIDE;

    /**
    *  GET request handler is used to query the launchable module name and status.
    *
    *  @param rRequest Request information
    *  @param rParameters List of parameters for the request
    */
    void onGetRequest(
        const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

    /**
    *  PUT request handler is used to command the launchable module.
    *
    *  @param rRequest Request information
    *  @param rParameters List of parameters for the request
    */
    void onPutRequest(
        const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

private:
    /** First registered LaunchableModule */
    static LaunchableModule* spFirst;

    /** DPC for switching execution to another context */
    Dpc* mpCallModuleMethodDpc;
};

} // namespace services
} // namespace whiteboard
