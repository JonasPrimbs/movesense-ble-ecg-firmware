/******************************************************************************

Copyright (c) Suunto Oy 2017.
All rights reserved.

******************************************************************************/
/**
* @file Utf8.hpp
*
* Functions for decoding utf8 encoded strings
*
* @note
* - supports 2003 standard (max 21 bits encoded to max 4 byte long sequence)
* - utf-8 decoding example:
*     utf8* pString //contains utf-8 encoded string
*     while(*pString)
*     {
*        uint32 chr = utf8_chrdecode(pString);
*        pString += utf8_chrlen(pString);
*     }
*/

#pragma once
#include <stdint.h>
#include "common/core/dbgassert.h"

typedef uint8_t utf8;

/*
 * Helpers for internal use only!
 */
int utf8_get_encoded_chrlen_internal(const utf8* const pStr);
uint32_t utf8_chrdecode_encoded_internal(const utf8* const pStr);

/*
 * utf8_strlen returns number of characters in the string. normal strlen returns the number of bytes
 * in string
 */
int utf8_strlen(const utf8* pStr);

/*
 * utf8_strbytelen is analogous to strlen
 */
inline int utf8_strbytelen(const utf8* pStr)
{
    return (int)strlen((const char*)pStr);
}

/*
 * utf8_chrlen returns number of bytes (1-4) in utf8 encoded character
 */
inline int utf8_chrlen(const utf8* const pStr)
{
    // handle the most common directly
    DEBUG_ASSERT(pStr);
    return *pStr < 0x80 ? 1 : utf8_get_encoded_chrlen_internal(pStr);
}

/*
 * decodes utf8 character into 32bit unicode
 */
inline uint32_t utf8_chrdecode(const utf8* const pStr)
{
    // handle the most common directly
    DEBUG_ASSERT(pStr);
    return *pStr < 0x80 ? *pStr : utf8_chrdecode_encoded_internal(pStr);
}

/*
 * decodes utf8 character into 32bit unicode and advances input pointer
 */
template <typename T> inline uint32_t utf8_chrdecode_and_advance(T*& pStr)
{
    // handle the most common directly
    DEBUG_ASSERT(pStr);
    const uint32_t firstByte = *reinterpret_cast<const uint8_t*>(pStr);
    if (firstByte < 0x80)
    {
        pStr = reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(pStr) + 1);
        return firstByte;
    }
    const int len = utf8_get_encoded_chrlen_internal(reinterpret_cast<const utf8*>(pStr));
    const uint32_t val = utf8_chrdecode_encoded_internal(reinterpret_cast<const utf8*>(pStr));
    pStr = reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(pStr) + len);
    return val;
}

/*
 * Check length of utf-8 encoded string truncated to fit to given
 * maximum byte length without breaking characters. Returns
 * number of bytes in truncated string without terminating null
 * character.
 */
int utf8_truncatelen(const utf8* pStr, const int maxByteLen);
