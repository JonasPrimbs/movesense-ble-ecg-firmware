/******************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

        $Id$

******************************************************************************/
/**
        @file pi.h
*/
#ifndef PI_H_
#define PI_H_

#include "common/compiler/genDef.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.141592653f
#endif

#define DEG_TO_RAD(DEG) ((DEG) * (M_PI / 180))
#define RAD_TO_DEG(RAD) ((RAD) * (180 / M_PI))

#endif // PI_H_
