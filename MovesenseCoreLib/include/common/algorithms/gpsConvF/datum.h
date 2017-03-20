//============================================================
//
//    Copyright (C) Fastrax Oy
//
//    This source file is property of Fastrax and may not be
//    copied, modified of distributed without a written permission
//    from the copyright holder.
//
//    Contact information:
//    --------------------
//
//    email: support@fastrax.fi
//
//============================================================
/// \ingroup nav
/// \file
/// Datum conversion API.
///
// $Revision: 17458 $
//
//   $Author: mp $
//
//  $Logfile:   W:/iSuite03/archives/firmware/inc/datum.h-arc  $
//
//     $Date: 2007-03-19 11:31:40 +0200 (ma, 19 maalis 2007) $
//
//============================================================

#ifndef DATUM_H_INCLUDED
#define DATUM_H_INCLUDED

#include "geo_types.h"

/// A structure for defining an ellipsoid.
typedef struct {
    DOUBLE                      a;      ///< Major axis in meters.
    DOUBLE                      invf;   ///< inverse of flattening.
//    DOUBLE                      da;     ///< delta a from WGS84 ellipsoid.
//    DOUBLE                      df;     ///< delta f from WGS84 ellipsoid.
} ELLIPSOID;

/// A structure for defining a datum.
typedef struct {
    INT16                       dX;         ///< delta X.
    INT16                       dY;         ///< delta Y.
    INT16                       dZ;         ///< delta Z.
    ELLIPSOID                   ellipsoid; ///< Reference ellipsoid Id for this Datum.
} DATUM;


/// Convert geodetic WGS84 coordinates to
/// geodetic coordinates in requested local datum.
/// Note! Does not convert altitude.
WORD FAR_CODE DATUM_Wgs84ToLocal(LLA_POS *pLocal,         ///< LLA in local datum [radians].
                                  const LLA_POS *pWgs84,   ///< LLA in Wgs84 [radians].
                                  WORD wDatumId            ///< Id of local datum.
                                  );

/// Convert geodetic WGS84 coordinates to
/// geodetic coordinates with user given parameters.
/// Note! Does not convert altitude.
WORD FAR_CODE DATUM_Wgs84ToUser(LLA_POS *pLocal,                  ///< LLA in local datum [radians].
                                 const LLA_POS *pWgs84,            ///< LLA in Wgs84 [radians].
                                 const __far DATUM *pDatum,        ///< Datum parameter structure.
                                 const __far ELLIPSOID *pEllipsoid ///< Ellipsoid parameter structure.
                                 );

/// Convert geodetic coordinates to
/// geodetic coordinates in WGS84 with user given parameters.
/// Note! Does not convert altitude.
WORD FAR_CODE DATUM_UserToWgs84(LLA_POS *pWgs84,                  ///< LLA in WGS84 [radians].
                                 const LLA_POS *pLocal,            ///< LLA in local datum [radians].
                                 const __far DATUM *pDatum,        ///< Datum parameter structure.
                                 const __far ELLIPSOID *pEllipsoid ///< Ellipsoid parameter structure.
                                 );

/// Get the parameters for the requested datum.
WORD FAR_CODE DATUM_GetDatumParameters(register DATUM *pDatum,     ///< Pointer to structure for the parameters.
                                        WORD wDatumId               ///< Id of datum.
                                        );

/// Get the parameters for the requested ellipsoid.
WORD FAR_CODE DATUM_GetEllisoidParameters(register ELLIPSOID *pEllipsoid,  ///< Pointer to structure for the parameters.
                                           WORD wEllipsoidId                ///< Id of ellipsoid.
                                           );



#endif /* DATUM_H_INCLUDED */
