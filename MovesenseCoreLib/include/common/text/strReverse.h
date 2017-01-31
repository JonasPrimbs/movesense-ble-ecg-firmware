#ifndef STRREVERSE_H_
#define STRREVERSE_H_

/******************************************************************************

        Copyright (c) Suunto Oy 2011.
        All rights reserved.

        $Id$

******************************************************************************/

/** @file strReverse.h
        Function strReverse.
*/

#include "common/compiler/genDef.h"

/**	@fn strReverse(char * str)
        Reverse character order in a zero-termited string so
        that first character swaps with the last etc...

        Example: "abcd" -> "dcba"

        @param str A string.

        @return int, length of string.
*/
C_API int strReverse(char* str);

#endif // #ifndef STRREVERSE_H_
