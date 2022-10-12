#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2017.
All rights reserved.

******************************************************************************/

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/DpcFunctor.h>
#include "common/compiler/genType.h"

class BleStandardHRS FINAL :
    public whiteboard::LaunchableModule,
    private whiteboard::ResourceProvider
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;
    BleStandardHRS();
    ~BleStandardHRS();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    virtual void onPostRequest(const wb::Request& request, const wb::ParameterList& parameters) OVERRIDE;

    void onBleHrsEvt();

    bool m_bInitCompleted;
    bool initHrs();

public:
    static BleStandardHRS *spInstance;
    wb::DpcFunctor mEvtDpc;
    bool m_bNotifEnabled;
};
