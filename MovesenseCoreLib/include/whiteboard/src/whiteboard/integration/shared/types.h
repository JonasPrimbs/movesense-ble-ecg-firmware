#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/
/**
    Generic type definitions.
*/

namespace whiteboard
{
}

namespace wb = whiteboard;

#include <stddef.h>
#include <stdint.h>
#include "whiteboard/integration/shared/macros.h"

/**
    General types:
*/
typedef uint8_t byte;
typedef uint16_t word;

typedef int8_t int8;   /**< 8-bit integer type. */
typedef uint8_t uint8; /**< 8-bit unsigned integer type. */

typedef int16_t int16;   /**< 16-bit integer type. */
typedef uint16_t uint16; /**< 16-bit unsigned integer type. */

#if (defined(__GNUC__) && defined(__arm__) && !defined(__LP64__)) || \
    (defined(__clang__) && defined(__hexagon__))
/* 32-bit ARM and clang hexagon compilers define int32_t as long 
 * instead of int, which causes all manner of chaos later on when regular
 * ints won't match to int32 or uint32. Special define that makes sure
 * regular int is covered.
 */
typedef int          int32;   /**< 32-bit integer type. */
typedef unsigned int uint32;  /**< 32-bit unsigned integer type. */
WB_STATIC_VERIFY(sizeof(int) == 4, SizeOf32BitIntIsNotWhatExpected);
#else
typedef int32_t int32;   /**< 32-bit integer type. */
typedef uint32_t uint32; /**< 32-bit unsigned integer type. */
#endif

typedef int64_t int64;   /**< 64-bit integer type. */
typedef uint64_t uint64; /**< 64-bit unsigned integer type. */

typedef unsigned int uint;

#ifdef ANDROID // Needed for Android only
#include <limits>
#ifdef max
#undef max
#endif
#ifndef UINT32_MAX
#define UINT32_MAX  std::numeric_limits<uint32_t>::max()
#endif
#ifndef INT32_MAX
#define INT32_MAX   std::numeric_limits<int32_t>::max()
#endif
#ifndef INT32_MIN
#define INT32_MIN   std::numeric_limits<int32_t>::min()
#endif
#ifndef UINT8_MAX
#define UINT8_MAX   std::numeric_limits<uint8_t>::max()
#endif
#endif
