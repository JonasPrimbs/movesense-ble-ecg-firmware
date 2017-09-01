#ifndef VECTOR3DINT16_H_
#define VECTOR3DINT16_H_
/****************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

        $Id$

****************************************************************************/
/**
        \file vector3DInt16.h
        16-bit 3D vector definition.
*/
#ifndef GENDEF_H_
#include "common/compiler/genDef.h"
#endif

/**
        3D Vector item
*/
typedef struct Vector3DInt16
{
    int16 x;
    int16 y;
    int16 z;

#ifdef CPP_API
// Vector3DInt16(): x(0), y(0), z(0) {}
// Vector3DInt16(int16 _x, int16 _y, int16 _z): x(_x), y(_y), z(_z) {}

// void reset() { x = y = z = 0; }
// Vector3DInt16& operator=(const Vector3DInt16& r) { x = r.x; y = r.y; z = r.z; return *this; }
#endif

} Vector3DInt16;

#endif // VECTOR3DINT16_H_
