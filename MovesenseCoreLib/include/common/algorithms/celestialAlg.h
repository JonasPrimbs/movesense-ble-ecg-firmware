/******************************************************************************

        Copyright (c) Suunto Oy 2013-2017.
        All rights reserved.

******************************************************************************/
#ifndef CELESTIALALG_H_
#define CELESTIALALG_H_

#include "common/compiler/genType.h"
#include "common/time/date.h"

#define CA_TIME_FOUND 0U
#define CA_NO_DAYLIGHT 1U
#define CA_NO_NIGHT 2U

#define CA_SUNRISE_SUNSET 0
#define CA_CIVIL_TWILIGHT 1
#define CA_NAUTI_TWILIGHT 2
#define CA_ASTRO_TWILIGHT 3
#define CA_ZENITH_COUNT 4

typedef struct _SunInfo {
    float latitude;                 // in decimal degrees
    float longitude;                // in decimal degrees
    struct Date date;               // year, month, day
    int16 dawn[CA_ZENITH_COUNT];    // in minutes, over 1440 next day, below zero offset backwards from midnight
    int16 dusk[CA_ZENITH_COUNT];    // in minutes, over 1440 next day, below zero offset backwards from midnight
    uint8 status[CA_ZENITH_COUNT];  // CA_TIME_FOUND, CA_NO_DAYLIGHT, CA_NO_NIGHT
} SunInfo;

#ifdef __cplusplus
extern "C" {
#endif

void celestialGetSunRiseSetTimes(SunInfo* pSunInfo);

#ifdef __cplusplus
}
#endif

#endif // CELESTIALALG_H_
