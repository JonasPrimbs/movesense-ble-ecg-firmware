#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2012.
All rights reserved.

******************************************************************************/

#include "oswrapper/debugAndTrace.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* MutexHandle;

MutexHandle mutexCreate(OS_API_DEBUG_PARAMS_DECL_VOID);
MutexHandle mutexCreateRecursive(OS_API_DEBUG_PARAMS_DECL_VOID);

void mutexDelete(MutexHandle mutexHandle);
void mutexDeleteRecursive(MutexHandle mutexHandle);

void mutexLock(MutexHandle mutexHandle);
bool mutexTryLock(MutexHandle mutexHandle, uint32_t timeToWaitMillis);
void mutexLockRecursive(MutexHandle mutexHandle);
bool mutexTryLockRecursive(MutexHandle mutexHandle, uint32_t timeToWaitMillis);
bool mutexUnlock(MutexHandle mutexHandle);
bool mutexUnlockRecursive(MutexHandle mutexHandle);

#ifdef CMAKE_NEA_HAVE_WAITOBJECT_DEBUGNAMES

#define mutexCreate() mutexCreate(MUTEX_NAME)
#define mutexCreateRecursive() mutexCreateRecursive(MUTEX_NAME)

#elif CMAKE_NEA_HAVE_HEAP_TRACE

#define mutexCreate() mutexCreate(HEAP_TRACE_ENTER("mutexCreate"))
#define mutexCreateRecursive() mutexCreateRecursive(HEAP_TRACE_ENTER("mutexCreateRecursive"))

#endif

#ifdef __cplusplus
} // extern "C" {
#endif
