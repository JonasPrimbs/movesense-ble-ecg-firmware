#pragma once
/******************************************************************************
    Copyright © Suunto Oy 2013.
    All rights reserved.
******************************************************************************/
#include "whiteboard/builtinTypes/VectorMathHelper.h"
#include "whiteboard/builtinTypes/Matrix.h"
#include "whiteboard/builtinTypes/Structures.h"

namespace whiteboard
{

/** 3D vector implementation.
 *
 * @tparam T Type of the internal data type
 */
template <typename T> class Vector3D
{
public:
    /** X component */
    union {
        WB_ALIGNED(T) mX;
        WB_ALIGNED(T) x;
    };

    union {
    /** Y component */
        WB_ALIGNED(T) mY;
        WB_ALIGNED(T) y;
    };

    /** Z component */
    union {
        WB_ALIGNED(T) mZ;
        WB_ALIGNED(T) z;
    };


public:
    /**
       Default constructor
     */
    Vector3D(void) : mX(0), mY(0), mZ(0) {}

    /**
       Constructor

       @param x X value
       @param y Y value
       @param z Z value
       */
    Vector3D(T x_, T y_, T z_) : mX(x_), mY(y_), mZ(z_) {}

    /**
       Constructor

       @param pData Pointer to item array [3]
     */
    Vector3D(const T* pData) : mX(pData[0]), mY(pData[1]), mZ(pData[2]) {}

    /**
       Copy constructor

       @param rVector Reference to the source
     */
    Vector3D(const Vector3D<T>& rVector) : mX(rVector.mX), mY(rVector.mY), mZ(rVector.mZ) {}

    /**
    Copy constructor

    @param rVector Reference to the source
    */
    template <typename U>
    EXPLICIT Vector3D(const Vector3D<U>& rVector)
        : mX(static_cast<T>(rVector.mX)), mY(static_cast<T>(rVector.mY)), mZ(static_cast<T>(rVector.mZ))
    {
    }

    /**
       Destructor
     */
    ~Vector3D(void) {}

    /**
       Assignment operator

       @param rVector Reference to the source
       @return Reference to the this object
     */
    Vector3D<T>& operator=(const Vector3D<T>& rVector);

    /**
       Assignment operator

       @param pData Pointer to item array [2]
       @return Reference to the this object
     */
    Vector3D<T>& operator=(const T* pData);

    /**
       Add operator

       @param rVector Reference to the source
       @return Reference to the this object
     */
    Vector3D<T>& operator+=(const Vector3D<T>& rVector);

    /**
       Subtract operator

       @param rVector Reference to the source
       @return Reference to the this object
     */
    Vector3D<T>& operator-=(const Vector3D<T>& rVector);

    /**
       Compare operator

       @param rVector Reference to the source
       @return True if equal
     */
    bool operator==(const Vector3D<T>& rVector) const;

    /**
       Compare operator

       @param rVector Reference to the source
       @return False if equal
     */
    bool operator!=(const Vector3D<T>& rVector) const;

    /**
       Add operator

       @param rVector	Reference to the source
       @return Result vector
     */
    Vector3D<T> operator+(const Vector3D<T>& rVector) const;

    /**
       Subtract operator

       @param rVector Reference to the source
       @return Result vector
     */
    Vector3D<T> operator-(const Vector3D<T>& rVector) const;

    /**
       Multiply operator

       @tparam U Type to use in internal processing
       @param value Operator value
       @return Result vector
     */
    template <typename U> Vector3D<U> operator*(U value) const;

    /**
       Divide operator

       @tparam U Type to use in internal processing
       @param value Operator value
       @return Result vector
     */
    template <typename U> Vector3D<U> operator/(U value) const;

    /**
       Multiply operator

       @tparam U Type to use in internal processing
       @param value Operator value
       @return Reference to the this object
     */
    template <typename U> Vector3D<T>& operator*=(U value);

    /**
       Divide operator

       @tparam U Type to use in internal processing
       @param value Operator value
       @return Reference to the this object
     */
    template <typename U> Vector3D<T>& operator/=(U value);

    /**
       Calculate dot product of two vectors

       @param rVector Reference to the source
       @return Dot multiplication result value
     */
    T dotProduct(const Vector3D<T>& rVector) const;

    /**
       Calculate vector length

       @tparam U Type to use in internal processing
       @return Vector length
     */
    template <typename U> U length(void) const;

    /**
       Calculate vector length^2

       @tparam U Type to use in internal processing
       @return Vector length
     */
    template <typename U> U lengthPow2(void) const;

    /**
       Calculate point distance

       @tparam U Type to use in internal processing
       @param rPoint Reference to the source
       @return Distance between two points
     */
    template <typename U> U distance(const Vector3D<T>& rPoint) const;

    /**
       Calculate vectors angle

       @tparam U Type to use in internal processing
       @param rPoint Reference to the source
       @return Angle bbetween two vectors
     */
    template <typename U> U angle(const Vector3D<T>& rPoint) const;

    /**
       Calculate point on line (point1 to point2) that is nearest to this point.

       @tparam U Type to use in internal processing
       @param rPoint1 Point 1
       @param rPoint2 Point 2
       @return Cross point
     */
    template <typename U> Vector3D<U> closestPointFromLine(const Vector3D<T>& rPoint1, const Vector3D<T>& rPoint2) const;

    /**
        Calculate cross product

        @param rVector Reference to the source vector
        @return Result Cross product of this and the given source vector
    */
    Vector3D<T> crossProduct(const Vector3D<T>& rVector) const;

    // Forward declarations
    class RotateMatrix;

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
    void matrixMoveAndRotate(const Vector3D<T>& rLocation, const RotateMatrix& rRotate);

public:
    /**
       Rotate points

       @param pTrg		Pointer to the target
       @param pSrc		Pointer to the source
       @param n			Number of points
       @param rRotate	Rotate matrix
     */
    static void matrixRotate(Vector3D<T>* pTrg, const Vector3D<T>* pSrc, int n, const RotateMatrix& rRotate);

    /**
       Rotate and move points

       @param pTrg		Pointer to the target
       @param pSrc		Pointer to the source
       @param n			Number of points
       @param rLocation	Current location
       @param rRotate	Rotate matrix
     */
    static void matrixMoveAndRotate(
        Vector3D<T>* pTrg, const Vector3D<T>* pSrc, int n, const Vector3D<T>& rLocation, const RotateMatrix& rRotate);

    /**
       Axis rotate

       @param pTrg		Pointer to the target
       @param pSrc		Pointer to the source
       @param n			Number of points
       @param rAngles	Rotate angles
     */
    static void axisRotate(Vector3D<T>* pTrg, const Vector3D<T>* pSrc, int n, const Vector3D<T>& rAngles);

    /**
       Move points in space

       @param rLocation		Current location
       @param rDirection	Direction to move
       @param speed			Speed to move
     */
    static void moveInSpace(Vector3D<T>& rLocation, const Vector3D<T>& rDirection, float speed);

    /**
       Move point with vector

       @param pVec		Pointer to the points
       @param n			Number of points
       @param rMove		Vector to move
     */
    static void move(Vector3D<T>* pVec, int n, const Vector3D<T>& rMove);

    /**
       Make 2D view

       @param xCenter	X center position
       @param yCenter	Y center position
       @param pVec		Pointer to the points
       @param n			Number of points
       @param zoom		Zoom factor
     */
    static void make2dView(float xCenter, float yCenter, Vector3D<T>* pVec, int n, float zoom);

public:
    // Needed for structure type identification at compile time
    typedef int Structure;

    // Fixed ID of the data type (must be defined in the header or it is not treated as constant)
    static const LocalDataTypeId DATA_TYPE_ID = static_cast<LocalDataTypeId>(
        IsSame<T, int32>::value ? 30 : IsSame<T, float>::value ? 31 : -1);
};

// Static template members
template <typename T> const LocalDataTypeId Vector3D<T>::DATA_TYPE_ID;

// Typedef some helpers
typedef Vector3D<int32> IntVector3D;
typedef Vector3D<float> FloatVector3D;

template <typename T> class Vector3D<T>::RotateMatrix
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
    void make(const Vector3D<T>& rAxes, T angle);

