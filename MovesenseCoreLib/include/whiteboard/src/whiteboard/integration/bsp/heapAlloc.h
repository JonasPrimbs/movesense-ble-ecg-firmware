#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

/** Allocates a new memory block.
 *
 * @note This function shall not ever return NULL. If memory cannot be allocated,
 *       system must be brought down immediately.
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory
 */
WB_API void* WbMemAlloc(size_t size);

/** Frees memory block.
 *
 * @note This function can be called with NULL pointer.
 * @param pointer Pointer to block that should be deallocated
 */
WB_API void WbMemFree(void* pointer);

#ifdef WB_HAVE_HEAP_TRACE

#ifdef __cplusplus
extern "C" {
#endif

WB_HEAP_TRACE_DECLARE_WRAPPER(void*, WbMemAlloc, size_t);
WB_HEAP_TRACE_DECLARE_VOID_WRAPPER(WbMemFree, void*);
#define WbMemAlloc(size) WB_HEAP_TRACE_WRAPPER(WbMemAlloc, size)
#define WbMemFree(pointer) WB_HEAP_TRACE_WRAPPER(WbMemFree, pointer)

#ifdef __cplusplus
}
#endif

#endif // WB_HAVE_HEAP_TRACE
