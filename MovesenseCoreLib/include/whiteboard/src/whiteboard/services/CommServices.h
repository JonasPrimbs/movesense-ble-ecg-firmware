// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/LaunchableModule.h"

namespace whiteboard
{
namespace services
{

/**
*	Class to instantiate whiteboard comm related services
*	This class implements launchable module interface (LaunchableModule).
*/
class CommServices FINAL : private LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    /** Constructor
    */
    CommServices();

    /** Destructor */
    ~CommServices();

private:
    /// @see whiteboard::ILaunchableModule::initModule
    virtual bool initModule() OVERRIDE;
    /// @see whiteboard::ILaunchableModule::deinitModule
    virtual void deinitModule() OVERRIDE;
    /// @see whiteboard::ILaunchableModule::startModule
    virtual bool startModule() OVERRIDE;
    /// @see whiteboard::ILaunchableModule::stopModule 
    virtual void stopModule() OVERRIDE;
};

}   // namespace services
}   // namespace whiteboard
