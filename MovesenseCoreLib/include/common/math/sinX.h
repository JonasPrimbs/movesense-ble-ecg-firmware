#pragma once
#include <stdint.h>
/** @file sinX.h
        Simple trigonometry support functions.
*/

/** @fn int16 sinX(int16 hypotenuse, int16 angle)
        Calucates hypotenuse * sin(angle).

        @param hypotenuse A hypotenuse.
        @param and A angle in degrees.

        @return hypotenuse * sin(angle) (rounded).
*/
int16_t sinX(int16_t hypotenuse, int16_t angle);

/** @fn int16 cosX(int16 hypotenuse, int16 angle)
        Calucates hypotenuse * cos(angle).

        @param hypotenuse A hypotenuse.
        @param and A angle in degrees.

        @return hypotenuse * cos(angle) (rounded).
*/
int16_t cosX(int16_t hypotenuse, int16_t angle);