    /**
       Set unit matrix
     */
    void unit(void);

    /**
       Rotate unit matrix

       @param rVector	Rotate around vector
       @param angle		Angle to rotate
     */
    void rotate(const Vector3D<T>& rVector, float angle);

    /**
       Rotate unit matrix width angles

       @param rAngle	Angles to rotate
     */
    void rotate(const Vector3D<T>& rAngle);

    /**
       Get matrix data

       @param pTarget	Target table pointer
     */
    void getData(float* pTarget) const;

    /**
       Get matrix rows

       @return const Vector3D* Matrix rows
     */
    inline const Vector3D<T>* getRows(void) const { return mRow; }

protected:
    Vector3D<T> mRow[3];
};

template <typename T> Vector3D<T>& Vector3D<T>::operator=(const Vector3D<T>& rVector)
{
    mX = rVector.mX;
    mY = rVector.mY;
    mZ = rVector.mZ;

    return *this;
}

template <typename T> Vector3D<T>& Vector3D<T>::operator=(const T* pData)
{
    mX = pData[0];
    mY = pData[1];
    mZ = pData[2];

    return *this;
}

template <typename T> Vector3D<T>& Vector3D<T>::operator+=(const Vector3D<T>& rVector)
{
    mX += rVector.mX;
    mY += rVector.mY;
    mZ += rVector.mZ;

    return *this;
}

