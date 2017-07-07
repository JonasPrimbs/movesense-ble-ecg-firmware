#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2017.
All rights reserved.

******************************************************************************/

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>


class BleService FINAL :
    private whiteboard::ResourceProvider,
    public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    BleService();
    ~BleService();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }

    virtual void onGetRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onPutRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onPostRequest(const whiteboard::Request& request,
                               const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onDeleteRequest(const whiteboard::Request& request,
                                 const whiteboard::ParameterList& parameters) OVERRIDE;

    inline void handleGetBLEAddress(const whiteboard::Request& request);
    inline void handleGetAdvertisingState(const whiteboard::Request& request);
    inline void handleGetAdvertisingSettings(const whiteboard::Request& request);
    
    inline void handlePutAdvertisingSettings(const whiteboard::Request& request, const whiteboard::ParameterList& parameters);

};
