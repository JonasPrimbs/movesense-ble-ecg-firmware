#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

namespace whiteboard {

/** Severity level of system events */
enum WbSystemEventSeverity
{
    WB_SYSEVENT_SEVERITY_ERROR,
    WB_SYSEVENT_SEVERITY_WARNING,
    WB_SYSEVENT_SEVERITY_EVENT,
    WB_SYSEVENT_SEVERITY_TRACE
};

/** System modules of system events */
enum WbSystemEventModule
{
    WB_SYSEVENT_ID_API,
    WB_SYSEVENT_ID_COMM,
    WB_SYSEVENT_ID_DEVD,
    WB_SYSEVENT_ID_MAIN,
    WB_SYSEVENT_ID_PORT,
    WB_SYSEVENT_ID_ELAS,
    WB_SYSEVENT_ID_JAVA,

    WB_NUMBER_OF_SYSTEM_EVENT_MODULES
};

#ifdef WB_HAVE_SYSTEM_EVENT_CONFIGURATION

/** Sets logging level of specified Whiteboard module
*
* @param module Module which log level should be changed
* @param logLevel New logging level.
*/
void WbSetSystemEventLogging(WbSystemEventModule module, WbSystemEventSeverity logLevel);

#endif

} // namespace

