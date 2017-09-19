#pragma once
/******************************************************************************

        Copyright (c) Suunto Oy 2015.
        All rights reserved.

******************************************************************************/
#include <stddef.h>
#include "common/core/HeapTrace.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

void* memalloc(size_t size HEAP_TRACE_PARAMS_DECL);
void* memcalloc(size_t size HEAP_TRACE_PARAMS_DECL);
void memfree(void* ptr HEAP_TRACE_PARAMS_DECL);

/* memrealloc function has been removed. Existing implementation
   added sizeof(size_t) amount of extra overhead to every memory
   allocation. Existance of this function also engourages to do
   designs that use dynamic memory management, that should be
   avoid in (semi) life critical applications.

   If you have to have memrealloc functionality, don't reimplement
   it here. Instead create another layer for allocating reallocatable
   buffers and this way avoid extra overhead to all other allocations

void* memrealloc(void* ptr, size_t size);
*/
#if __cplusplus
} // extern "C"
#endif // __cplusplus

#ifdef CMAKE_NEA_HAVE_HEAP_TRACE

#define memalloc(size) memalloc((size), HEAP_TRACE_ALLOC)
#define memcalloc(size) memcalloc((size), HEAP_TRACE_ALLOC)
#define memfree(ptr) memfree((ptr), HEAP_TRACE_FREE)

#endif
