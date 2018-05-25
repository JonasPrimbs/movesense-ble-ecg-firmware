#pragma once
/******************************************************************************
    Copyright © Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

namespace whiteboard
{

/**
    Simple helper functions for srting operations.

    These functions work correctly only in 7-bit ASCII charset and for example 'Ä'
    of ISO-8859-1 charset is not converted to 'ä'. The C standard setlocale function
    has no effect on these functions.
*/
class Ascii7String
{
public:

    /**
    Converts a single character to lower case if input is in upper case. Works also at compile time.
    @param c Character to convert.
    @return Lower case letter or original input if it was not an upper case letter.
    */
    static CONSTEXPR inline char caseLow(char c)
    {
        // do we have an upper case letter, if so bit 5 indicates case, set it
        return ((c >= 'A') && (c <= 'Z')) ? (c | (1 << 5)) : c;
    }

    /**
    Converts a single character to upper case if input is in lower case. Works also at compile time.
    @param c Character to convert.
    @return Upper case letter or original input if it was not an lower case letter.
    */
    static CONSTEXPR inline char caseUp(char c)
    {
        // do we have an low case letter, if so bit 5 indicates case, clear it
        return ((c >= 'a') && (c <= 'z')) ? (c & ~(1 << 5)) : c;
    }

    /**
        Compare two strings without case sensitivity.
        @param pStr1 Pointer to first string.
        @param pStr2 Pointer to second string.
        @param maxLen String length limit for compare.
        @return Delta of first encountered character inequality.
    */
    static int32 noCaseCmp(const char* pStr1, const char* pStr2, const size_t maxLen = static_cast<size_t>(-1));

    /**
        Convert a given input string to lower case.
        The input string will be modified.
        @param pStr Pointer to the string.
        @param maxLen Maximum count of characters to convert.
    */
    static void caseLowConvert(char* pStr, const size_t maxLen);

    /**
        Convert a given input string to upper case.
        The input string will be modified.
        @param pStr Pointer to the string.
        @param maxLen Maximum count of characters to convert.
    */
    static void caseUpConvert(char* pStr, const size_t maxLen);
};

} // namespace whiteboard
