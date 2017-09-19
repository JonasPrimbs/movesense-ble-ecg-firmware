#ifndef FIXEDMATH_H_
#define FIXEDMATH_H_
/****************************************************************************

        Copyright (c) Suunto Oy 2009.
        All rights reserved.

        $Id$

****************************************************************************/
/**
   @file fixedMath.h
   Fixed point math with exponent
*/

#ifndef GENDEF_H_
#include "common/compiler/genDef.h"
#endif
#include "common/compiler/genType.h"

/**
   Fixed point value with exponent
   Conversion to floating point is = value * 2^pow
 */
typedef struct FixedMathValue
{
    int16 value;
    int16 pow;
} FixedMathValue;

/**
   Method to set int16 value to object
 */
#define fixedMathSetInt16(OBJ, VALUE, DECIMALS)                                                                                  \
    (OBJ).value = VALUE;                                                                                                         \
    (OBJ).pow = DECIMALS

/**
   Method to shift value to left
 */
#define fixedMathShiftLeft(OBJ, BITS) (OBJ).pow += BITS

/**
   Method to shift value to right
 */
#define fixedMathShiftRight(OBJ, BITS) (OBJ).pow -= BITS

/**
   Set 32 bit value to object

   @param pObj		Pointer to object
   @param value		Value to set
   @param decimals	Decimal point to set
 */
extern void fixedMathSetInt32(FixedMathValue* pObj, int32 value, int16 decimals);

/**
   Get bits needed for store value without pow

   @param pObj		Pointer to object
   @param decimals	Output decimal point

   @return int16	Bits needed
 */
extern int16 fixedMathGetBits(const FixedMathValue* pObj, int16 decimals);

/**
   Get 16 bit value from object

   @param pObj		Pointer to object
   @param decimals	Output decimal point

   @return int16	16 bit value
 */
extern int16 fixedMathGetInt16(const FixedMathValue* pObj, int16 decimals);

/**
   Get 32 bit value from object

   @param pObj		Pointer to object
   @param decimals	Output decimal point

   @return int32	32 bit value
 */
extern int32 fixedMathGetInt32(const FixedMathValue* pObj, int16 decimals);

/**
   Add two values (output is target += source)

   @param pTarget	Pointer to target object
   @param pSource	Pointer to source object
 */
extern void fixedMathAdd(FixedMathValue* pTarget, const FixedMathValue* pSource);

/**
   Subtract two values (output is target -= source)

   @param pTarget	Pointer to target object
   @param pSource	Pointer to source object
 */
extern void fixedMathSub(FixedMathValue* pTarget, const FixedMathValue* pSource);

/**
   Multiply two objects (output is target *= source)

   @param pTarget	Pointer to target object
   @param pSource	Pointer to source object
 */
extern void fixedMathMul(FixedMathValue* pTarget, const FixedMathValue* pSource);

/**
   Divide two objects (output is target /= source)

   @param pTarget	Pointer to target object
   @param pSource	Pointer to source object
 */
extern void fixedMathDiv(FixedMathValue* pTarget, const FixedMathValue* pSource);

/**
   Take power from object

   @param pObj		Pointer to object
   @param n			Nth power (valid >= 0)
 */
extern void fixedMathPow(FixedMathValue* pObj, uint8 n);

/**
   Take root from object

   @param pObj		Pointer to object
   @param n			Nth root (valid range 2...4)
                                        Look details from mathRoot function
 */
extern void fixedMathRoot(FixedMathValue* pObj, uint8 n);

#endif // FIXEDMATH_H_
