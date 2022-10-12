#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/ResourceClient.h>
#include "common/compiler/genType.h"

class SettingsService FINAL :
    private whiteboard::ResourceProvider,
    public whiteboard::LaunchableModule,
    protected whiteboard::ResourceClient

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;
    SettingsService();
    ~SettingsService();

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

    virtual void onGetRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

};

