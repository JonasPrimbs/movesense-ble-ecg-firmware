#ifndef UINTTOSTR_H_
#define UINTTOSTR_H_

/******************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

        $Id$

******************************************************************************/

/** @file uintToStr.h
        Function uintToStr.
*/

#ifndef GENDEF_H_
#include "common/compiler/genDef.h"
#endif
#include "common/compiler/genType.h"

/** @fn int uintToStr(char * str, uint32 val, int width, char fill)
        Function uintToStr.

        Function uintToStr translates unsigned integer value to string representation.

        Example:

        @code

        char str[20];

        uintToStr(str, 123, 0, 0);    // -> str: "123".
        uintToStr(str, 123, 4, '0'); // -> str: "0123".

        @endcode


        @param str A string buffer big enough.
        @param val An unsigned integer.
        @param width A width; minimum width of string.
        @param fill A fill character used to pad string to given width.

        @return length of output string.
*/
C_API int uintToStr(char* str, uint32 val, int width, char fill);

#endif // #ifndef UINTTOSTR_H_
