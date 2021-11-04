// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include <whiteboard/LaunchableModule.h>

#include "common/compiler/genType.h"
namespace nea
{

/**
*	Class to instantiate services / providers in 'application' context.
*	This class implements launchable module interface (LaunchableModule).
*/
class StandardServices FINAL : private whiteboard::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;

    /** Constructor
        @param execCtxId ID of the execution context in which to start up.
    */
    explicit StandardServices(wb::ExecutionContextId execCtxId);

    /** Destructor */
    ~StandardServices();

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
};

} // namespace nea
