#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

//#define WB_DEBUG_MEMALLOC
//#define WB_DEBUG_MEMFREE

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

#ifdef WB_DEBUG_MEMALLOC
/** Wrapper function for tracing memory allocations
 *
 * @param size Number of bytes to allocate
 * @param file File name where the block is being allocated from
 * @param line Line number where the block is being allocated from
 * @return Pointer to allocated memory
 */
WB_API void* WbMemAllocDbg(size_t size, const char* file, int line);

/** Redirect to debug alloc */
#define WbMemAlloc(size) WbMemAllocDbg(size, __FILE__, __LINE__)
#endif

#ifdef WB_DEBUG_MEMFREE

/** Wrapper function for tracing memory deallocations
 *
 * @param pointer Pointer to block that should be deallocated
 * @param file File name where the block is being allocated from
 * @param line Line number where the block is being allocated from
 */
WB_API void WbMemFreeDbg(void* pointer, const char* file, int line);

/** Redirect to debug free */
#define WbMemFree(pointer) WbMemFreeDbg(pointer, __FILE__, __LINE__)
#endif
