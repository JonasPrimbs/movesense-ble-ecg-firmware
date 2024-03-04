#pragma once
#include "common/compiler/genType.h"
#include "common/core/HeapTrace.h"

#include "thread.h"
#include "eventGroup.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef void* ThreadNotificationHandle;

/** Initializes thread notification for given thread.
* Note that thread can have only one notification bit set.
*
* @param threadHandle Thread which notification should be initialized
* @return Handle that should be used to do thread notifications
*/
ThreadNotificationHandle threadNotificationInitialize(ThreadHandle threadHandle);

/** Deinitializes thread notification bit set
*
* @param threadNotificationHandle Thread notification structure that should be deinitialized
*/
void threadNotificationDeinitialize(ThreadNotificationHandle threadNotificationHandle);

/** Waits for flags in current thread's notification bit set. Flags are cleared on exit.
*
* @param threadNotificationHandle Handle of thread notification structure
* @param timeoutMs Timeout of wait in milliseconds
* @return Mask of flags that were set or zero if timeout elapsed
*/
EventFlagMask threadNotificationWait(ThreadNotificationHandle threadNotificationHandle, size_t timeoutMs);

/** Sets flags
*
* @param threadNotificationHandle Thread notification structure of thread which notification flags should be set
* @param flags Mask of flags that should be set
* @param isIsr A value indicating whether this function is called from interrupt service routine
*/
void threadNotificationSetFlags(ThreadNotificationHandle threadNotificationHandle, EventFlagMask flags);

/** Sets flags from interrupt service routine
*
* @param threadNotificationHandle Thread notification structure of thread which notification flags should be set
* @param flags Mask of flags that should be set
* @param isIsr A value indicating whether this function is called from interrupt service routine
*/
void threadNotificationSetFlagsISR(ThreadNotificationHandle threadNotificationHandle, EventFlagMask flags);




#if defined(__cplusplus)
} // extern "C" {
#endif
