// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/integration/port.h"
#include "whiteboard/Identifiers.h"
#include "wb-resources/resources.h"

namespace whiteboard
{

// Forward declarations
namespace services
{
class StartupProvider;
}

/** Base class for LaunchableModules. A launchable module
 * is a software component which running state can be controlled
 * through StatupProvider. Typically launchable module initializes
 * providers and clients of some feature, but the interface can
 * be used even without any provider or client.
 */
class WB_API LaunchableModule
{
private:
    /** Prevent use of default constructor */
    LaunchableModule() DELETED;
    /** Prevent use of copy constructor */
    LaunchableModule(const LaunchableModule&) DELETED;
    /** Prevent use of assignment operator */
    LaunchableModule& operator=(const LaunchableModule&) DELETED;

public:
    /* Define the maximum length of launchable module name */
    static const size_t MODULE_NAME_MAX_LEN = 32;

    /** Constructor
     *
     * @param moduleName Name of the module
     * @param executionContextId Execution context of the launchable module
     */
    LaunchableModule(const char* moduleName, ExecutionContextId executionContextId);

    /** Destructor */
    virtual ~LaunchableModule();

    /**
    Startup service provider will call this method to trigger the
    initialization of the module (hardware configuration etc.).
    @return true if initialization was successful, false otherwise.
    */
    virtual bool initModule() = 0;
    
    /**
    Startup service provider will call this method to trigger the
    de-initialization of the module.
    */
    virtual void deinitModule() = 0;
    
    /**
    Startup service provider will call this method to indicate the
    operation of the module can be started.
    @return true if startup was successful, false otherwise.
    */
    virtual bool startModule() = 0;
    
    /**
    Startup service provider will call this method to indicate the
    operation of the module is not allowed.
    */
    virtual void stopModule() = 0;

    /**
    Get the name of the module.
    @return Pointer to the module name.
    */
    inline const char* getLaunchableName() const { return mpModuleName; }

    /**
    Get current state of the module
    @return State of the module
    */
    inline WB_RES::ModuleState getModuleState() const { return static_cast<WB_RES::ModuleState::Type>(mModuleState); }

    /**
    Get the execution context of the launchable module.
    @return The execution contxet ID.
    */
    inline ExecutionContextId getExecutionContextId() const { return mExecutionContextId; }

protected:
    /** Current module state */
    uint8 mModuleState; // In order to avoid exposing generated code out of the dll, stored as uint8

private:
    /// name of the launchable module, will be used as an identifier for starting/stopping
    const char* mpModuleName;

private:
    /** StartupProvider has direct access to these members */
    friend class services::StartupProvider;

    /// Execution context of the launchable module
    const ExecutionContextId mExecutionContextId;

    /// Next launchable module in StartupProvider's registration list
    LaunchableModule* mpNext;
};

} // namespace whiteboard
