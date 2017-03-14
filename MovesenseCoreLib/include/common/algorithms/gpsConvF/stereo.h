/*========================================================================
   
    Copyright (C) Fastrax Ltd.

    This source file is property of Fastrax Ltd. and may not be
    copied, modified or distributed without a valid iSuite SDK Lisence or 
    other written permission from the copyright holder.

    Contact information:
    --------------------  

    email: swsup@fastrax.fi

 =========================================================================
    
    Module Desciption:  Swiss oblique mercator projection

  
 =========================================================================
 *
 *
 * $Revision:   1.0  $
 *
 * $Date:   16 Jun 2003 17:10:30  $
 *
 * $Logfile:   W:/customers/archives/Suunto/grid/stereo.h-arc  $
 *
 * $Author:   kh  $
 *
 *
 =========================================================================
*/
#ifndef _STEREO_H_
#define _STEREO_H_

#include "fstypes.h"
#include "grid.h"
#include "datum.h"

void FAR_CODE stereoSetConformalConversionOn(BOOL bOn);

WORD FAR_CODE stereoSetParams(const __far PROJ_PARAMS* pTmParams,
                              const __far ELLIPSOID* pEllipsoid);
/*
 * The function swissSetParams receives the ellipsoid
 * parameters and Tranverse Mercator projection parameters as inputs, and
 * sets the corresponding state variables. If any errors occur, the error
 * code(s) are returned by the function, otherwise TM_NO_ERROR is
 * returned.
 */

const __far PROJ_PARAMS* FAR_CODE stereoGetCurrentParams(void);
/*
 * The function TM_GetCurrentParams returns the current
 * ellipsoid and Transverse Mercator projection parameters.
 */

WORD FAR_CODE stereoLLtoXY(DOUBLE Latitude,
                           DOUBLE Longitude,
                           DOUBLE *Easting,
                           DOUBLE *Northing);

WORD FAR_CODE stereoXYtoLL(const DOUBLE Easting,
                           const DOUBLE Northing,
                                 DOUBLE *Latitude,
                                 DOUBLE *Longitude);

#endif
