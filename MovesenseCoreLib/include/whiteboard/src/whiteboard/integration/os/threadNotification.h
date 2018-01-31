#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"

#include "whiteboard/integration/os/eventGroup.h"

struct WbThreadNotification; // Forward declaration hides the implementation
typedef WbThreadNotification* WbThreadNotificationHandle;
#define WB_INVALID_THREADNOTIFICATION NULL

/** Initializes thread notification for given thread.
* Note that thread can have only one notification bit set.
*
* @param threadHandle Thread which notification should be initialized
* @return Handle that should be used to do thread notifications
*/
WB_API WbThreadNotificationHandle WbThreadNotificationInitialize(WbThreadHandle threadHandle);

/** Deinitializes thread notification bit set
*
* @param threadNotificationHandle Thread notification structure that should be deinitialized
*/
WB_API void WbThreadNotificationDeinitialize(WbThreadNotificationHandle threadNotificationHandle);

/** Waits for flags in current thread's notification bit set. Flags are cleared on exit.
*
* @param threadNotificationHandle Handle of thread notification structure
* @param timeoutMs Timeout of wait in milliseconds
* @return Mask of flags that were set or zero if timeout elapsed
*/
WB_API WbEventFlagMask WbThreadNotificationWait(WbThreadNotificationHandle threadNotificationHandle, size_t timeoutMs);

/** Sets flags
*
* @param threadNotificationHandle Thread notification structure of thread which notification flags should be set
* @param flags Mask of flags that should be set
* @param isIsr A value indicating whether this function is called from interrupt service routine
*/
WB_API void WbThreadNotificationSetFlags(WbThreadNotificationHandle threadNotificationHandle, WbEventFlagMask flags);

#ifdef MIM_HAVE_INTERRUPT_API

/** Sets flags from interrupt service routine
*
* @param threadNotificationHandle Thread notification structure of thread which notification flags should be set
* @param flags Mask of flags that should be set
* @param isIsr A value indicating whether this function is called from interrupt service routine
*/
WB_API void WbThreadNotificationSetFlagsISR(WbThreadNotificationHandle threadNotificationHandle, WbEventFlagMask flags);

#endif

#ifdef WB_HAVE_HEAP_TRACE

#ifdef __cplusplus
extern "C" {
#endif

WB_HEAP_TRACE_DECLARE_WRAPPER(WbThreadNotificationHandle, WbThreadNotificationInitialize, WbThreadHandle);
#define WbThreadNotificationInitialize(threadHandle) WB_HEAP_TRACE_WRAPPER(WbThreadNotificationInitialize, threadHandle)

#ifdef __cplusplus
}
#endif

#endif // WB_HAVE_HEAP_TRACE
