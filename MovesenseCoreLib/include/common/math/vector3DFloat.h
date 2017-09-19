#ifndef VECTOR3DFLOAT_H_
#define VECTOR3DFLOAT_H_
/****************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

        $Id$

****************************************************************************/
/**
        \file vector3DFloat.h
        3D vector definitions of various types.
*/
#include "common/compiler/genDef.h"
#include "common/compiler/pack.h"

/**
        3D Vector item
*/

typedef struct Vector3DFloat
{
    float x;
    float y;
    float z;

#ifdef CPP_API
// Vector3DFloat(): x(0), y(0), z(0) {}
// Vector3DFloat(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}

// void reset() { x = y = z = 0.f; }
// Vector3DFloat& operator=(const Vector3DFloat& r) { x = r.x; y = r.y; z = r.z; return *this; }
#endif

} PACKED Vector3DFloat;

#endif // VECTOR3DFLOAT_H_
