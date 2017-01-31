/******************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

        $Id$

******************************************************************************/
/**
        @file crc16.h
*/

#ifndef CRC16_H_
#define CRC16_H_

#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"

/*
 * Calculates crc16 ccitt from data buffer. uses 0x0000 as initial value
 */
C_API uint16 crc16(const void* pData, uint32 size);

/*
 * Calculates crc16 ccitt from data buffer. uses 0xFFFF as initial value
 */
C_API uint16 crc16_ccitt(const void* pData, uint32 size);

/*
 * single step crc16 ccitt (x^16+x^12+x^5+1)
 */
C_API uint16 crc16Add(uint16 crc, uint8 data);

#endif /* CRC16_H_ */
