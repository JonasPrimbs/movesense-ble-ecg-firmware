#ifndef VECTOR3DINT32_H_
#define VECTOR3DINT32_H_
/****************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

        $Id$

****************************************************************************/
/**
        \file vector3DInt32.h
        32-bit 3D vector definition.
*/
#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"
#include "common/compiler/pack.h"

/**
        3D Vector item
*/
typedef struct Vector3DInt32
{
    int32 x;
    int32 y;
    int32 z;

#ifdef CPP_API
// Vector3DInt32(): x(0), y(0), z(0) {}
// Vector3DInt32(int32 _x, int32 _y, int32 _z): x(_x), y(_y), z(_z) {}

// void reset() { x = y = z = 0; }
// Vector3DInt32& operator=(const Vector3DInt32& r) { x = r.x; y = r.y; z = r.z; return *this; }
#endif

} PACKED Vector3DInt32;

#endif // VECTOR3DINT32_H_