template <typename T> Vector3D<T>& Vector3D<T>::operator-=(const Vector3D<T>& rVector)
{
    mX -= rVector.mX;
    mY -= rVector.mY;
    mZ -= rVector.mZ;

    return *this;
}

template <typename T> bool Vector3D<T>::operator==(const Vector3D<T>& rVector) const
{
    if ((mX == rVector.mX) && (mY == rVector.mY) && (mZ == rVector.mZ))
    {
        return true;
    }

    return false;
}

template <typename T> bool Vector3D<T>::operator!=(const Vector3D<T>& rVector) const
{
    return !(*this == rVector);
}

template <typename T> Vector3D<T> Vector3D<T>::operator+(const Vector3D<T>& rVector) const
{
    return Vector3D<T>(*this) += rVector;
}

template <typename T> Vector3D<T> Vector3D<T>::operator-(const Vector3D<T>& rVector) const
{
    return Vector3D<T>(*this) -= rVector;
}

template <typename T> template <typename U> Vector3D<U> Vector3D<T>::operator*(U value) const
{
    return Vector3D<U>(static_cast<U>(mX) * value, static_cast<U>(mY) * value, static_cast<U>(mZ) * value);
}

template <typename T> template <typename U> Vector3D<U> Vector3D<T>::operator/(U value) const
{
    return Vector3D<U>(static_cast<U>(mX) / value, static_cast<U>(mY) / value, static_cast<U>(mZ) / value);
}

template <typename T> template <typename U> Vector3D<T>& Vector3D<T>::operator*=(U value)
{
    mX = static_cast<T>(static_cast<U>(mX) * value);
    mY = static_cast<T>(static_cast<U>(mY) * value);
    mZ = static_cast<T>(static_cast<U>(mZ) * value);

    return *this;
}

template <typename T> template <typename U> Vector3D<T>& Vector3D<T>::operator/=(U value)
{
    if (value != 0)
    {
        mX = static_cast<T>(static_cast<U>(mX) / value);
        mY = static_cast<T>(static_cast<U>(mY) / value);
        mZ = static_cast<T>(static_cast<U>(mZ) / value);
    }

    return *this;
}

template <typename T> T Vector3D<T>::dotProduct(const Vector3D<T>& rVector) const
{
    return mX * rVector.mX + mY * rVector.mY + mZ * rVector.mZ;
}

template <typename T> template <typename U> U Vector3D<T>::length(void) const
{
    return VectorMathHelper::sqrt(lengthPow2<U>());
}

template <typename T> template <typename U> U Vector3D<T>::lengthPow2(void) const
{
    return static_cast<U>(mX) * static_cast<U>(mX) + static_cast<U>(mY) * static_cast<U>(mY) +
           static_cast<U>(mZ) * static_cast<U>(mZ);
}

