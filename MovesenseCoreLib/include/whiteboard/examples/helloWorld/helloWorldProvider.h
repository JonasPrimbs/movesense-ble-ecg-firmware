#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>

class HelloWorldProvider FINAL :
    public whiteboard::ResourceProvider,
    public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    
    HelloWorldProvider();
    virtual ~HelloWorldProvider();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    virtual void onGetRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;
};
