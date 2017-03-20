/*========================================================================

    Copyright (C) Fastrax Ltd.

    This source file is property of Fastrax Ltd. and may not be
    copied, modified or distributed without a valid iSuite SDK Lisence or
    other written permission from the copyright holder.

    Contact information:
    --------------------

    email: swsup@fastrax.fi

 =========================================================================

    Module Desciption:  Interface for local grid conversion


 =========================================================================
 *
 *
 * $Revision:   1.14  $
 *
 * $Date:   05 Apr 2005 13:48:08  $
 *
 * $Logfile:   W:/customers/archives/Suunto/inc/grid.h-arc  $
 *
 * $Author:   kh  $
 *
 *
 =========================================================================
*/
#ifndef _GRIDS_H_
#define _GRIDS_H_

#include "fstypes.h"


/////      ERROR CODES:  /////////////////////

  // Error codes related to the projection only
  #define GRID_NO_ERROR           0x0000
  #define GRID_LAT_ERROR          0x0001
  #define GRID_LON_ERROR          0x0002
  #define GRID_EASTING_ERROR      0x0004
  #define GRID_NORTHING_ERROR     0x0008
  #define GRID_ORIGIN_LAT_ERROR   0x0010
  #define GRID_CENT_MER_ERROR     0x0020
  #define GRID_A_ERROR            0x0040
  #define GRID_INV_F_ERROR        0x0080
  #define GRID_SCALE_FACTOR_ERROR 0x0100
  #define GRID_LON_WARNING        0x0200
  #define GRID_INVALID_AREA_ERROR 0x0400 // Square letters outside of limits in BRITISH or IRISH
  #define GRID_STRING_ERROR       0x0800 // Invalid square letter for BRITISH or IRISH

  // Higher level error codes
  #define GRID_PROJ_ERROR         0x1000  // This is any of the above errors
  #define GRID_DATUM_ERROR        0x2000
  #define GRID_SELECTION_ERROR    0x4000

/*
 *       TM_NO_ERROR           : No errors occurred in function
 *       TM_LAT_ERROR          : Latitude outside of valid range
 *                                      (-90 to 90 degrees)
 *       TM_LON_ERROR          : Longitude outside of valid range
 *                                      (-180 to 360 degrees, and within
 *                                        +/-90 of Central Meridian)
 *       TM_EASTING_ERROR      : Easting outside of valid range
 *                                      (depending on ellipsoid and
 *                                       projection parameters)
 *       TM_NORTHING_ERROR     : Northing outside of valid range
 *                                      (depending on ellipsoid and
 *                                       projection parameters)
 *       TM_ORIGIN_LAT_ERROR   : Origin latitude outside of valid range
 *                                      (-90 to 90 degrees)
 *       TM_CENT_MER_ERROR     : Central meridian outside of valid range
 *                                      (-180 to 360 degrees)
 *       TM_A_ERROR            : Semi-major axis less than or equal to zero
 *       TM_INV_F_ERROR        : Inverse flattening outside of valid range
 *								  	                  (250 to 350)
 *       TM_SCALE_FACTOR_ERROR : Scale factor outside of valid
 *                                     range (0.3 to 3.0)
 *		 TM_LON_WARNING              : Distortion will result if longitude is more
 *                                     than LATLON_WARN_LIMIT degrees from the Central Meridian
 */

#define LATLON_WARN_LIMIT       (9.0 * M_PI / 180.0)          /* 9 degrees */


///// AVAILABLE GRIDS ////////////////////////////
typedef enum
{
    UTM=0,
    KKJ27,
    SWEDISH,
	SWEREF99_TM,
    BRITISH,
    SWISS,
    IRISH,
	ETRS89TM35FIN,
	NZTM,
//    DUTCH,
//    BMN_M28,
//    BMN_M31,
//    BMN_M34,
//    QATAR,
//    TWD67,
//    TWD97,
//    KKJ21,
//    KKJ24,
//    KKJ30,
//    HUNGARY,
//    MALAYAN,

    NR_GRIDS
} GRID_ID;

#define LAST_GRID   (NR_GRIDS-1)

/*
// Ellipsoids used in grid projections.
typedef enum
{
    GRS80 = 0,
    INT1924,
    AIRY,
    BESSEL,
    MOD_AIRY,
    GRS67,
    WGS_84,
    AN,
    CC1866,
    NR_ELLIPSOIDS
} ELLIPSOID_ID;

#define LAST_ELLIPSOID  (NR_ELLIPSOIDS - 1)
*/


