#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2017.
All rights reserved.

******************************************************************************/

#include <whiteboard/ResourceClient.h>
#include <whiteboard/services/WhiteboardServices.h>
#include <whiteboard/services/CommServices.h>
#include "common/services/InfoProvider.hpp"
#include "common/services/SystemServices.hpp"
#include "buildconfig/hal/features.h"
#include "HardwareServices.hpp"

#include "hal/sysmode/masks.h"

#include "buildconfig/bsp/config.h"

#include "BleTestService.hpp"


class BleTestApplication : public whiteboard::ResourceClient
{
public:
    BleTestApplication();
    virtual ~BleTestApplication();
    void applicationContextInit();

private:
    whiteboard::services::WhiteboardServicesInstance mWbServices;
    whiteboard::services::CommServices mCommServices;

    nea::InfoProvider mInfoProvider;
    nea::SystemServices mSystemServices;
    
    // Launchables
    BleTestService mBleTestService;
};
