#ifndef FSTYPES_H_
#define FSTYPES_H_

#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"
#include "common/core/dbgassert.h"

#ifdef ARCH_SDL
#define DOUBLE double // VS does not like floats
#else
#define DOUBLE float // iar and gcc are fine
#endif

#define FAR_CODE
#define __far
#define __y

#define INT16 int16
#define INT32 int32

#define WORD int16
#define DWORD int32

#define FromWGS84ToLocal 1
#define FromLocalToWGS84 2

#define BOOL bool
#define TRUE true
#define FALSE false

#define M_PI ((DOUBLE)3.1415926535897932384626433832795028)

#define D2R (M_PI / 180.0)
#define R2D (180.0 / M_PI)

#endif
