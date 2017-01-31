/******************************************************************************

        Copyright (c) Suunto Oy 2016.
        All rights reserved.

******************************************************************************/
#pragma once

#include "common/compiler/genDef.h"

/** @fn size_t binToHexStr(void* pDst, size_t size, const void* pSrc, size_t count)
        Function binToHexStr.

        Function binToHexStr creates a hex string representation of the given
        binary data. The string will be NULL terminated.

        @param pDst A buffer to store the string into.
        @param size Size of the buffer.
        @param pSrc A buffer of the input data.
        @param count Number of bytes in the input data.

        @return Number of characters written, not counting the NULL character.
*/
C_API size_t binToHexStr(void* pDst, size_t size, const void* pSrc, size_t count);

/** @fn size_t hexStrToBin(void* pDst, size_t size, const void* pSrc)
        Function hexStrToBin.

        Function hexStrToBin parses a hex string representation into binary data.

        @param pDst A buffer to store the data into.
        @param size Size of the buffer.
        @param pSrc Number of characters to parse.

        @return Number of bytes written.
*/
C_API size_t hexStrToBin(void* pDst, size_t size, const void* pSrc);
