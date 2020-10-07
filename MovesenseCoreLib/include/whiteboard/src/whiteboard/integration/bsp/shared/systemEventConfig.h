#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

namespace whiteboard
{

/** Number of system event severity levels.
 *
 * @see WbSystemEventSeverity
 */
static const size_t WB_SYSEVENT_SEVERITY_COUNT = 4;

/** Number of system event modules.
*
* @see WbSystemEventModule
*/
static const size_t WB_SYSEVENT_MODULE_COUNT = 7;

/** Array of bools for doing run-time checking whether system event logging is enabled
 * for specific system event. Using here more memory than needed to have simpler check.
 */
extern volatile bool gSystemEventEnabled[WB_SYSEVENT_MODULE_COUNT][WB_SYSEVENT_SEVERITY_COUNT];

#define WB_IS_SYSEVENT_ENABLED(module, severity) whiteboard::gSystemEventEnabled[whiteboard::module][whiteboard::severity]

}