template <typename T> template <typename U> U Vector3D<T>::distance(const Vector3D<T>& rPoint) const
{
    Vector3D vec = *this - rPoint;
    return vec.length<U>();
}

template <typename T> template <typename U> U Vector3D<T>::angle(const Vector3D<T>& rPoint) const
{
    const U lengths = length<U>() * rPoint.length<U>();
    if (lengths > 0)
    {
        const T value = dotProduct(rPoint);
        return VectorMathHelper::acos(value / lengths);
    }

    return 0;
}

template <typename T>
template <typename U>
Vector3D<U> Vector3D<T>::closestPointFromLine(const Vector3D<T>& rPoint1, const Vector3D<T>& rPoint2) const
{
    // Vectors between points
    Vector3D<U> a(rPoint2 - rPoint1);
    Vector3D<U> b(*this - rPoint1);

    // Cross vector from line to the this point
    Vector3D<U> c(a.crossProduct(a.crossProduct(b)));
    U n = 0, div = 0;

    // Solve vector A length 'n' to the cross point (three possibility)
    if ((div = (a.mY * c.mX) - (a.mX * c.mY)) != 0)
    {
        n = (c.mX * (mY - rPoint1.mY) + c.mY * (rPoint1.mX - mX)) / div;
    }
    else if ((div = (a.mY * c.mZ) - (a.mZ * c.mY)) != 0)
    {
        n = (c.mZ * (mY - rPoint1.mY) + c.mY * (rPoint1.mZ - mZ)) / div;
    }
    else if ((div = (a.mZ * c.mX) - (a.mX * c.mZ)) != 0)
    {
        n = (c.mX * (mZ - rPoint1.mZ) + c.mZ * (rPoint1.mX - mX)) / div;
    }

    // And finaly calculate cross point on line
    return Vector3D<U>(rPoint1) + (a * n);
}

template <typename T> Vector3D<T> Vector3D<T>::crossProduct(const Vector3D<T>& rVector) const
{
    return Vector3D<T>(
        (mY * rVector.mZ) - (mZ * rVector.mY), (mZ * rVector.mX) - (mX * rVector.mZ), (mX * rVector.mY) - (mY * rVector.mX));
}

template <typename T> void Vector3D<T>::matrixRotate(const RotateMatrix& rRotate)
{
    Vector3D<T> tmp(*this);
    matrixRotate(this, &tmp, 1, rRotate);
}

template <typename T> void Vector3D<T>::matrixMoveAndRotate(const Vector3D<T>& rLocation, const RotateMatrix& rRotate)
{
    Vector3D<T> tmp(*this);
    matrixMoveAndRotate(this, &tmp, 1, rLocation, rRotate);
}

template <typename T>
void Vector3D<T>::matrixRotate(Vector3D<T>* pTrg, const Vector3D<T>* pSrc, int n, const RotateMatrix& rRotate)
{
    WB_ASSERT(pTrg != NULL);
    WB_ASSERT(pSrc != NULL);
    const Vector3D<T>* pRow = rRotate.getRows();

    while (n-- > 0)
    {
        pTrg->mX = pSrc->mX * pRow[RotateMatrix::X_AXIS].mX + pSrc->mY * pRow[RotateMatrix::Y_AXIS].mX +
                   pSrc->mZ * pRow[RotateMatrix::Z_AXIS].mX;
        pTrg->mY = pSrc->mX * pRow[RotateMatrix::X_AXIS].mY + pSrc->mY * pRow[RotateMatrix::Y_AXIS].mY +
                   pSrc->mZ * pRow[RotateMatrix::Z_AXIS].mY;
        pTrg->mZ = pSrc->mX * pRow[RotateMatrix::X_AXIS].mZ + pSrc->mY * pRow[RotateMatrix::Y_AXIS].mZ +
                   pSrc->mZ * pRow[RotateMatrix::Z_AXIS].mZ;

        ++pTrg;
        ++pSrc;
    }
}

