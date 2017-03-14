#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"

/** Allocation aligment of WbMalloc (system malloc) */
#ifndef WB_MALLOC_ALIGNMENT
#define WB_MALLOC_ALIGNMENT sizeof(void*)
#endif


/**
* Allocates aligned buffer from the memory pool
*
* @param numberOfBytes [in] Number of bytes to allocate
* @param alignment [in] Required alignment of the data buffer
* @return Allocated buffer or NULL if memory allocation failed
*/
WB_API void* WbMemAllocAlignedAlways(size_t numberOfBytes, size_t alignment);

/**
* Deallocates buffer that was previously allocated with WbMemAllocAligned function
* 
* @param pBuffer [in] Pointer to buffer that should be deallocated
*/
WB_API void WbMemFreeAlignedAlways(void* pBuffer);

/** Helper template for making choise whether aligned allocation is needed */
template <size_t alignment>
struct NeedsAlignedAllocation
{
    static const bool value = alignment > WB_MALLOC_ALIGNMENT;
};

/**
* Allocates aligned buffer from the memory pool
*
* @tparam alignment [in] Required alignment of the data buffer
* @param numberOfBytes [in] Number of bytes to allocate
* @return Allocated buffer or NULL if memory allocation failed
*/
template <size_t alignment>
inline void* WbMemAllocAligned(
    size_t numberOfBytes,
    typename whiteboard::EnableIf<!NeedsAlignedAllocation<alignment>::value>::type* = 0)
{
    WB_STATIC_VERIFY(WB_IS_POWER_OF_TWO(alignment), InvalidAlignment);
    return WbMemAlloc(numberOfBytes);
}

template <size_t alignment>
inline void* WbMemAllocAligned(
    size_t numberOfBytes,
    typename whiteboard::EnableIf<NeedsAlignedAllocation<alignment>::value>::type* = 0)
{
    WB_STATIC_VERIFY(WB_IS_POWER_OF_TWO(alignment), InvalidAlignment);
    return WbMemAllocAlignedAlways(numberOfBytes, alignment);
}

/**
* Deallocates buffer that was previously allocated with WbMemAllocAligned function
*
* @param pBuffer [in] Pointer to buffer that should be deallocated
*/
template <size_t alignment>
inline void WbMemFreeAligned(
    void* pBuffer,
    typename whiteboard::EnableIf<!NeedsAlignedAllocation<alignment>::value>::type* = 0)
{
    WB_STATIC_VERIFY(WB_IS_POWER_OF_TWO(alignment), InvalidAlignment);
    WbMemFree(pBuffer);
}

template <size_t alignment>
inline void WbMemFreeAligned(
    void* pBuffer,
    typename whiteboard::EnableIf<NeedsAlignedAllocation<alignment>::value>::type* = 0)
{
    WB_STATIC_VERIFY(WB_IS_POWER_OF_TWO(alignment), InvalidAlignment);
    WbMemFreeAlignedAlways(pBuffer);
}

