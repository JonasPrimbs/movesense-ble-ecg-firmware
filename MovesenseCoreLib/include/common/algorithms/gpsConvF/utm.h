/*========================================================================
   
    Copyright (C) Fastrax Ltd.

    This source file is property of Fastrax Ltd. and may not be
    copied, modified or distributed without a valid iSuite SDK Lisence or 
    other written permission from the copyright holder.

    Contact information:
    --------------------  

    email: swsup@fastrax.fi

 =========================================================================
    
    Module Desciption:  British national grid formatting functions

  
 =========================================================================
 *
 *
 * $Revision:   1.1  $
 *
 * $Date:   09 Mar 2005 10:59:04  $
 *
 * $Logfile:   W:/customers/archives/Suunto/grid/utm.h-arc  $
 *
 * $Author:   kh  $
 *
 *
 =========================================================================
*/

#ifndef _UTM_H_
#define _UTM_H_
#include "fstypes.h"
#include "limits.h"
#include "grid.h"
#include "tm.h"

///// DEFINITIONS FOR UTM ///////////////////////

#define UTM_FALSE_NORTHING_ON_SOUTHERN_HEMISPHERE   10000000.0

// UTM zone letters:
// When lat < letterLimit take the corresponding letters from UtmLetter.
// Never use " ","A" or "B". Outside of limits use "Z"
#define UTM_LETTER_LIMITS \
    {SHRT_MIN,SHRT_MIN,SHRT_MIN,-72,-64,-56,-48,-40,-32,-24,-16,-8,0,8,16,24,32,40,48,56,64,72,84,SHRT_MIN,SHRT_MAX}

#define UTM_LETTER_TABLE_LENGTH 25
#define UTM_LETTERS     " ABCDEFGHJKLMNPQRSTUVWXYZ"


#endif
