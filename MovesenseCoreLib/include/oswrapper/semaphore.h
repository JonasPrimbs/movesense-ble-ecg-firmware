#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2012.
All rights reserved.

******************************************************************************/

#include "common/core/HeapTrace.h"
#include "oswrapper/debugAndTrace.h"
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef void* SemaphoreHandle;

SemaphoreHandle semaphoreCreate(uint32_t maxCount, uint32_t initialCount OS_API_DEBUG_PARAMS_DECL);
/*
 * Create binary semaphore. Note that binary semaphore has initial count of 1
 */
SemaphoreHandle semaphoreCreateBinary(OS_API_DEBUG_PARAMS_DECL_VOID);

void semaphoreDelete(SemaphoreHandle semaHandle);
void semaphoreTake(SemaphoreHandle semaHandle);
bool semaphoreTryToTake(SemaphoreHandle semaHandle, uint32_t timeToWaitMillis);
bool semaphoreGive(SemaphoreHandle semaHandle);
bool semaphoreGiveISR(SemaphoreHandle semaHandle);

#ifdef CMAKE_NEA_HAVE_WAITOBJECT_DEBUGNAMES

#define semaphoreCreate(maxCount, initialCount) semaphoreCreate(maxCount, initialCount, SEMAPHORE_NAME)
#define semaphoreCreateBinary() semaphoreCreateBinary(SEMAPHORE_NAME)

#elif defined(CMAKE_NEA_HAVE_HEAP_TRACE)

#define semaphoreCreate(maxCount, initialCount) semaphoreCreate(maxCount, initialCount, HEAP_TRACE_ENTER("semaphoreCreate"))

#define semaphoreCreateBinary() semaphoreCreateBinary(HEAP_TRACE_ENTER("semaphoreCreateBinary"))

#endif

#if defined(__cplusplus)
} // extern "C" {
#endif
