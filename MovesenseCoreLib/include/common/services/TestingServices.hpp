// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include <whiteboard/LaunchableModule.h>
#include "common/services/DebugProvider.hpp"
#include "common/compiler/genType.h"

namespace nea
{

/**
*	Class to instantiate testing services and init in correct execution context.
*	This class implements launchable module interface (LaunchableModule).
*/
class TestingServices FINAL : private whiteboard::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;

    /** Constructor
        @param execCtxId ID of the execution context in which to start up.
    */
    explicit TestingServices(wb::ExecutionContextId execCtxId);

    /** Destructor */
    ~TestingServices();

private:
    /// @see whiteboard::ILaunchableModule::initModule
    virtual bool initModule() OVERRIDE;
    /// @see whiteboard::ILaunchableModule::deinitModule
    virtual void deinitModule() OVERRIDE;
    /// @see whiteboard::ILaunchableModule::startModule
    virtual bool startModule() OVERRIDE;
    /// @see whiteboard::ILaunchableModule::stopModule
    virtual void stopModule() OVERRIDE;

private:
    nea::DebugProvider mDebugProvider;
};

} // namespace nea
