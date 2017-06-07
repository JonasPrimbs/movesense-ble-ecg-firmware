#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/

#include <whiteboard/ResourceClient.h>
#include <whiteboard/services/WhiteboardServices.h>
#include <whiteboard/services/CommServices.h>
#include "common/services/InfoProvider.hpp"
#include "common/services/LowPriorityServices.hpp"
#include "common/services/SystemServices.hpp"
#include "buildconfig/hal/features.h"
#include "HardwareServices.hpp"

#include "hal/sysmode/masks.h"

#include "buildconfig/bsp/config.h"

#include "CalibrationService.hpp"
#include "ManufacturingService.hpp"


class FactoryCalibrationApplication : public whiteboard::ResourceClient
{
public:
    FactoryCalibrationApplication();
    virtual ~FactoryCalibrationApplication();
    void applicationContextInit();

/*    virtual void onNotify(wb::ResourceId resourceId, const wb::Value& value, const wb::ParameterList& parameters) OVERRIDE;

    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

    virtual void onSubscribeResult(wb::RequestId requestId, wb::ResourceId resourceId, wb::Result resultCode, const wb::Value& resultData) OVERRIDE;
*/
private:
    whiteboard::services::WhiteboardServicesInstance mWbServices;
    whiteboard::services::CommServices mCommServices;

    nea::InfoProvider mInfoProvider;
    nea::SystemServices mSystemServices;
    nea::LowPriorityServices mLowPriorityServices;
    
    // Launchables
    hal::AccelerometerProvider mAccelProvider;
    ManufacturingService mManufService;
    CalibrationService mCalibService;
};
