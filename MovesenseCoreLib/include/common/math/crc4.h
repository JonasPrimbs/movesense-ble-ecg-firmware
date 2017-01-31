#ifndef CRC4_H_
#define CRC4_H_
/******************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

        $Id$

******************************************************************************/
/**
        \file crc4.h
        CRC-4 algorithm from Measurement Specialties application note AN520.
*/

#ifndef GENDEF_H_
#include "common/compiler/genDef.h"
#endif
#include "common/compiler/genType.h"

/**
        Calculate CRC-4 code.
        \return crc code
*/
C_API uint8 crc4CheckSum(uint16* n_prom);

#endif
