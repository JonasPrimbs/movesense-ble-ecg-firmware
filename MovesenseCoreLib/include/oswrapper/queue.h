#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2012.
All rights reserved.

******************************************************************************/

#include "common/core/HeapTrace.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* QueueHandle;

typedef void* QueueSetHandle;

typedef void* QueueSetMemberHandle;

QueueHandle queueCreate(const char* name, uint32_t numItems, uint32_t itemSize HEAP_TRACE_PARAMS_DECL);
void queueDelete(QueueHandle queueHandle);

bool queueHasMessages(QueueHandle queueHandle);
uint32_t queueGetFreeCount(QueueHandle queueHandle);

bool queueSendToBack(QueueHandle queueHandle, const void* pItem, uint32_t timeToWaitMillis);
bool queueSendToFront(QueueHandle queueHandle, const void* pItem, uint32_t timeToWaitMillis);
bool queueSendToBackISR(QueueHandle queueHandle, const void* pItem);
bool queueSendToFrontISR(QueueHandle queueHandle, const void* pItem);

void queueReceive(QueueHandle queueHandle, void* pItemBuffer);
bool queueTryToReceive(QueueHandle queueHandle, void* pItemBuffer, uint32_t timeToWaitMillis);
bool queueReceiveISR(QueueHandle queueHandle, void* pItemBuffer);
bool queuePeek(QueueHandle queueHandle, void* pItemBuffer, uint32_t timeToWaitMillis);

#ifdef CMAKE_NEA_HAVE_HEAP_TRACE

inline QueueHandle trace_queueCreate(const char* name, uint32_t numItems, uint32_t itemSize HEAP_TRACE_PARAMS_DECL)
{
    return queueCreate(name, numItems, itemSize HEAP_TRACE_PARAMS);
}

#define queueCreate(name, numItems, itemSize) queueCreate(name, numItems, itemSize, HEAP_TRACE_ENTER("queueCreate"))

#endif

#ifdef __cplusplus
} // extern "C" {
#endif