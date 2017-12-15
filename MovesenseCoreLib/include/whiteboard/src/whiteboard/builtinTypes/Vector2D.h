#pragma once
/******************************************************************************
    Copyright © Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/VectorMathHelper.h"
#include "whiteboard/builtinTypes/Structures.h"

namespace whiteboard
{

/** 2D vector implementation.
*
* @tparam T Type of the internal data type
*/
template <typename T> class Vector2D
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

public:
    /**
       Default constructor
     */
    Vector2D(void) : mX(0), mY(0) {}

    /**
       Constructor

       @param x X value
       @param y Y value
     */
    Vector2D(T x_, T y_) : mX(x_), mY(y_) {}

    /**
       Constructor

       @param pData Pointer to item array [2]
     */
    Vector2D(const T* pData) : mX(pData[0]), mY(pData[1]) {}

    /**
       Copy constructor

       @param rVector Reference to the source
     */
    Vector2D(const Vector2D<T>& rVector) : mX(rVector.mX), mY(rVector.mY) {}

    /**
    Copy constructor

    @param rVector Reference to the source
    */
    template <typename U>
    EXPLICIT Vector2D(const Vector2D<U>& rVector) : mX(static_cast<T>(rVector.mX)), mY(static_cast<T>(rVector.mY))
    {
    }

    /**
       Destructor
     */
    ~Vector2D(void) {}

    /**
       Assignment operator

       @param rVector Reference to the source
       @return Reference to the this object
     */
    Vector2D<T>& operator=(const Vector2D<T>& rVector);

    /**
       Assignment operator

       @param pData Pointer to item array [2]
       @return Reference to the this object
     */
    Vector2D<T>& operator=(const T* pData);

    /**
       Add operator

       @param rVector Reference to the source
       @return Reference to the this object
     */
    Vector2D<T>& operator+=(const Vector2D<T>& rVector);

    /**
       Subtract operator

       @param rVector Reference to the source
       @return Reference to the this object
     */
    Vector2D<T>& operator-=(const Vector2D<T>& rVector);

    /**
       Compare operator

       @param rVector Reference to the source
       @return True if equal
     */
    bool operator==(const Vector2D<T>& rVector) const;

    /**
       Compare operator

       @param rVector Reference to the source
       @return False if equal
     */
    bool operator!=(const Vector2D<T>& rVector) const;

    /**
       Add operator

       @param rVector	Reference to the source
       @return Result vector
     */
    Vector2D<T> operator+(const Vector2D<T>& rVector) const;

    /**
       Subtract operator

       @param rVector Reference to the source
       @return Result vector
     */
    Vector2D<T> operator-(const Vector2D<T>& rVector) const;

    /**
       Multiply operator

       @tparam U Type to use in internal processing
       @param value Operator value
       @return Result vector
     */
    template <typename U> Vector2D<U> operator*(U value) const;

    /**
       Divide operator

       @tparam U Type to use in internal processing
       @param value Operator value
       @return Result vector
     */
    template <typename U> Vector2D<U> operator/(U value) const;

    /**
       Multiply operator

       @tparam U Type to use in internal processing
       @param value Operator value
       @return Reference to the this object
     */
    template <typename U> Vector2D<T>& operator*=(U value);

    /**
       Divide operator

       @tparam U Type to use in internal processing
       @param value Operator value
       @return Reference to the this object
     */
    template <typename U> Vector2D<T>& operator/=(U value);

    /**
       Calculate dot product of two vectors

       @param rVector Reference to the source
       @return Dot multiplication result value
     */
    T dotProduct(const Vector2D<T>& rVector) const;

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
    template <typename U> U distance(const Vector2D<T>& rPoint) const;

    /**
       Calculate vectors angle

       @tparam U Type to use in internal processing
       @param rPoint Reference to the source
       @return Angle bbetween two vectors
     */
    template <typename U> U angle(const Vector2D<T>& rPoint) const;

    /**
       Calculate point on line (point1 to point2) that is nearest to this point.

       @tparam U Type to use in internal processing
       @param rPoint1 Point 1
       @param rPoint2 Point 2
       @return Cross point
     */
    template <typename U> Vector2D<U> closestPointFromLine(const Vector2D<T>& rPoint1, const Vector2D<T>& rPoint2) const;