template <typename T>
void Vector3D<T>::matrixMoveAndRotate(
    Vector3D<T>* pTrg, const Vector3D<T>* pSrc, int n, const Vector3D<T>& rLocation, const RotateMatrix& rRotate)
{
    WB_ASSERT(pTrg != NULL);
    WB_ASSERT(pSrc != NULL);
    const Vector3D<T>* pRow = rRotate.getRows();

    while (n-- > 0)
    {
        const Vector3D<T> vec = *pSrc - rLocation;

        pTrg->mX = vec.mX * pRow[RotateMatrix::X_AXIS].mX + vec.mY * pRow[RotateMatrix::Y_AXIS].mX +
                   vec.mZ * pRow[RotateMatrix::Z_AXIS].mX;
        pTrg->mY = vec.mX * pRow[RotateMatrix::X_AXIS].mY + vec.mY * pRow[RotateMatrix::Y_AXIS].mY +
                   vec.mZ * pRow[RotateMatrix::Z_AXIS].mY;
        pTrg->mZ = vec.mX * pRow[RotateMatrix::X_AXIS].mZ + vec.mY * pRow[RotateMatrix::Y_AXIS].mZ +
                   vec.mZ * pRow[RotateMatrix::Z_AXIS].mZ;

        ++pTrg;
        ++pSrc;
    }
}

template <typename T> void Vector3D<T>::axisRotate(Vector3D<T>* pTrg, const Vector3D<T>* pSrc, int n, const Vector3D<T>& rAngles)
{
    WB_ASSERT(pTrg != NULL);
    WB_ASSERT(pSrc != NULL);

    float sin_x = sinf(rAngles.mX);
    float cos_x = cosf(rAngles.mX);
    float sin_y = sinf(rAngles.mY);
    float cos_y = cosf(rAngles.mY);
    float sin_z = sinf(rAngles.mZ);
    float cos_z = cosf(rAngles.mZ);

    while (n-- > 0)
    {
        float x, y, z;
        Vector3D<T> vec = *pSrc++;

        z = vec.mZ;
        y = vec.mY;
        vec.mZ = cos_x * z + sin_x * y;
        vec.mY = cos_x * y - sin_x * z;

        x = vec.mX;
        z = vec.mZ;
        vec.mX = cos_y * x + sin_y * z;
        pTrg->mZ = cos_y * z - sin_y * x;

        y = vec.mY;
        x = vec.mX;
        pTrg->mY = cos_z * y + sin_z * x;
        pTrg->mX = cos_z * x - sin_z * y;

        ++pTrg;
    }
}

template <typename T> void Vector3D<T>::moveInSpace(Vector3D<T>& rLocation, const Vector3D<T>& rDirection, float speed)
{
    rLocation += rDirection * speed;
}

template <typename T> void Vector3D<T>::move(Vector3D<T>* pVec, int n, const Vector3D<T>& rMove)
{
    WB_ASSERT(pVec != NULL);

    while (n-- > 0)
    {
        *pVec += rMove;
        ++pVec;
    }
}

template <typename T> void Vector3D<T>::make2dView(float xCenter, float yCenter, Vector3D<T>* pVec, int n, float zoom)
{
    WB_ASSERT(pVec != NULL);

    while (n-- > 0)
    {
        float dist = -pVec->mX;

        if (dist > 0)
        {
            if (dist > 0.1f)
            {
                const float factor = zoom / dist;
                dist += (fabsf(pVec->mY) + fabsf(pVec->mZ)) * 0.1f;
                pVec->mX = xCenter + pVec->mY * factor;
                pVec->mY = yCenter - pVec->mZ * factor;
            }
            else
            {
                dist = 0;
            }
        }
        pVec->mZ = dist;

        ++pVec;
    }
}

template <typename T> Vector3D<T>::RotateMatrix::RotateMatrix(void)
{
}

template <typename T> Vector3D<T>::RotateMatrix::RotateMatrix(const typename Vector3D<T>::RotateMatrix& rRotateMatrix)
{
    *this = rRotateMatrix;
}

