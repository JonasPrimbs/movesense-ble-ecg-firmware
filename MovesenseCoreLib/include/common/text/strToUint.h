/******************************************************************************

        Copyright (c) Suunto Oy 2016.
        All rights reserved.

******************************************************************************/
#pragma once

#include "common/compiler/genDef.h"

/** @fn size_t strToUint(const void* pData, size_t size)
        Function strToUint.

        Function strToUint parses a string representation of an unsigned
        interger and returns the corresponding unsigned integer value. Up
        to size characters of the string are parsed. If the string contains
        non-digit characters, parsing is terminated and the result so far
        is returned.

        Example:

        @code

        strToUint("1234", 2);    // -> 12
        strToUint("1234", 4);    // -> 1234
        strToUint("1234", 6);    // -> 1234
        strToUint("123456X", 8); // -> 123456

        @endcode

        @param pData A buffer to parse the string from.
        @param size Number of characters to parse.

        @return Unsigned integer value.
*/
C_API size_t strToUint(const void* pData, size_t size);
