#pragma once
#include <stdint.h>

/** @fn int valueToStr(char * str, int32 val, int scale)
        Function valueToStr.
        Print a scaled integer value to string.

        Example:

        @code

        char text[20];

        valueToStr(text, 123, 0);	// -> text: "123".
        valueToStr(text, 123, 1);	// -> text: "12.3".
        valueToStr(text, 123, 2);	// -> text: "1.23".
        valueToStr(text, 123, 3);	// -> text: "0.123".

        @endcode

        @param str A string big enough,
        @param val A scaled integer value.
        @param scale A scale value.

        @return length of output string.
*/
int valueToStr(char* str, int32_t val, int scale);
