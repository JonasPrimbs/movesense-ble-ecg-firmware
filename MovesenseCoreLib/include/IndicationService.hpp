// Copyright (c) Suunto Oy 2017. All rights reserved.
#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>

#include "ui_ind/resources.h"
#include "common/compiler/genType.h"

class IndicationService FINAL :
    private whiteboard::ResourceProvider,
    public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;
    IndicationService();
    ~IndicationService();

private:

    typedef enum
    {
        PHASE_LED_OFF,
        PHASE_LED_ON
    } IndicationPhase;

    WB_RES::VisualIndType mCurrentIndication;
    whiteboard::TimerId mIndTimer;

    // indication parameters
    struct
    {
        size_t durationON;
        size_t durationOFF;
        IndicationPhase phase;

    } mIndParams;

    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }


    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE FINAL;


    virtual void onGetRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onPutRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;


    void setIndicationType(const WB_RES::VisualIndType &newIndication);


    // single-blink indication if durationOFF == 0
    void startIndication(const size_t durationON, const size_t durationOFF = 0);

    void stopIndication();

    void indicationStep();
};
