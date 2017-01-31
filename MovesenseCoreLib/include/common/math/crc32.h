#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// IEEE 802.3 CRC-32 with given initial value
uint32_t crc32IEEEWithInitial(const uint32_t initial, const void* pBuffer, const size_t byteCount);

// IEEE 802.3 CRC-32 (zero initial value)
uint32_t crc32IEEE(const void* pBuffer, const size_t byteCount);

#ifdef __cplusplus
} // extern "C"
#endif