public:
    // Needed for structure type identification at compile time
    typedef int Structure;

    // Fixed ID of the data type (must be defined in the header or it is not treated as constant)
    static const LocalDataTypeId DATA_TYPE_ID = static_cast<LocalDataTypeId>(
        IsSame<T, int32>::value ? 28 : IsSame<T, float>::value ? 29 : -1);
};

// Static template members
template <typename T> const LocalDataTypeId Vector2D<T>::DATA_TYPE_ID;

// Typedef some helpers
typedef Vector2D<int32> IntVector2D;
typedef Vector2D<float> FloatVector2D;

template <typename T> Vector2D<T>& Vector2D<T>::operator=(const Vector2D<T>& rVector)
{
    mX = rVector.mX;
    mY = rVector.mY;

    return *this;
}

template <typename T> Vector2D<T>& Vector2D<T>::operator=(const T* pData)
{
    mX = pData[0];
    mY = pData[1];

    return *this;
}

template <typename T> Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& rVector)
{
    mX += rVector.mX;
    mY += rVector.mY;

    return *this;
}

template <typename T> Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& rVector)
{
    mX -= rVector.mX;
    mY -= rVector.mY;

    return *this;
}

template <typename T> bool Vector2D<T>::operator==(const Vector2D<T>& rVector) const
{
    if ((mX == rVector.mX) && (mY == rVector.mY))
    {
        return true;
    }

    return false;
}

template <typename T> bool Vector2D<T>::operator!=(const Vector2D<T>& rVector) const
{
    return !(*this == rVector);
}

template <typename T> Vector2D<T> Vector2D<T>::operator+(const Vector2D<T>& rVector) const
{
    return Vector2D<T>(*this) += rVector;
}

template <typename T> Vector2D<T> Vector2D<T>::operator-(const Vector2D<T>& rVector) const
{
    return Vector2D<T>(*this) -= rVector;
}

template <typename T> template <typename U> Vector2D<U> Vector2D<T>::operator*(U value) const
{
    return Vector2D<U>(static_cast<U>(mX) * value, static_cast<U>(mY) * value);
}

template <typename T> template <typename U> Vector2D<U> Vector2D<T>::operator/(U value) const
{
    return Vector2D<U>(static_cast<U>(mX) / value, static_cast<U>(mY) / value);
}

template <typename T> template <typename U> Vector2D<T>& Vector2D<T>::operator*=(U value)
{
    mX = static_cast<T>(static_cast<U>(mX) * value);
    mY = static_cast<T>(static_cast<U>(mY) * value);

    return *this;
}

template <typename T> template <typename U> Vector2D<T>& Vector2D<T>::operator/=(U value)
{
    if (value != 0)
    {
        mX = static_cast<T>(static_cast<U>(mX) / value);
        mY = static_cast<T>(static_cast<U>(mY) / value);
    }

    return *this;
}

template <typename T> T Vector2D<T>::dotProduct(const Vector2D<T>& rVector) const
{
    return mX * rVector.mX + mY * rVector.mY;
}

template <typename T> template <typename U> U Vector2D<T>::length(void) const
{
    return VectorMathHelper::sqrt(lengthPow2<U>());
}

template <typename T> template <typename U> U Vector2D<T>::lengthPow2(void) const
{
    return static_cast<U>(mX) * static_cast<U>(mX) + static_cast<U>(mY) * static_cast<U>(mY);
}

template <typename T> template <typename U> U Vector2D<T>::distance(const Vector2D<T>& rPoint) const
{
    Vector2D vec = *this - rPoint;
    return vec.length<U>();
}

template <typename T> template <typename U> U Vector2D<T>::angle(const Vector2D<T>& rPoint) const
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
Vector2D<U> Vector2D<T>::closestPointFromLine(const Vector2D<T>& rPoint1, const Vector2D<T>& rPoint2) const
{
    // Vectors between points
    Vector2D<U> a(rPoint2 - rPoint1);
    Vector2D<U> b(*this - rPoint1);

    // Squared length of a vector
    U aLengthPow2 = a.template lengthPow2<U>();
    if (aLengthPow2 == 0)
    {
        return Vector2D<U>(rPoint1);
    }

    // Dot product of the two vectors
    U abDotProduct = a.dotProduct(b);

    // Normalized distance
    U normDistance = abDotProduct / aLengthPow2;

    // And finally caluclate closest point from line
    return Vector2D<U>(static_cast<U>(rPoint1.mX) + a.mX * normDistance, static_cast<U>(rPoint1.mY) + a.mY * normDistance);
}

} // namespace whiteboard
