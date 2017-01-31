#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"
#include <new>

/** Macro for allocating objects "dynamically" from stack.
 *
 * @note On most targets we could use alloca function (IAR doesn't support it), which could lower stack usage of some threads.
 *       To be on the safe side, we use this macro to always reserve maximum amount of memory.
 *
 * Example:
 *
 * @code
 * WB_STACK_ALLOC(MyObject* pMyObjects, MyObject[numberOfObjects], MAX_OBJECTS * sizeof(MyObject), numberOfObjects *
 * sizeof(MyObject));
 * if (pMyObjects == NULL)
 * {
 *    ...
 * }
 * @endcode
 */
#define WB_STACK_ALLOC(result, typeAndInitializer, maxSize, size)                                                                \
    uint8 WB_EVAL_AND_CONCATENATE(stackAllocBuffer, __LINE__)[maxSize];                                                             \
    result = (((size) > (maxSize)) ? NULL : new (WB_EVAL_AND_CONCATENATE(stackAllocBuffer, __LINE__)) typeAndInitializer)
