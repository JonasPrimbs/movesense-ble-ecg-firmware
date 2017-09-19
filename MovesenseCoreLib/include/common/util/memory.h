#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2017.
    All rights reserved.
******************************************************************************/

#include "common/compiler/genType.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
    Fills memory area with given 16bit value
    @param dest         Destination
    @param value        The 16 bit value to fill area with
    @param byteCount    Size of the memory are to fill in bytes

    @note
    - dest does not have to be aligned
    - byteCount does not have to be even
*/
void memset_16b(void* dest, uint16_t value, size_t byteCount);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
    Get Nth byte of an uint16_t value
    @param value        The data
    @param index        Byte index

    @note
    - For example uint16_t x[] = {0x1234, 0x5678} has following memory layout:
      little endian system: 34 12 78 56
      big endian system:    12 34 56 78
    - so getNthByte(x[0], 1) returns 0x12 in little endian system, and 0x34 in big endian
*/
inline uint8_t getNthByte(uint16_t value, size_t index)
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return index == 0 ? (uint8_t)value : (uint8_t)(value >> 8);
#else
    return index == 1 ? (uint8_t)value : (uint8_t)(value >> 8);
#endif
}

/**
    Advance pointer N bytes
    @param ptr          The pointer value
    @param byteCount    Number of bytes to advance
    @return             Advanced pointer value
*/
template <typename T> inline T* advancePointer(T* ptr, size_t byteCount)
{
    return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(ptr) + byteCount);
}

#endif
