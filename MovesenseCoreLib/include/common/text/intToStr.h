#ifndef INTTOSTR_H_
#define INTTOSTR_H_

/******************************************************************************

        Copyright (c) Suunto Oy 2011.
        All rights reserved.

        $Id$

******************************************************************************/

/** @file intToStr.h
        Function intToStr.
*/

#ifndef GENDEF_H_
#include "common/compiler/genDef.h"
#endif
#include "common/compiler/genType.h"

/** @fn int intToStr(char * str, int32 val, int width, char fill)
        Function intToStr.

        Function intToStr translates integer value to string representation.

        Example:

        @code

        char str[20];

        intToStr(str, 123, 0, 0);    // -> str: "123".
        intToStr(str, 123, 4, '0'); // -> str: "0123".

        @endcode


        @param str A string buffer big enough.
        @param val An integer.
        @param width A width; minimum width of string.
        @param fill A fill character used to fill string to given width.

        @return length of output string.
*/
C_API int intToStr(char* str, int32 val, int width, char fill);

#endif // #ifndef INTTOSTR_H_
