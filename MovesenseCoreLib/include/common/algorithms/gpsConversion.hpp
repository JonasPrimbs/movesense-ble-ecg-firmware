/******************************************************************************

        Copyright (c) Suunto Oy 2012-2016.
        All rights reserved.

******************************************************************************/
#pragma once

#include "common/compiler/genType.h"

enum CoordFmt_e {
    CF_HDDD_DDDDD,     // 0 = Hddd.ddddd°
    CF_HDDD_MM_MMM,    // 1 = Hddd°mm.mmm'
    CF_HDDD_MM_SS_S,   // 2 = Hddd°mm'ss.s"
    CF_UTM,            // 3 = UTM
    CF_MGRS,           // 4 = MGRS
    CF_BNG,            // 5 = BNG
    CF_ETRS89_TM35FIN, // 6 = ETRS89_TM35FIN
    CF_KKJ27,          // 7 = KKJ27
    CF_ING,            // 8 = IG
    CF_RT90,           // 9 = RT90
    CF_SWEREF99TM,     // 10 = SWEREF99TM
    CF_CH1903,         // 11 = CH1903
    CF_NAD_27_ALASKA,  // 12 = NAD_27 Alaska // UTM with special datum
    CF_NAD_27_CONUS,   // 13 = NAD_27 Conus // UTM with special datum
    CF_NAD_83,         // 14 = NAD_83
    CF_NZTM2000        // 15 = NZTM2000
};

void gpsCoordinateGetDefaultLocation(CoordFmt_e fmt, int32& lat, int32& lon);

const char* gpsCoordinateGetFormatString(CoordFmt_e fmt);

bool gpsCoordinateFormat(CoordFmt_e fmt, int32 lat, int32 lon, char* title, char* line1, char* line2);
