// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include <whiteboard/ResourceClient.h>
#include <inetgw/inetgwlp/InetGwLPProvider.h>
#include <whiteboard/services/WhiteboardServices.h>
#include <whiteboard/services/CommServices.h>
#include "common/services/SystemServices.hpp"
#include "common/services/StandardServices.hpp"
#include "common/services/LowPriorityServices.hpp"
#include "ApplicationServices.hpp"
//#include "Hackathon2016SampleService.hpp"
#include "HardwareServices.hpp"

#include "DataLogger.hpp"
#include "Logbook.hpp"


class SmartSensor2Application
{
public:
    SmartSensor2Application();
    void applicationContextInit();
    virtual ~SmartSensor2Application();

private:
    void runStartupClients();

    
    whiteboard::services::WhiteboardServicesInstance mWbServices;
    //nea::inetgateway::InetGwLPProvider mInetGw;
    whiteboard::services::CommServices mCommServices;

    nea::SystemServices mSystemServices;
    nea::StandardServices mStandardServices;
    HardwareServices hardwareServices;

    nea::LowPriorityServices mLowPriorityServices;

    ExtflashChunkStorage mChunkStorage;
    MeasStorage          mMeasStorage;
        
    LogbookDb       mLogbookDb;
    DataLogger      mDataLogger;
    Logbook         mLogbook;
};
