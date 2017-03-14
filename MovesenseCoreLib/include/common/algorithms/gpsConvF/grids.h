/*========================================================================
   
    Copyright (C) Fastrax Ltd.

    This source file is property of Fastrax Ltd. and may not be
    copied, modified or distributed without a valid iSuite SDK Lisence or 
    other written permission from the copyright holder.

    Contact information:
    --------------------  

    email: swsup@fastrax.fi

 =========================================================================
    
    Module Desciption:  definitions for local grids

  
 =========================================================================
 *
 *
 * $Revision:   1.11  $
 *
 * $Date:   05 Apr 2005 13:27:42  $
 *
 * $Logfile:   W:/customers/archives/Suunto/grid/grids.h-arc  $
 *
 * $Author:   kh  $
 *
 *
 =========================================================================
*/
#include "grid.h"
#include "datumids.h"


// Supported projection methods:
// - Transverse Mercator
// - Swiss Oblique Mercator
// - Stereographic
// - Double Stereographic (Schreiber double projection)
typedef enum
{
    TM, 
    SWISS_OM, 
#ifdef STEREO_ENABLED
    STEREO,
    DBL_ST,
#endif
    NR_PROJECTIONS
} PROJECTION_METHOD;

#define PROJECTION_METHOD_LAST (NR_PROJECTIONS - 1)

/*
const __far ELLIPSOID g_ellipsoids[] =
{
    {6378137.0,   298.257222101, 0.0, 0.0},  // GRS80
    {6378388.0,   297.0,         0.0, 0.0},  // Finland - Hayford (International 1924)
    {6377563.396, 299.3249646,   0.0, 0.0},  // Airy
    {6377397.155, 299.1528128,   0.0, 0.0},  // Bessel 1841
    {6377340.189, 299.3249646,   0.0, 0.0},  // Modified Airy
    {6378160.0,   298.247167427, 0.0, 0.0},  // GRS67
    {6378137.0,   298.257223563, 0.0, 0.0},  // WGS84
    {6378160.0,   298.25,        0.0, 0.0},  // Australian National (AN)
    {6378206.4,   294.9786982,   0.0, 0.0}   // Clarke 1866 (CC) 
};
*/



