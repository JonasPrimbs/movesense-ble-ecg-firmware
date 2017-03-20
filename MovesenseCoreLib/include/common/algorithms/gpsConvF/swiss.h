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
 * $Revision:   1.1  $
 *
 * $Date:   04 Jul 2003 11:53:50  $
 *
 * $Logfile:   W:/customers/archives/Suunto/grid/swiss.h-arc  $
 *
 * $Author:   kh  $
 *
 *
 =========================================================================
*/
#ifndef _SWISS_H_
#define _SWISS_H_

#include "fstypes.h"
#include "grid.h"
#include "datum.h"


WORD FAR_CODE swissSetParams(const PROJ_PARAMS __far* pTmParams,
                             const ELLIPSOID __far* pEllipsoid);
/*
 * The function swissSetParams receives the ellipsoid
 * parameters and Tranverse Mercator projection parameters as inputs, and
 * sets the corresponding state variables. If any errors occur, the error
 * code(s) are returned by the function, otherwise TM_NO_ERROR is
 * returned.
 */

const PROJ_PARAMS __far* FAR_CODE swissGetCurrentParams(void);
/*
 * The function TM_GetCurrentParams returns the current
 * ellipsoid and Transverse Mercator projection parameters.
 */

WORD FAR_CODE swissLLtoXY(const DOUBLE Lat,
                          const DOUBLE Long, 
			                    DOUBLE* SwissEasting, 
                                DOUBLE* SwissNorthing);

WORD FAR_CODE swissXYtoLL(const DOUBLE SwissEasting, 
                          const DOUBLE SwissNorthing, 
					            DOUBLE* Lat, 
                                DOUBLE* Long);

#endif
