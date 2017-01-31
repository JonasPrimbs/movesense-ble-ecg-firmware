/******************************************************************************

        Copyright (c) Suunto Oy 2013.
        All rights reserved.

        $Id: hash.h 75106 2015-04-07 14:51:37Z treyh $

******************************************************************************/
/**
        @file hash.h
*/

#ifndef HASH_H_
#define HASH_H_

#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"

// FNV hash, usable for short strings (5-128 chars)
C_API uint32 hashFnvStr(const char* pStr);
C_API uint32 hashFnvStrN(const char* pStr, size_t len);
C_API uint32 hashFnvAddStr(uint32 startValue, const char* pStr);
C_API uint32 hashFnvAddStrN(uint32 startValue, const char* pStr, size_t len);

// xxhash, faster than crc32
C_API uint32 hashXxStr(const char* pStr);
C_API uint32 hashXx(const void* pBuffer, uint32 size);

#endif /* HASH_H_ */
