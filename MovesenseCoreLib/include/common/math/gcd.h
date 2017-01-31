#pragma once
/****************************************************************************

        Copyright (c) Suunto Oy 2016.
        All rights reserved.

****************************************************************************/

#include "common/compiler/genDef.h" // genType & C_API
#include "common/compiler/genType.h"

/**
 * Greatest common divisor
 */
C_API uint32 gcd(uint32 a, uint32 b);
