// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include <whiteboard/LaunchableModule.h>
#include "common/services/InfoProvider.hpp"
#include "common/services/SystemProvider.hpp"
#include "common/services/ShutdownProvider.hpp"
#include "common/services/SystemModeProvider.hpp"
#include "common/services/TimeProvider.hpp"

#include "hal/systemevent/SystemEventProvider.hpp"

namespace nea
{

/**
*	Class to instantiate services / providers in 'application' context.
*	This class implements launchable module interface (LaunchableModule).
*/
class SystemServices FINAL : private whiteboard::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;

    /** Constructor
        @param execCtxId ID of the execution context in which to start up.
    */
    SystemServices(wb::ExecutionContextId execCtxId);
    /** Destructor */
    ~SystemServices();

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
    SystemProvider mSystemProvider;
    SystemModeProvider mSystemModeProvider;
    ShutdownProvider mShutdownProvider;
    InfoProvider mInfoProvider;
    TimeProvider mTimeProvider;
    hal::SystemEventProvider mSystemEventProvider;

    // @TODO: move to debug startup
//    hal::MemAccessService mMemAccessService;
};

} // namespace nea
