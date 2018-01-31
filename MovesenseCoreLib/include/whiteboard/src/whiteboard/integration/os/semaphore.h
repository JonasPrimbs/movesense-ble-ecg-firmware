#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

struct WbSemaphore; // Forward declaration hides the implementation
typedef WbSemaphore* WbSemaphoreHandle;

#define WB_INVALID_SEMAPHORE NULL

/** Creates new semaphore

@param initialCount The initial count for the semaphore object. This value must be 
greater than or equal to zero and less than or equal to maximumCount. The state of a 
semaphore is signaled when its count is greater than zero and nonsignaled when it is 
zero. The count is decreased by one whenever a wait function releases a thread that was 
waiting for the semaphore. The count is increased by a specified amount by calling the 
WbSemaphoreRelease function.

@param maximumCount The maximum count for the semaphore object. 

@return handle to the created semaphore or WB_INVALID_SEMAPHORE if creation failed
*/
WB_API WbSemaphoreHandle WbSemaphoreCreate(size_t initialCount, size_t maximumCount);

/** Deletes a semaphore and frees used resources.

@param semaphoreHandle Semaphore to delete
*/
WB_API void WbSemaphoreDelete(WbSemaphoreHandle semaphoreHandle);

/** Blocks until the semaphores internal counter is non-zero.

@param semaphoreHandle Semaphore to wait
*/
WB_API void WbSemaphoreWait(WbSemaphoreHandle semaphoreHandle);

/** Blocks until the semaphores internal counter is non-zero or given timeout occurs.

@param semaphoreHandle Semaphore to wait/test
@param timeoutMs time in [ms] to wait if the semaphores count is zero

@return true if wait succeeded, false if timeout occurred.
*/
WB_API bool WbSemaphoreTryWait(WbSemaphoreHandle semaphoreHandle, size_t timeoutMs);

/** Increases the count of the semaphore by one.

@param semaphoreHandle Semaphore to delete.
*/
WB_API void WbSemaphoreRelease(WbSemaphoreHandle semaphoreHandle);

#ifdef MIM_HAVE_INTERRUPT_API

/** Increases the count of the semaphore by one.
*
* @note This function should be called only from interrupt service routines
*
* @param semaphoreHandle Semaphore to delete.
*/
WB_API void WbSemaphoreReleaseISR(WbSemaphoreHandle semaphoreHandle);

#endif

#ifdef WB_HAVE_HEAP_TRACE

#ifdef __cplusplus
extern "C" {
#endif

WB_HEAP_TRACE_DECLARE_WRAPPER(WbSemaphoreHandle, WbSemaphoreCreate, size_t, size_t);
#define WbSemaphoreCreate(initialCount, maximumCount) WB_HEAP_TRACE_WRAPPER(WbSemaphoreCreate, initialCount, maximumCount)

#ifdef __cplusplus
}
#endif

#endif // WB_HAVE_HEAP_TRACE
