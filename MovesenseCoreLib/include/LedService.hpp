// Copyright (c) Suunto Oy 2017. All rights reserved.
#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>

class LedService FINAL :
    private whiteboard::ResourceProvider,
    public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    LedService();
    ~LedService();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }

    virtual void onPutRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;
};