#ifndef MATHROOT_H_
#define MATHROOT_H_
/****************************************************************************

        Copyright (c) Suunto Oy 2009.
        All rights reserved.

        $Id$

****************************************************************************/
/**
   @file mathRoot.h
   Calculate root
*/

#ifndef GENDEF_H_
#include "common/compiler/genDef.h"
#endif
#include "common/compiler/genType.h"

/**
   Calculate root a^(1/n)

   @param a			Value to calculate
   @param n			Nth root of 'a' i.e. result^n=a (valid range is 2,3 and 4)
                                        NOTE that fourth root is is not very accurate.

   @return uint16	Root result zero if invalid parameters
 */
extern uint16 mathRoot(uint32 a, uint8 n);

uint16 intSqrt32(uint32 x);

#endif // MATHROOT_H_
