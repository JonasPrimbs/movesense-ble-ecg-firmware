// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include <whiteboard/ResourceClient.h>
//#include <inetgw/inetgwlp/InetGwLPProvider.h> TODO: Uncomment when we enable inetgw support
#include <whiteboard/services/WhiteboardServices.h>
#include <whiteboard/services/CommServices.h>
#include <whiteboard/services/MetricsServices.h>

#include "hwconfig.h"
#include "common/services/SystemServices.hpp"
#include "common/services/StandardServices.hpp"
#include "common/services/LowPriorityServices.hpp"

#ifndef HWCONFIG_SIMU
#include "hal/sensor/AFE/MAX3000X/MAX30004_provider.hpp"
#else
#include "hal/sensor/AFE/MAX3000X_simu/MAX30004_provider.hpp"
#endif

#include "ApplicationServices.hpp"

#include "HardwareServices.hpp"

#include "LogbookDb.hpp"
#include "SettingsService.hpp"
#include "StatesService.hpp"
#include "internal/AppInfoProvider.hpp"
#include "internal/ConnectorSwitch.hpp"

#ifndef HWCONFIG_SIMU
#define ADD_BLE_CLASSES
#endif

#ifdef ADD_BLE_CLASSES
#include "BleGattService.hpp"
#endif // def ADD_BLE_CLASSES

class SmartSensor2Application : public whiteboard::ResourceClient 
#ifdef ADD_BLE_CLASSES
    ,public BleGattService::IBatteryServiceEventCb
#endif // def ADD_BLE_CLASSES
{
public:
    SmartSensor2Application();
    void applicationContextInit();
    virtual ~SmartSensor2Application();

    virtual void onNotify(wb::ResourceId resourceId, const wb::Value& value, const wb::ParameterList& parameters) OVERRIDE;

    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE FINAL;

    // BleGattService::IBatteryServiceEventCb interface
    void onBatteryServiceEvt(bool bNotificationEnabled);

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

    ConnectorSwitch mConnSwitch;

    // Shutdown timer
    bool                mAppModulesStarted;
    whiteboard::TimerId mShutdownTimerId;

    whiteboard::services::MetricsServices mWbMetricsServices;

    whiteboard::DpcFunctor mBASDpc;
    volatile bool mIsBASNotifEnabled;
    void basDpcHandler();
};
