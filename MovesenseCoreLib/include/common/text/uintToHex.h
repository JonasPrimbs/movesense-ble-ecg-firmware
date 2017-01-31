#pragma once
#include "common/compiler/genDef.h"

/** @fn int uintToHex(char * str, uint32 val, int width, char fill)
        Function uintToHex.

        Function uintToHex translates unsigned integer value to hexadecimal representation.

        Example:

        @code

        char str[20];

        uintToHex(str, 123, 0, 0);    // -> str: "7B".
        uintToHex(str, 123, 4, '0'); // -> str: "007B".

        @endcode


        @param str A string buffer big enough.
        @param val An unsigned integer.
        @param width A width; minimum width of string.
        @param fill A fill character used to fill string to given width.

        @return length of output string.
*/
C_API int uintToHex(char* str, size_t val, int width, char fill);
