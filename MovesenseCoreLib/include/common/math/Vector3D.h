#pragma once
/******************************************************************************
    Copyright � Suunto Oy 2013.

    All rights reserved.

******************************************************************************/
/**
        @file Vector3D.h

        3D vector implementation.
*/

#include "common/math/vector3DInt32.h"

namespace nea
{

class Matrix;

class Vector3D
{
public:
    class RotateMatrix;

    float mX, mY, mZ;

public:
    /**
       Default constructor
     */
    Vector3D(void) : mX(0), mY(0), mZ(0) {}

    /**
       Constructor

       @param x		X value
       @param y		Y value
       @param z		Z value
     */
    Vector3D(float x, float y, float z) : mX(x), mY(y), mZ(z) {}

    /**
       Constructor

       @param pData	Pointer to float array [3]
     */
    explicit Vector3D(const float* pData) : mX(pData[0]), mY(pData[1]), mZ(pData[2]) {}

    /**
       Copy constructor

       @param rVector	Reference to the source
     */
    Vector3D(const Vector3D& rVector) : mX(rVector.mX), mY(rVector.mY), mZ(rVector.mZ) {}

    /**
     *
     *
     * @author sutinenm (23.5.2013)
     *
     * @param rVector, sori Aimo, taa on valiaikainen valppa.
     */
    explicit Vector3D(const Vector3DInt32& rVector) : mX((float)rVector.x), mY((float)rVector.y), mZ((float)rVector.z) {}

    /**
       Destructor
     */
    ~Vector3D(void) {}

    /**
       Copy operator

       @param rVector	Reference to the source

       @return Vector3D&		Reference to the this object
     */
    Vector3D& operator=(const Vector3D& rVector);

    /**
       Copy operator

       @param pData		Pointer to float array [3]

       @return Vector3D&		Reference to the this object
     */
    Vector3D& operator=(const float* pData);

    /**
       Add operator

       @param rVector	Reference to the source

       @return Vector3D&	Reference to the this object
     */
    Vector3D& operator+=(const Vector3D& rVector);

    /**
       Subtract operator

       @param rVector	Reference to the source

       @return Vector3D&	Reference to the this object
     */
    Vector3D& operator-=(const Vector3D& rVector);

    /**
       Compare operator

       @param rVector	Reference to the source

       @return bool		True if equal
     */
    bool operator==(const Vector3D& rVector) const;

    /**
       Compare operator

       @param rVector	Reference to the source

       @return bool		False if equal
     */
    bool operator!=(const Vector3D& rVector) const;

    /**
       Add operator

       @param rVector	Reference to the source

       @return Vector3D	Result vector
     */
    Vector3D operator+(const Vector3D& rVector) const;

    /**
       Subtract operator

       @param rVector	Reference to the source

       @return Vector3D	Result vector
     */
    Vector3D operator-(const Vector3D& rVector) const;

    /**
       Multiply operator

       @param value		Operator value

       @return Vector3D	Result vector
     */
    Vector3D operator*(float value) const;

    /**
       Divide operator

       @param value		Operator value

       @return Vector3D	Result vector
     */
    Vector3D operator/(float value) const;

    /**
       Multiply operator

       @param value		Operator value

       @return Vector3D&	Reference to the this object
     */
    Vector3D& operator*=(float value);

    /**
       Divide operator

       @param value		Operator value

       @return Vector3D&	Reference to the this object
     */
    Vector3D& operator/=(float value);

    /**
       Calculate dot mul

       @param rVector	Reference to the source

       @return float	Result value
     */
    float dotMul(const Vector3D& rVector) const;

    /**
       Calculate vector length

       @return float	Vector length
     */
    float length(void) const;

    /**
       Calculate vector length^2

       @return float	Vector length
     */
    float lengthPow2(void) const;

    /**
       Calculate point distance

       @param rPoint	Reference to the source

       @return float	Distance
     */
    float distance(const Vector3D& rPoint) const;

    /**
       Calculate vectors angle

       @param rPoint	Reference to the source

       @return float	Angle value
     */
    float angle(const Vector3D& rPoint) const;

    /**
       Rotate points

       @param rRotate	Rotate matrix
     */
    void matrixRotate(const RotateMatrix& rRotate);

    /**
       Rotate and move points

       @param rLocation	Current location
       @param rRotate	Rotate matrix
     */
    void matrixMoveAndRotate(const Vector3D& rLocation, const RotateMatrix& rRotate);

