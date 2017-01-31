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
class StringHelper
{
public:
    /**
        Converts a single character to lower case if input is in upper case.
        @param c Character to convert.
        @return Lower case letter or original input if it was not an upper case letter.
    */
    static inline char caseLow(char c)
    {
        if ((c >= 'A') && (c <= 'Z')) // do we have an upper case letter
        {
            c |= 1 << 5; // bit 5 indicates low case, set it
        }
        return c;
    }

    /**
        Converts a single character to upper case if input is in lower case.
        @param c Character to convert.
        @return Upper case letter or original input if it was not an lower case letter.
    */
    static inline char caseUp(char c)
    {
        if ((c >= 'a') && (c <= 'z')) // do we have an low case letter
        {
            c &= ~(1 << 5); // bit 5 indicates low case, clear it
        }
        return c;
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
