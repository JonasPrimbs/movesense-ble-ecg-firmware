#pragma once
/**
    @file genType.h
    General types.
*/

#include <stdint.h>

// Character definition (for MISRA-2008)
typedef char char_t;

/**
 * Sized integer types.
 */

typedef int8_t int8;   /**< 8-bit integer type. */
typedef uint8_t uint8; /**< 8-bit unsigned integer type. */

typedef int16_t int16;   /**< 16-bit integer type. */
typedef uint16_t uint16; /**< 16-bit unsigned integer type. */

#if defined(__GNUC__) && defined(__arm__) && !defined(__LP64__)
/* 32-bit ARM defines int32_t as long instead of int, which causes all manner
* of chaos later on when regular ints won't match to any of
* int8|int16|int32|int64. Have a special define for ARM that makes sure
* regular int is covered. */
typedef int int32;           /**< 32-bit integer type. */
typedef unsigned int uint32; /**< 32-bit unsigned integer type. */
#else
typedef int32_t int32;   /**< 32-bit integer type. */
typedef uint32_t uint32; /**< 32-bit unsigned integer type. */
#endif

typedef int64_t int64;   /**< 64-bit integer type. */
typedef uint64_t uint64; /**< 64-bit unsigned integer type. */

typedef unsigned int uint; // TODO: remove

#ifndef __cplusplus
// instead of defining true/false manually, use the proper C way
#include <stdbool.h>
#endif

/*
 * Just in case
 */
#ifndef NULL
#ifdef __cplusplus
#ifdef _MSC_VER
#define NULL nullptr
#else
#define NULL 0
#endif
#else
#define NULL ((void*)0)
#endif
#endif
