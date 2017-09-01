/******************************************************************************

        Copyright (c) Suunto Oy 2003.
        All rights reserved.

        $Id$

        Description:  Calculate:  e^(-X)

--------------------------------------------------------------------------------

        Inputs:	 Exponent X [e^(-X)], X is between (0 - 0xFFFF).
                     0x0001 vastaa arvoa 0.00024414 (0.001H) ja
                         0xFFFF vastaa arvoa 15.999756 (F.FFFH).

        Outputs: Value [e^(-X)] which is scaled so that, if expression
                 [e^(-X)] = 0.713495, so Outputvalue is
                         0.713495 * 65536 = 33362 = 0x8252.

                         0x0001 (.0001) equals to 0.000015259 and
                         0xFFFF (.FFFF) equals to 0.9999847

        Notes: If input value > 0xC3FF (12.25), Outputvalue = 0.

******************************************************************************/

#ifndef EXPO_H
#define EXPO_H

/**
        \file expo.h
        Calculation of negative exponent e^(-X)
*/

/*--------- INCLUDE FILES ----------------------------*/
#ifndef GENDEF_H_
#include "common/compiler/genDef.h"
#endif
#include "common/compiler/genType.h"

/*--------- CONSTANTS --------------------------------*/

/*--------- TYPE DEFINITIONS OF THE DATA TYPES -------*/

/*--------- DATA STRUCTURES --------------------------*/

/*--------- VARIABLES --------------------------------*/

/*--------- MACROS -----------------------------------*/

/*--------- FUNCTIONS --------------------------------*/

/**
   Negative exponent can be calculated by negativeExponent(uint16 exponent).
   Return value is real result << 16

   \param exponent Input parameter << 12.
*/

uint16 negativeExponent(uint16 exponent);

enum
{
    NEG_EXP_INPUT_DECIMALS_BITS = 12
};
enum
{
    NEG_EXP_OUTPUT_DECIMALS_BITS = 16
};

#endif // Preventing multiple include of header