template <typename T>
typename Vector3D<T>::RotateMatrix& Vector3D<T>::RotateMatrix::operator=(const typename Vector3D<T>::RotateMatrix& rRotateMatrix)
{
    mRow[X_AXIS] = rRotateMatrix.mRow[X_AXIS];
    mRow[Y_AXIS] = rRotateMatrix.mRow[Y_AXIS];
    mRow[Z_AXIS] = rRotateMatrix.mRow[Z_AXIS];

    return *this;
}

template <typename T> typename Vector3D<T>::RotateMatrix& Vector3D<T>::RotateMatrix::operator=(const float* pSource)
{
    mRow[X_AXIS] = &pSource[0];
    mRow[Y_AXIS] = &pSource[3];
    mRow[Z_AXIS] = &pSource[6];

    return *this;
}

template <typename T> typename Vector3D<T>::RotateMatrix& Vector3D<T>::RotateMatrix::operator=(const Matrix& rMatrix)
{
    *this = rMatrix.getData();
    return *this;
}

template <typename T> void Vector3D<T>::RotateMatrix::make(const Vector3D<T>& rAxes, T angle)
{
    static const Vector3D<T> sUnit[3] = {Vector3D<T>(1, 0, 0), Vector3D<T>(0, 1, 0), Vector3D<T>(0, 0, 1)};

    Vector3D<T> axes(rAxes);
    axes /= axes.length<T>();
    const T xy = axes.mX * axes.mY;
    const T xz = axes.mX * axes.mZ;
    const T yz = axes.mY * axes.mZ;

    const Vector3D<T> nnT[3] = {
        Vector3D<T>(axes.mX * axes.mX, xy, xz), Vector3D<T>(xy, axes.mY * axes.mY, yz), Vector3D<T>(xz, yz, axes.mZ * axes.mZ)};

    const Vector3D<T> cross[3] = {
        Vector3D<T>(0, axes.mZ, -axes.mY), Vector3D<T>(-axes.mZ, 0, axes.mX), Vector3D<T>(axes.mY, -axes.mX, 0)};

    const T cosAngle = VectorMathHelper::cos(angle);
    const T sinAngle = VectorMathHelper::sin(angle);

    for (int i = 0; i < 3; ++i)
    {
        mRow[i] = nnT[i] + (sUnit[i] - nnT[i]) * cosAngle + cross[i] * sinAngle;
    }
}

template <typename T> void Vector3D<T>::RotateMatrix::unit(void)
{
    mRow[X_AXIS] = Vector3D<T>(1, 0, 0);
    mRow[Y_AXIS] = Vector3D<T>(0, 1, 0);
    mRow[Z_AXIS] = Vector3D<T>(0, 0, 1);
}

template <typename T> void Vector3D<T>::RotateMatrix::rotate(const Vector3D<T>& rVector, float angle)
{
    RotateMatrix matrix;
    RotateMatrix temp(*this);

    matrix.make(rVector, angle);
    Vector3D<T>::matrixRotate(mRow, temp.mRow, 3, matrix);
}

template <typename T> void Vector3D<T>::RotateMatrix::rotate(const Vector3D<T>& rAngle)
{
    if (rAngle.mX != 0.0)
    {
        rotate(mRow[X_AXIS], rAngle.mX);
    }

    if (rAngle.mY != 0.0)
    {
        rotate(mRow[Y_AXIS], rAngle.mY);
    }

    if (rAngle.mZ != 0.0)
    {
        rotate(mRow[Z_AXIS], rAngle.mZ);
    }
}

template <typename T> void Vector3D<T>::RotateMatrix::getData(float* pTarget) const
{
    *pTarget++ = mRow[X_AXIS].mX;
    *pTarget++ = mRow[X_AXIS].mY;
    *pTarget++ = mRow[X_AXIS].mZ;

    *pTarget++ = mRow[Y_AXIS].mX;
    *pTarget++ = mRow[Y_AXIS].mY;
    *pTarget++ = mRow[Y_AXIS].mZ;

    *pTarget++ = mRow[Z_AXIS].mX;
    *pTarget++ = mRow[Z_AXIS].mY;
    *pTarget++ = mRow[Z_AXIS].mZ;
}

} // namespace whiteboard
