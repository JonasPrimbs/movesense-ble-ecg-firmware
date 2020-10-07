#ifndef INTUTIL_H_
#define INTUTIL_H_
/* Copyright (c) Suunto Oy 2003. All rights reserved. */

#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef bool boolean;

/**
        Increments the given variable by one, and if the hi-limit value is reached,
        wraps the value over to zero (0).

        \param	pV	Pointer to variable to increment
        \param	hiLimit	The high limit value.

        \return boolean TRUE in case of wraparound, otherwise returns FALSE.
*/
boolean uint8IncWrapped(uint8* pV, uint8 hiLimit);

/**
        Increments the given variable by one, and if the hi-limit value is reached,
        wraps the value over to low limit.

        \param	pV	Pointer to variable to increment
        \param  firstLow The first legal value.
        \param	hiLimit	The high limit value.

        \return boolean TRUE in case of wraparound, otherwise returns FALSE.
*/
boolean uint8IncWrappedLow(uint8* pV, uint8 firstLow, uint8 hiLimit);

/**
        Decrements the given variable by one, and if the zero value is passed,
        wraps the value back to the last value below the limit.

        \param	pV	Pointer to variable to increment
        \param	hiLimit	The high limit value.

        \return boolean TRUE in case of wraparound, otherwise returns FALSE.
*/
boolean uint8DecWrapped(uint8* pV, uint8 hiLimit);

/**
        Decrements the given variable by one, and if the given low value is passed,
        wraps the value back to the last value below the limit.

        \param	pV	Pointer to variable to increment
        \param  firstLow The first legal value.
        \param	hiLimit	The high limit value.

        \return boolean TRUE in case of wraparound, otherwise returns FALSE.
*/
boolean uint8DecWrappedLow(uint8* pV, uint8 firstLow, uint8 hiLimit);

/**
        Increments the given variable by one
        if it still stays below the the limit.

        \param	pV	Pointer to variable to increment
        \param	hiLimit	The high limit value.

        \return boolean TRUE when hitting against the upper limit
*/
boolean uint8IncLimited(uint8* pV, uint8 hiLimit);

/**
   Clips the given variable below the given limit.

        \param pV Pointer to variable to be checked
        \param hiLimit The high limit

        \return boolean TRUE if clipped.
        \return FALSE if there was no need to clip.
*/
boolean uint8Clip(uint8* pV, uint8 hiLimit);

/**
   Clips the given variable between the given limits.

        \param pV Pointer to variable to be checked
        \param firstLow The first legal value
        \param hiLimit The high limit

        \return boolean TRUE if clipped.
        \return FALSE if there was no need to clip.
*/
boolean uint8ClipBoth(uint8* pV, uint8 firstLow, uint8 hiLimit);

uint16 uint16Div16Round(const uint16 value, const uint16 divisor);
uint32 uint32Div32Round(const uint32 value, const uint32 divisor);
int32 int32Div32Round(const int32 value, const int32 divisor);

/**
        Calculation of scaled values
*/
int16 int16ScaledValue(int16 value, int16 scalerDividend, int16 scalerDivider);
int32 int32ScaledValue(int32 value, int32 scalerDividend, int32 scalerDivider);

#ifdef __cplusplus
}
#endif

#endif
