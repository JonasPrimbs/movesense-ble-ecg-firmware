/******************************************************************************

        Copyright (c) Suunto Oy 2009.
        All rights reserved.

******************************************************************************/
/**
        \file coordinateDistanceAndBearingCalculate.h
*/

#ifndef COORDINATEDISTANCEANDBEARINGCALCULATE_H_
#define COORDINATEDISTANCEANDBEARINGCALCULATE_H_
#include "common/compiler/genType.h"

/* @TODO This is exact copy from gpsDef.h to avoid dependency, replace
   with gpsInterface when available */
typedef struct CdabLocation {
    int32 latitude;  // [deg],resolution 0.0000001,range -90 ... 90
    int32 longitude; // [deg],resolution 0.0000001,range -180 ... 180
} CdabLocation;

/**
        Calculate the distance from start point to end point, in cm.
        Calculate the (initial) bearing from start point to end point, in 0.1 degrees

        \param start, Coordinates of start point
        \param end, Coordinates of end point
        \return Initial bearing in 0.1 degrees from North
        \return Distance from start to end, in cm.
*/

#ifdef __cplusplus
extern "C" {
#endif

uint32 coordinateDistanceAndBearingCalculate(const CdabLocation* p1, const CdabLocation* p2,
                                             int16* pBearing);

#ifdef __cplusplus
}
#endif

#endif
