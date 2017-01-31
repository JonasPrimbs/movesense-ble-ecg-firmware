#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"

/**
* Allocates aligned buffer from the memory pool
*
* @param numberOfBytes [in] Number of bytes to allocate
* @param alignment [in] Required alignment of the data buffer
* @return Allocated buffer or NULL if memory allocation failed
*/
WB_API void* WbMemAllocAligned(size_t numberOfBytes, size_t alignment);

/**
* Deallocates buffer that was previously allocated with WbMemAllocAligned function
* 
* @param pBuffer [in] Pointer to buffer that should be deallocated
*/
WB_API void WbMemFreeAligned(void* pBuffer);
