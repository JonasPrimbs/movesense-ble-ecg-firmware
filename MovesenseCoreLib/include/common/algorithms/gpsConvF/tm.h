/*========================================================================
   
    Copyright (C) Fastrax Ltd.

    This source file is property of Fastrax Ltd. and may not be
    copied, modified or distributed without a valid iSuite SDK Lisence or 
    other written permission from the copyright holder.

    Contact information:
    --------------------  

    email: swsup@fastrax.fi

 =========================================================================
    
    Module Desciption:  LLa - transverse mercator conversion functions

  
 =========================================================================
 *
 *
 * $Revision:   1.0  $
 *
 * $Date:   16 Jun 2003 17:10:32  $
 *
 * $Logfile:   W:/customers/archives/Suunto/grid/tm.h-arc  $
 *
 * $Author:   kh  $
 *
 *
 =========================================================================
*/

#ifndef TM_H
#define TM_H

#include "fstypes.h"
#include "datum.h"
#include "grid.h"


WORD FAR_CODE TM_ChangeCentralMeridian(DOUBLE dCentralMeridianInRad);

void FAR_CODE TM_ChangeFalseNorthing(DOUBLE dFalseNorthingNew);

WORD FAR_CODE TM_SetParams(const PROJ_PARAMS __far* pTmParams,
                           const ELLIPSOID __far* pEllipsoid);
/*
 * The function TM_SetParams receives the ellipsoid
 * parameters and Tranverse Mercator projection parameters as inputs, and
 * sets the corresponding state variables. If any errors occur, the error
 * code(s) are returned by the function, otherwise TM_NO_ERROR is
 * returned.
 */


const PROJ_PARAMS __far* FAR_CODE TM_GetCurrentParams(void);
/*
 * The function TM_GetCurrentParams returns the current
 * ellipsoid and Transverse Mercator projection parameters.
 */


WORD FAR_CODE TM_LLtoXY( DOUBLE Latitude,
                         DOUBLE Longitude,
                         DOUBLE *Easting,
                         DOUBLE *Northing);

/*
 * The function TM_LL2TM converts geodetic
 * (latitude and longitude) coordinates to Transverse Mercator projection
 * (easting and northing) coordinates, according to the current ellipsoid
 * and Transverse Mercator projection coordinates.  If any errors occur, the
 * error code(s) are returned by the function, otherwise TM_NO_ERROR is
 * returned.
 *
 *    Latitude      : Latitude in radians                         (input)
 *    Longitude     : Longitude in radians                        (input)
 *    Easting       : Easting/X in meters                         (output)
 *    Northing      : Northing/Y in meters                        (output)
 */


WORD FAR_CODE TM_XYtoLL(const DOUBLE Easting,
                        const DOUBLE Northing,
                              DOUBLE *Latitude,
                              DOUBLE *Longitude);

/*
 * The function Convert_Transverse_Mercator_To_Geodetic converts Transverse
 * Mercator projection (easting and northing) coordinates to geodetic
 * (latitude and longitude) coordinates, according to the current ellipsoid
 * and Transverse Mercator projection parameters.  If any errors occur, the
 * error code(s) are returned by the function, otherwise TM_NO_ERROR is
 * returned.
 *
 *    Easting       : Easting/X in meters                         (input)
 *    Northing      : Northing/Y in meters                        (input)
 *    Latitude      : Latitude in radians                         (output)
 *    Longitude     : Longitude in radians                        (output)
 */


#endif /* TM_H */
