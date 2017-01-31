#pragma once
/******************************************************************************
    Copyright © Suunto Oy 2015.
    All rights reserved.
******************************************************************************/
#include <math.h>

namespace whiteboard
{

/** Helper template for specializing math primitives for vector classes */
struct VectorMathHelper
{
    /** Calculates arcus cosine for given value
     *
     * @param value Source value
     * @return Arcus cosine of source value
     */
    template <typename U> static inline U acos(U value);

    /** Calculates squate root for given value
    *
    * @param value Source value
    * @return Square root of source value
    */
    template <typename U> static inline U sqrt(U value);

    /**
        Calculartes sin for given value.
        @param value Source value.
        @param Sin of source value.
    */
    template <typename U> static inline U sin(U value);

    /**
        Calculartes cos for given value.
        @param value Source value.
        @param Cos of source value.
    */
    template <typename U> static inline U cos(U value);
};

// Default case is to use float for all types
template <typename U> U VectorMathHelper::acos(U value)
{
    return static_cast<U>(::acosf(static_cast<float>(value)));
}

// Default case is to use float for all types
////////////////////////////////////////

template <typename U> U VectorMathHelper::sqrt(U value)
{
    return static_cast<U>(::sqrtf(static_cast<float>(value)));
}

template <typename U> U VectorMathHelper::sin(U value)
{
    return static_cast<U>(::sinf(static_cast<float>(value)));
}

template <typename U> U VectorMathHelper::cos(U value)
{
    return static_cast<U>(::cosf(static_cast<float>(value)));
}

// Specializations
////////////////////////////////////////

template <> inline double VectorMathHelper::acos(double value)
{
    return ::acos(value);
}

template <> inline double VectorMathHelper::sqrt(double value)
{
    return ::sqrt(value);
}

template <> inline double VectorMathHelper::cos(double value)
{
    return ::cos(value);
}

template <> inline double VectorMathHelper::sin(double value)
{
    return ::sin(value);
}

} // namespace whiteboard
