#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

// System Event definitions

// Usage:
// First, make the desired systemevent configuration in application level cmake configuration file.
//
// Then, before any other code, include this header:
//
// #include "platform/bsp/systemevent.h"
//
// Then, to log a message, e.g.
//
// SYSEVENT_BLE_TRACE(" Found %d Bluetooth devices!", getNumberOfBluetoothDevices() )
//
// or, if there are no parameters (since the above form will not compile with 0 parameters):
//
// SYSEVENT_BLE_TRACE("%s", "Entered function" )
//
// NOTE: functions in this header CANNOT be used within an interrupt context!
//
// Severity levels
enum SystemEventSeverity_e
{
    SYSEVENT_SEVERITY_ERROR = 0,
    SYSEVENT_SEVERITY_WARNING = 1,
    SYSEVENT_SEVERITY_EVENT = 2,
    SYSEVENT_SEVERITY_TRACE = 3,
    SYSEVENT_SEVERITY_NONE = 4
};

// Internal logging function - not for use by any other code!
void SystemEventInternal(uint16_t severityAndModuleId, const char* format, ...);

// Internal logging macro - not for use by any other code!
#define SYSEVENT_INTERNAL(sysevent_severity, sysevent_module, ...)                                                               \
    do                                                                                                                           \
    {                                                                                                                            \
        SystemEventInternal((sysevent_severity << 12) | sysevent_module, __VA_ARGS__);                                           \
    } while (0)

#include "buildconfig/systemevent/systemevent_id.h"
#include "buildconfig/systemevent/systemevent_internal.h"

#if defined(__cplusplus)
}
#endif /* __cplusplus */
