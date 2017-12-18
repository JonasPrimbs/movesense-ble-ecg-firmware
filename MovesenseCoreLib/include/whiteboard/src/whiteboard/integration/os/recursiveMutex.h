#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

struct WbRecursiveMutex; // Forward declaration hides the implementation
typedef WbRecursiveMutex* WbRecursiveMutexHandle;

#define WB_INVALID_RECURSIVE_MUTEX NULL

/** Creates new recursive mutex. Recursive mutex can be acquired multiple times within the same thread.

@return handle to the created mutex or WB_INVALID_RECURSIVE_MUTEX if creation failed
*/
WB_API WbRecursiveMutexHandle WbRecursiveMutexCreate();

/** Deletes recursive mutex and frees it's used resources.

@param mutexHandle Handle to mutext to delete
*/
WB_API void WbRecursiveMutexDelete(WbRecursiveMutexHandle mutexHandle);

/** Acquires mutual exclusion. Blocks until the mutex has been acquired.

Note: Can be called multiple times within the same thread.

@param mutexHandle Handle to mutext to acquire
*/
WB_API void WbRecursiveMutexLock(WbRecursiveMutexHandle mutexHandle);

/** Tries to acquire mutual exclusion, if the mutex is already acquired
by another thread will not block.

@param mutexHandle Handle to mutext to acquire

@return true if lock succeeded, else false.
*/
WB_API bool WbRecursiveMutexTryLock(WbRecursiveMutexHandle mutexHandle);

/** Releases mutual exclusion. 

@param mutexHandle Handle to mutext to release
*/
WB_API void WbRecursiveMutexUnlock(WbRecursiveMutexHandle mutexHandle);

#ifdef WB_HAVE_HEAP_TRACE

#ifdef __cplusplus
extern "C" {
#endif

WB_HEAP_TRACE_DECLARE_WRAPPER(WbRecursiveMutexHandle, WbRecursiveMutexCreate);
#define WbRecursiveMutexCreate() WB_HEAP_TRACE_WRAPPER(WbRecursiveMutexCreate)

#ifdef __cplusplus
}
#endif

#endif // WB_HAVE_HEAP_TRACE
