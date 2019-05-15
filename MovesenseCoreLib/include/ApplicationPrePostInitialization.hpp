#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <whiteboard/integration/port.h>
#include "common/core/WhiteboardAddress.hpp"
#include "movesense.h"

/** Application initialization parameters */
struct ApplicationInitializationParameters_t
{
    WhiteboardAddress_t whiteboardAddress;
    const char* productName;
    bool enableSerialComm;
    bool enableBLEComm;
};

/**
 * Set nea::InfoProvider::DeviceFirmwareVersion
 */
void preApplicationStartInitFirmwareVersion(void);

/** Initialization code that should be called just before application is started
 *
 * @param parameters Initialization parameters
 * @return A value indicating wether initialization steps were successfully completed
 */
bool preApplicationStartInit(const ApplicationInitializationParameters_t& parameters);

/** Initialization code that should be called just after application is started
*
*/
void postApplicationStartInit(const ApplicationInitializationParameters_t& parameters);
