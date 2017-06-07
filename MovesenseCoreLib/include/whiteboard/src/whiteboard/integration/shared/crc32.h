#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <stdint.h>
#include <stddef.h>
#include "whiteboard/integration/shared/macros.h"

namespace whiteboard 
{
    //calculates crc32 from buffer. uses 0x00000000 as initial value
    WB_API uint32_t crc32(const void *pBuffer, uint32_t size);
    
    //single step crc32
    WB_API uint32_t crc32Add(uint32_t crc, uint8_t byte);

    // IEEE 802.3 CRC-32 (zero initial value)
    WB_API uint32_t crc32IEEE(const void* pBuffer, size_t size);

    // IEEE 802.3 CRC-32 with given initial value
    WB_API uint32_t crc32IEEEWithInitial(uint32_t initial, const void* pBuffer, size_t size);

} // namspace whiteboard
