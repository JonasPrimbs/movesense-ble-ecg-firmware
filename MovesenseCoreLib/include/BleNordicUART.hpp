#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2017.
All rights reserved.

******************************************************************************/

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/DpcFunctor.h>
#include <whiteboard/containers/RequestQueue.h>
#include "common/compiler/genType.h"

class BleNordicUART FINAL : public whiteboard::LaunchableModule, private whiteboard::ResourceProvider
{
public:
    static const size_t MAX_NUS_CLIENTS;
    static constexpr size_t NUS_MAX_DATA_LEN = 161-2-1;

    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;
    BleNordicUART();
    ~BleNordicUART();

    static BleNordicUART* spInstance;

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

    void onNUSEvt();

    bool m_bInitCompleted;
    wb::RequestQueue<4, void> m_postReqQueue;
    bool initNus();

public:
    wb::DpcFunctor mEvtDpc;
    // Temp buffer for received data
    size_t mRxDataLen;
    uint8_t mRxDataBuffer[NUS_MAX_DATA_LEN];
};
