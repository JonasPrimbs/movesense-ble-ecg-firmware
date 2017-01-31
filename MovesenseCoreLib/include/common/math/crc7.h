/******************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

        $Id$

******************************************************************************/
/**
        @file crc7.h
*/

#ifndef CRC7_H_
#define CRC7_H_

#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"

/*
 * Calculates crc7 (x^7+x^3+1) from data buffer with the given initializer
 */
C_API uint8 crc7(uint8 init, const void* pData, uint8 size);

#endif /* CRC7_H_ */
