#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

/**
    Safe string operations
*/

// Secure string operations are defined in C11 Annex K which support is optional
// Visual C++ compiler doesn't define standard identification macros eventhough safe string 
// operation are supported.
#ifndef __IAR_SYSTEMS_ICC__
#if (defined(__STDC_LIB_EXT1__) && (__STDC_LIB_EXT1__ >= 201112L)) || defined(_MSC_VER)
    #define WB_HAVE_STDC_LIB_EXT1 1
    #define __STDC_WANT_LIB_EXT1__ 1 /* Want the ext1 functions */
#endif
#endif

#include <string.h>
#include <stdio.h>

#include "whiteboard/integration/port.h"

#if !defined(WB_HAVE_STDC_LIB_EXT1)

/** If not C++11 standard safe string functions are not supported we use quick and dirty wrappers to dangerous
 * functions. If we ever need standard like behaviour for these functions then we could use Safe C library
 * (https://sourceforge.net/projects/safeclib). */

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

inline int strcpy_s(char* dest, size_t destsz, const char* src)
{
    const size_t toCopy = strlen(src);

    WB_ASSERT(destsz >= toCopy + 1);
    memcpy(dest, src, toCopy);
    dest[toCopy] = 0;
    return 0;
}

template <size_t SIZE>
inline int strcpy_s(char(&dest)[SIZE], const char* src)
{
    return strcpy_s(dest, SIZE, src);
}

inline int strncpy_s(char* dest, size_t destsz, const char* src, size_t count)
{
    const size_t srcLen = strlen(src);
    size_t toCopy = (count < srcLen) ? count : srcLen;
    if (count == _TRUNCATE)
    {
        if (toCopy + 1 > destsz)
        {
            toCopy = destsz - 1;
        }
    }
    else
    {
        WB_ASSERT(destsz >= toCopy + 1);
    }

    memcpy(dest, src, toCopy);
    dest[toCopy] = 0;
    return 0;
}

template <size_t SIZE>
inline int strncpy_s(char(&dest)[SIZE], const char* src, size_t count)
{
    return strncpy_s(dest, SIZE, src, count);
}

inline int strcat_s(char* dest, size_t destsz, const char* src)
{
    const size_t destOffset = strlen(dest);
    const size_t toCopy = strlen(src);

    WB_ASSERT(destsz >= destOffset + toCopy + 1);
    memcpy(&dest[destOffset], src, toCopy);
    dest[destOffset + toCopy] = 0;
    return 0;
}

template <size_t SIZE>
inline int strcat_s(char(&dest)[SIZE], const char* src)
{
    return strcat_s(dest, SIZE, src);
}

#define sscanf_s sscanf
#define sprintf_s sprintf

#endif
