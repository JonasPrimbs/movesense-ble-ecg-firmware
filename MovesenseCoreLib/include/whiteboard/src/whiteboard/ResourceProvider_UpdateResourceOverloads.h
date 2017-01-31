#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

template <typename V>
inline Result updateResource(
    ResourceId resourceId,
    const ResponseOptions& responseOptions,
    const V& rValue)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);
    WB_STATIC_VERIFY(WB_MAX_NUMBER_OF_PARAMETERS == 8, This_method_must_be_adapted_if_WB_MAX_NUMBER_OF_PARAMETERS_is_changed);

    return updateResourceInternal(
        resourceId,
        responseOptions,
        Value(rValue),
        ParameterList::Empty);
}

template <typename V, typename P1>
inline Result updateResource(
    ResourceId resourceId,
    const ResponseOptions& responseOptions,
    const V& rValue,
    const P1& rP1)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);
    WB_STATIC_VERIFY(WB_MAX_NUMBER_OF_PARAMETERS == 8, This_method_must_be_adapted_if_WB_MAX_NUMBER_OF_PARAMETERS_is_changed);

    ParameterListInstance<1> parameters;
    new (&parameters[0]) Value(rP1);

    return updateResourceInternal(
        resourceId,
        responseOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2>
inline Result updateResource(
    ResourceId resourceId,
    const ResponseOptions& responseOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);
    WB_STATIC_VERIFY(WB_MAX_NUMBER_OF_PARAMETERS == 8, This_method_must_be_adapted_if_WB_MAX_NUMBER_OF_PARAMETERS_is_changed);

    ParameterListInstance<2> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);

    return updateResourceInternal(
        resourceId,
        responseOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3>
inline Result updateResource(
    ResourceId resourceId,
    const ResponseOptions& responseOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);
    WB_STATIC_VERIFY(WB_MAX_NUMBER_OF_PARAMETERS == 8, This_method_must_be_adapted_if_WB_MAX_NUMBER_OF_PARAMETERS_is_changed);

    ParameterListInstance<3> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);

    return updateResourceInternal(
        resourceId,
        responseOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3, typename P4>
inline Result updateResource(
    ResourceId resourceId,
    const ResponseOptions& responseOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);
    WB_STATIC_VERIFY(WB_MAX_NUMBER_OF_PARAMETERS == 8, This_method_must_be_adapted_if_WB_MAX_NUMBER_OF_PARAMETERS_is_changed);

    ParameterListInstance<4> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);

    return updateResourceInternal(
        resourceId,
        responseOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5>
inline Result updateResource(
    ResourceId resourceId,
    const ResponseOptions& responseOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);
    WB_STATIC_VERIFY(WB_MAX_NUMBER_OF_PARAMETERS == 8, This_method_must_be_adapted_if_WB_MAX_NUMBER_OF_PARAMETERS_is_changed);

    ParameterListInstance<7> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);

    return updateResourceInternal(
        resourceId,
        responseOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline Result updateResource(
    ResourceId resourceId,
    const ResponseOptions& responseOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);
    WB_STATIC_VERIFY(WB_MAX_NUMBER_OF_PARAMETERS == 8, This_method_must_be_adapted_if_WB_MAX_NUMBER_OF_PARAMETERS_is_changed);

    ParameterListInstance<7> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);
    new (&parameters[5]) Value(rP6);

    return updateResourceInternal(
        resourceId,
        responseOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline Result updateResource(
    ResourceId resourceId,
    const ResponseOptions& responseOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);
    WB_STATIC_VERIFY(WB_MAX_NUMBER_OF_PARAMETERS == 8, This_method_must_be_adapted_if_WB_MAX_NUMBER_OF_PARAMETERS_is_changed);

    ParameterListInstance<7> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);
    new (&parameters[5]) Value(rP6);
    new (&parameters[6]) Value(rP7);

    return updateResourceInternal(
        resourceId,
        responseOptions,
        Value(rValue),
        parameters);
}
