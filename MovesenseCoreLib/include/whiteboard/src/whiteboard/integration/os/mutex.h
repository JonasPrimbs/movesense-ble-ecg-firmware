#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

struct WbMutex; // Forward declaration hides the implementation
typedef WbMutex* WbMutexHandle;
#define WB_INVALID_MUTEX NULL

/** Creates new mutex.

@return handle to the created mutex or WB_INVALID_MUTEX if creation failed
*/
WB_API WbMutexHandle WbMutexCreate();

/** Deletes mutex and frees it's used resources.

@param mutexHandle Handle to mutext to delete
*/
WB_API void WbMutexDelete(WbMutexHandle mutexHandle);

/** Acquires mutual exclusion. Blocks until the mutex has been acquired.

@param mutexHandle Handle to mutext to acquire
*/
WB_API void WbMutexLock(WbMutexHandle mutexHandle);

/** Tries to acquire mutual exclusion, if the mutex is already acquired
by another thread will not block.

@param mutexHandle Handle to mutext to acquire

@return true if lock succeeded, else false.
*/
WB_API bool WbMutexTryLock(WbMutexHandle mutexHandle);

/** Releases mutual exclusion.

@param mutexHandle Handle to mutext to release
*/
WB_API void WbMutexUnlock(WbMutexHandle mutexHandle);
