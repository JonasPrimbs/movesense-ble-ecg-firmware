#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/Value.h"

namespace whiteboard
{

/** Don't panic. These are Whiteboard ResourceClient and ResourceProvider API related
 * helper types. They are used internally to help in compile time type checking
 * and to make API calls more optimized and probably are something that you should not
 * worry about.
 */
struct WB_API Api
{
    /** Wrapper class for passing optional parameters in compile time type checking functions.
    * We don't really use the values there, so this is just a minimal implementation.
    */
    template <typename T>
    class WB_API OptionalParameter
    {
    public:
        /** Implicit constructor for no value case */
        inline OptionalParameter(const whiteboard::NoType&)
        {
        }

        /** Implicit constructor for the case when value is passed as parameter */
        inline OptionalParameter(T)
        {
        }
    };

    /** Helper template for resolving base type for type */
    template <typename Type> struct BaseType;

    /** Helper template for resolving optimal parameter type for type. Using base type
    * in Whiteboard API calls produces a slightly more optimized code for compilers
    * that generate separate template function instances based on signature and cannot
    * merge the instances even if they generated same code.
    */
    template <typename Type> struct ParameterType
    {
        typedef typename BaseType<typename RemoveAll<Type>::type>::type type;
    };

    /** Helper template for resolving optimal parameter type for a WBRES generated
    * parameter type that can also be optional. We could also use OptionalParameter<T>
    * to pass the value, but that would generate less efficient code. Because we know
    * the type at compile time, we resolve and use it directly.
    */
    template <typename GivenType, typename ParamType> struct OptionalParameterType;
};

// Default case is to use Value::convertTo result type
// ie. scalars as they are and others with const reference
template <typename Type>
struct Api::BaseType
{
    typedef typename Value::ResultType<Type>::type type;
};

// Enumerations are passed as their scala base types
template <typename Definition, typename DefitionType, typename ScalarBaseType>
struct Api::BaseType<TypedEnum<Definition, DefitionType, ScalarBaseType> >
{
    typedef ScalarBaseType type;
};

// Default case is to use real parameter type
template <typename GivenType, typename ParamType>
struct Api::OptionalParameterType
{
    typedef typename ParameterType<ParamType>::type type;
};

// Pass NoType as it is
template <typename ParamType>
struct Api::OptionalParameterType<NoType, ParamType>
{
    typedef NoType type;
};

} // namespace whiteboard
