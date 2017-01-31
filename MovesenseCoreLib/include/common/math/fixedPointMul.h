#ifndef FIXEDPOINTMUL_H_
#define FIXEDPOINTMUL_H_
/****************************************************************************

        Copyright (c) Suunto Oy 2007.
        All rights reserved.

        $Id$

****************************************************************************/
/**
   @file fixedPointMul.h
   Fixed point multiply implementation.
*/

#ifndef GENDEF_H_
#include "common/compiler/genDef.h"
#endif
#include "common/compiler/genType.h"

/**
        Fixed point mul will calculate (a * b) >> decimalBits.
        Difference to the normal operation is that (a * b) can be bigger that
        2^32. Calculation accuracy will maintain if result fit in 32 bits.

        @param a				value to mul
        @param b				value to mul
        @param decimalBits	decimal bits used (good value is example near of
                            16)

        @return int32		calculation result
 */
C_API int32 fixedPointMul(int32 a, int32 b, uint8 decimalBits);

#endif
