#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2017.
All rights reserved.

******************************************************************************/

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>

#include "buildconfig/hal/features.h"
#include "common/compiler/genType.h"

class BleTestService FINAL :
    private whiteboard::ResourceProvider,
    public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;
    BleTestService();
    ~BleTestService();

private:
    uint8_t currentCommand;
    uint8_t currentChannel;
    uint8_t currentChannelEnd;
    uint8_t currentPayloadLength;
    uint32_t currentPayloadPattern;
    volatile bool softdeviceDisabled;
    bool hasValueCurrentChannelEnd;
#ifdef BUILD_HAL_PMIC_DRIVER_BQ25120
    bool mIsChargerConnected;
#endif
    uint8_t currentTimeout;
    void initDTM();


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

    inline void handleModeGet(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters);

    inline void handleModePut(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters);

    inline void handleTxPowerPut(const whiteboard::Request& request,
                                 const whiteboard::ParameterList& parameters);

};
