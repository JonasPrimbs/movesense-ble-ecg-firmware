#ifndef FIXEDPOINTDIV_H_
#define FIXEDPOINTDIV_H_
/****************************************************************************

        Copyright (c) Suunto Oy 2007.
        All rights reserved.

        $Id$

****************************************************************************/
/**
   @file fixedPointDiv.h
   Fixed point divide implementation.
*/

#ifndef GENDEF_H_
#include "common/compiler/genDef.h"
#endif
#include "common/compiler/genType.h"

/**
        Fixed point div will calculate (a / b) << decimalBits. Difference to
        the normal operation is that "a" could be smaller that than "b" and
        result is not zero. Also both values could be small or big.
        Calculation accuracy will drop if "a" is much bigger that "b" or
        otherround.

        Example:
                fixedPointDiv(1, 100, 16)	= 1/100 * 2^16	= 655
                fixedPointDiv(1000000, 100000000, 16)		= 655
                fixedPointDiv(10, 3, 16)	= 10/3 * 2^16	= 218453

        @param a 			numerator
        @param b				divisor
        @param decimalBits	decimal bits used (good value is example near of
                            16)

        @return int32		calculation result
*/
C_API int32 fixedPointDiv(int32 a, int32 b, uint8 decimalBits);

#endif // FIXEDPOINTDIV_H_
