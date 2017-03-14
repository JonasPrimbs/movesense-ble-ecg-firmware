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
/// Common definitions for coordinate conversions.
/// 
/// This file provides the types and definitions which are used in
/// geodetic coordinate conversions.  There are mainly two kinds of
/// structures available for geodetic types. The floating-point types
/// such as XYZ_VECTOR and LLA_POS are used internally by iSuite. The
/// integer types INT_XYZ_VECTOR and INT_LLA_POS are used in some
/// iTalk message payloads, in order to allow communication with external
/// software with no floating point support.
/// 
/// The conversion functions are defined in the coord.h header file.
/// 
/// 
/// 
/// 
/// 
/// 
// $Revision: 17458 $
// 
//   $Author: mp $
// 
//  $Logfile:   W:/iSuite3/archives/firmware/inc/geo_types.h-arc  $
//
//     $Date: 2007-03-19 11:31:40 +0200 (ma, 19 maalis 2007) $
//
//============================================================

#ifndef GEO_TYPES_H_INCLUDED
#define GEO_TYPES_H_INCLUDED

#include "fstypes.h"

/// Dimension of coordinate transformation matrix.
#define N_TOPO_DIM 3

/// Vector in XYZ format.
typedef struct
{
    DOUBLE dX; ///< X coordinate.
    DOUBLE dY; ///< Y coordinate.
    DOUBLE dZ; ///< Z coordinate.
} XYZ_VECTOR;

/// Vector in LLA format, with the latitude and longitude expressed in
/// radians.
typedef struct
{
    DOUBLE dLat; ///< Latitude in radians.
    DOUBLE dLon; ///< Longitude in radians.
    DOUBLE dAlt; ///< Altitude in meters.
} LLA_POS;

/// Vector in ENU format.  If the vector is used for distance,
/// velocity or acceleration, then units or [m], [m/s] or [m/(s*s)]
/// are used, respectively.
typedef struct
{
    DOUBLE dEast;  ///< East component. 
    DOUBLE dNorth; ///< North component.
    DOUBLE dUp;    ///< Up component.
} ENU_VECTOR;

/// Vector in polar (Azi, Ele, Rho) format.
typedef struct
{
    DOUBLE dAzi; ///< Azimuth in radians. This is the eastwards angle from north.
    DOUBLE dEle; ///< Elevation in radians. This is the angle upwards from horizontal.
    DOUBLE dRho; ///< Distance in meters.
} POLAR_VECTOR;

/// Coordinates in grid format.
typedef struct
{
    DOUBLE dNorthing;      ///< Northwards distance of grid origin in meters.
    DOUBLE dEasting;       ///< Eastwards distance of grid origin in meters.
    DOUBLE dAlt;           ///< Altitude in meters.
    WORD   wGridId;        ///< Identifier of grid (e.g. UTM grid number).
    WORD   wCoordSystem;   ///< Grid system identifier (e.g. UTM, ...).
} GRID_COORD;

/// \name Conversion factors.
/// These definitions are used as conversion factors when converting from one unit to another.
//@{

/// Conversion factor for converting integer position to decimal.
#define CONV_COEFF_POSITION     0.01        

/// Conversion factor for converting integer velocity to decimal.
#define CONV_COEFF_VELOCITY     0.001     

/// Conversion factor for converting integer coordinate angle to floating point.
#define CONV_COEFF_COORD_ANGLE  1.0e-7

/// Conversion factor for converting integer altitude to floating point.
#define CONV_COEFF_COORD_ALT    0.001

/// Conversion factor from m/s to knots.
#define METERS_IN_NAUTICAL_MILE 1852


/// \name Conversion macros.
/// These macros are used to convert values from one unit to another.
//@{

/// Convert position coordinate values from integer to floating point.
#define CONV_INT_POSITION(x)    (CONV_COEFF_POSITION * (DOUBLE)(x))

/// Convert position coordinate values from floating point to integer.
#define CONV_TO_INT_POSITION(x)    ((INT32)((x>0)?((1.0 / CONV_COEFF_POSITION) * (DOUBLE)(x))+0.5:((1.0 / CONV_COEFF_POSITION) * (DOUBLE)(x))-0.5))

/// Convert velocity components from integer to floating point.
#define CONV_INT_VELOCITY(x)    (CONV_COEFF_VELOCITY * (DOUBLE)(x))

/// Convert velocity components from floating point to integer.
#define CONV_TO_INT_VELOCITY(x)     ((INT32)((x>0)?((1.0 / CONV_COEFF_VELOCITY) * (DOUBLE)(x))+0.5:((1.0 / CONV_COEFF_VELOCITY) * (DOUBLE)(x))-0.5))

/// Convert angles from integer to floating point.
#define CONV_INT_COORD_ANGLE(x) (CONV_COEFF_COORD_ANGLE * (DOUBLE)(x))

/// Convert angles from floating point to integer.
#define CONV_TO_INT_COORD_ANGLE(x)  ((INT32)((x>0)?((1.0 / CONV_COEFF_COORD_ANGLE) * (DOUBLE)(x))+0.5:((1.0 / CONV_COEFF_COORD_ANGLE) * (DOUBLE)(x))-0.5))

/// Convert altitude from integer to floating point.
#define CONV_INT_ALTITUDE(x)    (CONV_COEFF_COORD_ALT * (DOUBLE)(x))

/// Convert altitude from floating point to integer.
#define CONV_TO_INT_ALTITUDE(x)     ((INT32)((x>0)?((1.0 / CONV_COEFF_COORD_ALT) * (DOUBLE)(x))+0.5:((1.0 / CONV_COEFF_COORD_ALT) * (DOUBLE)(x))-0.5))

/// Convert m/s to knots.
#define CONV_MPS_TO_KNOTS(mps)  (mps*3600/METERS_IN_NAUTICAL_MILE)

/// Conversion factor from m/s to km/h.
#define CONV_MPS_TO_KPH(mps)    (3.6*mps)

//@}

/// Integer vector in XYZ format.
/// 
/// The coordinates are given in units of [cm] for position data, and
/// in units of [mm/s] for velocity.
typedef struct
{
    INT32 lX;  ///< X coordinate.
    INT32 lY;  ///< Y coordinate.
    INT32 lZ;  ///< Z coordinate.
} INT_XYZ_VECTOR;

/// Integer short vector in XYZ format
/// The coordinates are given units of [cm] for position data, and
/// in units of [mm/s] for velocity.
typedef struct
{
    INT16 iX;
    INT16 iY;
    INT16 iZ;
} SHORT_XYZ_VECTOR;

/// Vector in LLA integer format with the latitude and longitude
/// expressed in degrees.
typedef struct
{
    INT32 lLat;     ///< Latitude in units of 0.0000001 degrees.
    INT32 lLon;     ///< Longitude in units of 0.0000001 degrees.
    INT32 lAlt;     ///< Altitude in units of 0.001 meters.
} INT_LLA_POS;

/// Grid coordinates.
typedef struct
{
    INT32   lNorthing;      ///< Northing, cm's from coordinate system origin.
    INT32   lEasting;       ///< Easting, cm's from coordinate system origin.
    WORD    wGridId;        ///< Identifier of grid (e.g. UTM grid number).
    WORD    wCoordSystem;   ///< Grid system identifier (e.g. UTM, ...).
} INT_GRID_COORD;

#endif /* GEO_TYPES_H_INCLUDED */
