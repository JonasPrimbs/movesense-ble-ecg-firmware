// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include <whiteboard/ResourceClient.h>
//#include <inetgw/inetgwlp/InetGwLPProvider.h> TODO: Uncomment when we enable inetgw support
#include <whiteboard/services/WhiteboardServices.h>
#include <whiteboard/services/CommServices.h>
#include <whiteboard/services/MetricsServices.h>

#include "common/services/SystemServices.hpp"
#include "common/services/StandardServices.hpp"
#include "common/services/LowPriorityServices.hpp"
#include "nea/hal/sensor/AFE/MAX3000x/MAX30004_provider.hpp"

#include "ApplicationServices.hpp"

#include "HardwareServices.hpp"

#include "LogbookDb.hpp"
#include "SettingsService.hpp"
#include "StatesService.hpp"
#include "internal/AppInfoProvider.hpp"


class SmartSensor2Application : public whiteboard::ResourceClient
{
public:
    SmartSensor2Application();
    void applicationContextInit();
    virtual ~SmartSensor2Application();

    virtual void onNotify(wb::ResourceId resourceId, const wb::Value& value, const wb::ParameterList& parameters) OVERRIDE;

    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE FINAL;
    
private:
    void runStartupClients();
    void stopHWServices();

    void startOptionalCoreModules();

    whiteboard::services::WhiteboardServicesInstance mWbServices;
    //nea::inetgateway::InetGwLPProvider mInetGw; TODO: Uncomment when we enable inetgw support
    whiteboard::services::CommServices mCommServices;

    nea::SystemServices mSystemServices;
    nea::StandardServices mStandardServices;
    HardwareServices hardwareServices;

    nea::LowPriorityServices mLowPriorityServices;

    ExtflashChunkStorage mChunkStorage;
    MeasStorage          mMeasStorage;
    StatesService mStatesService;

    LogbookDb       mLogbookDb;
    SettingsService mSettingsService;

    AppInfoProvider mAppInfoProvider;

    // Shutdown timer
    bool                mAppModulesStarted;
    whiteboard::TimerId mShutdownTimerId;

    whiteboard::services::MetricsServices mWbMetricsServices;
};