///// STRUCTURES FOR GRID PARAMETERS //////////////
typedef struct PROJ_PARAMS
{
    DOUBLE Origin_Latitude; // Latitude in radians at the origin of the projection
    DOUBLE Central_Meridian;// Longitude in radians at the center of the projection
    DOUBLE False_Easting;   // Easting/X at the center of the projection
    DOUBLE False_Northing;  // Northing/Y at the center of the projection
    DOUBLE Scale_Factor;    // Projection scale factor
} PROJ_PARAMS;


typedef struct GRID_PARAMS
{
    //char        Name[10];
    INT16       iDatumId;
    INT16       iProjMethod;
    PROJ_PARAMS par;
} GRID_PARAMS;



///// API FUNCTIONS FOR GRID CONVERSIONS //////////

// Setting datum to this value causes the default datum for the grid to be used.
#define GRID_USE_GRID_DEF_DATUM     1234

// Function for selecting the grid
WORD FAR_CODE gridSet(INT16 iGridNr, INT16 iDatum);
const __far GRID_PARAMS* gridGet(INT16 iGridNr);


// These are the actual API functions for grid conversion:
WORD FAR_CODE gridWgs84ToGrid(WORD wGridNr, INT16 iDatum, DOUBLE dLatDeg, DOUBLE dLonDeg, DOUBLE* pNorthing, DOUBLE* pEasting);
WORD FAR_CODE gridGridToWgs84(WORD wGridNr, INT16 iDatum, DOUBLE* pLatDeg, DOUBLE* pLonDeg, DOUBLE dNorthing, DOUBLE dEasting);

// Additional API functions for handling UTM zones.
WORD FAR_CODE gridSetUtmZone(WORD wZoneNumber, char cZoneLetter);
WORD FAR_CODE gridGetUtmZoneLetterIndex(DOUBLE dLatDeg);
char FAR_CODE gridGetUtmZoneLetter(DOUBLE dLatDeg);
char FAR_CODE gridGetUtmZoneLetterFromIndex(INT16 wLetterIndex);
WORD FAR_CODE gridGetUtmZoneNumber(DOUBLE dLatDeg, DOUBLE dLonDeg);
DOUBLE FAR_CODE gridGetUtmZoneMinLatitude(WORD wZoneLetterIndex);
DOUBLE FAR_CODE gridGetUtmZoneMinNorthing(WORD wZoneLetterIndex);

// Additional API functions for British and Irish grid coordinate formatting
WORD FAR_CODE gridTM_to_BNG(DOUBLE* Easting, DOUBLE* Northing, char *BNG, BOOL bng);
WORD FAR_CODE gridTM_to_BNG2(DOUBLE* Easting, DOUBLE* Northing, WORD* Bng500index, WORD* Bng100index);
WORD FAR_CODE gridBNG_to_TM(char *BNG, DOUBLE *Easting, DOUBLE *Northing);
WORD FAR_CODE gridBNG_to_TM2(WORD Bng500Index, WORD Bng100Index, DOUBLE *Easting, DOUBLE *Northing);

// Additional API functions for MGRS coordinate formatting
void FAR_CODE gridMgrsSetEllipsoid(WORD wEllipsoidID);
WORD FAR_CODE gridMgrsGetEllipsoid(void);
WORD FAR_CODE gridUTM_to_MGRS (WORD wZoneNr,
                      WORD wZoneLetterIndex,
                      DOUBLE* pEasting,
                      DOUBLE* pNorthing,
                      WORD* pE_LetterIndex,
                      WORD* pN_LetterIndex);

WORD FAR_CODE gridMGRS_to_UTM (WORD wZoneNr,
                      WORD wZoneLetterIndex,
                      DOUBLE* pEasting,
                      DOUBLE* pNorthing,
                      WORD pE_LetterIndex,
                      WORD pN_LetterIndex);

WORD FAR_CODE mgrsUTM_To_MGRS ( WORD wZoneNr,
                                WORD wZoneLetterIndex,
                                DOUBLE* pEasting,
                                DOUBLE* pNorthing,
                                WORD* pE_LetterIndex,
                                WORD* pN_LetterIndex );

WORD FAR_CODE mgrsMGRS_to_UTM(  WORD    wZoneNr,
                                WORD    wZoneLetterIndex,
                                DOUBLE* pEasting,
                                DOUBLE* pNorthing,
                                WORD    wE_LetterIndex,
                                WORD    wN_LetterIndex);


void fixRoundingError(DOUBLE * lat, DOUBLE * lon);

#endif
