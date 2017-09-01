// Copyright (c) Suunto Oy 2016. All rights reserved.
#pragma once

#include <whiteboard/LaunchableModule.h>
#ifndef USE_NOFS
#include "filesystems/wbintegration/FilesystemProvider.hpp"
#endif
// XXXX TODO: Refactor this kind of stuff away! USE__EXTFLASHIMAGE etc...
#ifdef APP_BOOTLOADER
#include "hal/memory/ExtFlashImage.hpp"
#endif
#ifdef APP_SS2_APPLICATION
#include "hal/memory/ExtFlashImage.hpp"
#endif

namespace nea
{

/**
*	Class to instantiate services / providers in 'primary services' context.
*	This class implements launchable module interface (LaunchableModule).
*/
class LowPriorityServices FINAL : private whiteboard::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    /** Constructor
    @param execCtxId ID of the execution context in which to start up.
    */
    LowPriorityServices(wb::ExecutionContextId execCtxId);

    /** Destructor */
    ~LowPriorityServices();

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
#ifndef USE_NOFS
    FilesystemProvider mFsProvider;
#endif // #ifndef USE_NOFS
#ifdef APP_BOOTLOADER
    hal::ExtFlashImageProvider mExtFlashImageProvider;
#endif
#ifdef APP_SS2_APPLICATION
//    hal::ExtFlashImageProvider mExtFlashImageProvider;
#endif
};

} // namespace nea