    /**
       Calculate nearest point on line (line = point1 to point2).

       @param rPoint1	Point 1
       @param rPoint2	Point 2

       @return Vector3D	Cross point
     */
    Vector3D lineCrossPoint(const Vector3D& rPoint1, const Vector3D& rPoint2) const;

    /**
       Calculate cross mul

       @param rVec1		Reference to the source vector 1
       @param rVec2		Reference to the source vector 2

       @return Vector3D	Result vector 1 & 2 cross mul
     */
    static Vector3D crossMul(const Vector3D& rVec1, const Vector3D& rVec2);

public:
    /**
       Rotate points

       @param pTrg		Pointer to the target
       @param pSrc		Pointer to the source
       @param n			Number of points
       @param rRotate	Rotate matrix
     */
    static void matrixRotate(Vector3D* pTrg, const Vector3D* pSrc, int n, const RotateMatrix& rRotate);

    /**
       Rotate and move points

       @param pTrg		Pointer to the target
       @param pSrc		Pointer to the source
       @param n			Number of points
       @param rLocation	Current location
       @param rRotate	Rotate matrix
     */
    static void
    matrixMoveAndRotate(Vector3D* pTrg, const Vector3D* pSrc, int n, const Vector3D& rLocation, const RotateMatrix& rRotate);

    /**
       Axis rotate

       @param pTrg		Pointer to the target
       @param pSrc		Pointer to the source
       @param n			Number of points
       @param rAngles	Rotate angles
     */
    static void axisRotate(Vector3D* pTrg, const Vector3D* pSrc, int n, const Vector3D& rAngles);

    /**
       Move points in space

       @param rLocation		Current location
       @param rDirection	Direction to move
       @param speed			Speed to move
     */
    static void moveInSpace(Vector3D& rLocation, const Vector3D& rDirection, float speed);

    /**
       Move point with vector

       @param pVec		Pointer to the points
       @param n			Number of points
       @param rMove		Vector to move
     */
    static void move(Vector3D* pVec, int n, const Vector3D& rMove);

    /**
       Make 2D view

       @param xCenter	X center position
       @param yCenter	Y center position
       @param pVec		Pointer to the points
       @param n			Number of points
       @param zoom		Zoom factor
     */
    static void make2dView(float xCenter, float yCenter, Vector3D* pVec, int n, float zoom);
};

class Vector3D::RotateMatrix
{
public:
    enum
    {
        X_AXIS,
        Y_AXIS,
        Z_AXIS
    };

public:
    /**
       Default constructor (makes unit matrix)
     */
    RotateMatrix(void);

    /**
       Copy constructor

       @param rRotateMatrix Source object
     */
    RotateMatrix(const RotateMatrix& rRotateMatrix);

    /**
       Destructor
     */
    ~RotateMatrix(void) {}

    /**
       Copy operator

       @param rRotateMatrix Source object

       @return RotateMatrix& This object
     */
    RotateMatrix& operator=(const RotateMatrix& rRotateMatrix);

    /**
       Copy operator

       @param pSource 	Source data

       @return RotateMatrix& This object
     */
    RotateMatrix& operator=(const float* pSource);

    /**
       Copy operator for calibration matrix

       @param rMatrix 	Source data

       @return RotateMatrix& This object
     */
    RotateMatrix& operator=(const Matrix& rMatrix);

    /**
       Make rotate matrix

       @param rAxes		Axes to rotate
       @param angle		Angle to rotate
     */
    void make(Vector3D axes, float angle);

    /**
       Set unit matrix
     */
    void unit(void);

    /**
       Rotate unit matrix

       @param rVector	Rotate around vector
       @param angle		Angle to rotate
     */
    void rotate(const Vector3D& rVector, float angle);

    /**
       Rotate unit matrix width angles

       @param rAngle	Angles to rotate
     */
    void rotate(const Vector3D& rAngle);

    /**
       Get matrix data

       @param pTarget	Target table pointer
     */
    void getData(float* pTarget) const;

    /**
       Get matrix rows

       @return const Vector3D* Matrix rows
     */
    inline const Vector3D* getRows(void) const { return mRow; }

protected:
    Vector3D mRow[3];

    static const Vector3D sUnit[3];
};

} // namespace nea
