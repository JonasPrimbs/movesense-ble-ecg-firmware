#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

public:

// Helper macros so that we don't need to repeat these complex cast operators everywhere

#define WB_VALUE() \
    static_cast<typename Api::ParameterType<V>::type>(rValue)
#define WB_PARAM(index) \
    static_cast<typename Api::ParameterType<WB_CONCATENATE(P, index)>::type>(WB_CONCATENATE(rP, index))

#define WB_TYPECHECKED_VALUE(OP) \
    static_cast<typename Api::OptionalParameterType< \
        V, \
        typename Api::ParameterType<typename RESOURCE::OP::NotificationType>::type>::type>(rValue)
#define WB_TYPECHECKED_PARAM(OP, index) \
    static_cast<typename Api::OptionalParameterType< \
        WB_CONCATENATE(P, index), \
        typename Api::ParameterType<typename RESOURCE::OP::Parameters::WB_CONCATENATE(Parameter, index)::Type>::type>::type>(WB_CONCATENATE(rP, index))

WB_FORCE_INLINE
template <typename V>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    LocalResourceId localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue)
{
    return updateResourceImpl(
        localResourceId,
        rOptions,
        WB_VALUE());
}

WB_FORCE_INLINE
template <typename V, typename P1>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    LocalResourceId localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1)
{
    return updateResourceImpl(
        localResourceId,
        rOptions,
        WB_VALUE(),
        WB_PARAM(1));
}

WB_FORCE_INLINE
template <typename V, typename P1, typename P2>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    LocalResourceId localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2)
{
    return updateResourceImpl(
        localResourceId,
        rOptions,
        WB_VALUE(),
        WB_PARAM(1),
        WB_PARAM(2));
}

WB_FORCE_INLINE
template <typename V, typename P1, typename P2, typename P3>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    LocalResourceId localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    return updateResourceImpl(
        localResourceId,
        rOptions,
        WB_VALUE(),
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3));
}

WB_FORCE_INLINE
template <typename V, typename P1, typename P2, typename P3, typename P4>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    LocalResourceId localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    return updateResourceImpl(
        localResourceId,
        rOptions,
        WB_VALUE(),
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4));
}

WB_FORCE_INLINE
template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    LocalResourceId localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    return updateResourceImpl(
        localResourceId,
        rOptions,
        WB_VALUE(),
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5));
}

WB_FORCE_INLINE
template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    LocalResourceId localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    return updateResourceImpl(
        localResourceId,
        rOptions,
        WB_VALUE(),
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6));
}

WB_FORCE_INLINE
template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    LocalResourceId localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    return updateResourceImpl(
        localResourceId,
        rOptions,
        WB_VALUE(),
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6),
        WB_PARAM(7));
}

WB_FORCE_INLINE
template <typename RESOURCE, typename V,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    const RESOURCE,
    const ResponseOptions& rOptions,
    const V& rValue)
{
    RESOURCE::EVENT::typeCheck(rValue);
    return updateResourceImpl(
        RESOURCE::LID | TYPE_CHECKED,
        rOptions,
        WB_VALUE());
}

WB_FORCE_INLINE
template <typename RESOURCE, typename V, typename P1,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    const RESOURCE,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1)
{
    RESOURCE::EVENT::typeCheck(rValue, rP1);
    return updateResourceImpl(
        RESOURCE::LID | TYPE_CHECKED,
        rOptions,
        WB_TYPECHECKED_VALUE(EVENT),
        WB_TYPECHECKED_PARAM(EVENT, 1));
}

WB_FORCE_INLINE
template <typename RESOURCE, typename V, typename P1, typename P2,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    const RESOURCE,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2)
{
    RESOURCE::EVENT::typeCheck(rValue, rP1, rP2);
    return updateResourceImpl(
        RESOURCE::LID | TYPE_CHECKED,
        rOptions,
        WB_TYPECHECKED_VALUE(EVENT),
        WB_TYPECHECKED_PARAM(EVENT, 1),
        WB_TYPECHECKED_PARAM(EVENT, 2));
}

WB_FORCE_INLINE
template <typename RESOURCE, typename V, typename P1, typename P2, typename P3,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    const RESOURCE,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    RESOURCE::EVENT::typeCheck(rValue, rP1, rP2, rP3);
    return updateResourceImpl(
        RESOURCE::LID | TYPE_CHECKED,
        rOptions,
        WB_TYPECHECKED_VALUE(EVENT),
        WB_TYPECHECKED_PARAM(EVENT, 1),
        WB_TYPECHECKED_PARAM(EVENT, 2),
        WB_TYPECHECKED_PARAM(EVENT, 3));
}

WB_FORCE_INLINE
template <typename RESOURCE, typename V, typename P1, typename P2, typename P3, typename P4,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    const RESOURCE,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    RESOURCE::EVENT::typeCheck(rValue, rP1, rP2, rP3, rP4);
    return updateResourceImpl(
        RESOURCE::LID | TYPE_CHECKED,
        rOptions,
        WB_TYPECHECKED_VALUE(EVENT),
        WB_TYPECHECKED_PARAM(EVENT, 1),
        WB_TYPECHECKED_PARAM(EVENT, 2),
        WB_TYPECHECKED_PARAM(EVENT, 3),
        WB_TYPECHECKED_PARAM(EVENT, 4));
}

WB_FORCE_INLINE
template <typename RESOURCE, typename V, typename P1, typename P2, typename P3, typename P4, typename P5,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    const RESOURCE,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    RESOURCE::EVENT::typeCheck(rValue, rP1, rP2, rP3, rP4, rP5);
    return updateResourceImpl(
        RESOURCE::LID | TYPE_CHECKED,
        rOptions,
        WB_TYPECHECKED_VALUE(EVENT),
        WB_TYPECHECKED_PARAM(EVENT, 1),
        WB_TYPECHECKED_PARAM(EVENT, 2),
        WB_TYPECHECKED_PARAM(EVENT, 3),
        WB_TYPECHECKED_PARAM(EVENT, 4),
        WB_TYPECHECKED_PARAM(EVENT, 5));
}

WB_FORCE_INLINE
template <typename RESOURCE, typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    const RESOURCE,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    RESOURCE::EVENT::typeCheck(rValue, rP1, rP2, rP3, rP4, rP5, rP6);
    return updateResourceImpl(
        RESOURCE::LID | TYPE_CHECKED,
        rOptions,
        WB_TYPECHECKED_VALUE(EVENT),
        WB_TYPECHECKED_PARAM(EVENT, 1),
        WB_TYPECHECKED_PARAM(EVENT, 2),
        WB_TYPECHECKED_PARAM(EVENT, 3),
        WB_TYPECHECKED_PARAM(EVENT, 4),
        WB_TYPECHECKED_PARAM(EVENT, 5),
        WB_TYPECHECKED_PARAM(EVENT, 6));
}

WB_FORCE_INLINE
template <typename RESOURCE, typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    const RESOURCE,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    RESOURCE::EVENT::typeCheck(rValue, rP1, rP2, rP3, rP4, rP5, rP6, rP7);
    return updateResourceImpl(
        RESOURCE::LID | TYPE_CHECKED,
        rOptions,
        WB_TYPECHECKED_VALUE(EVENT),
        WB_TYPECHECKED_PARAM(EVENT, 1),
        WB_TYPECHECKED_PARAM(EVENT, 2),
        WB_TYPECHECKED_PARAM(EVENT, 3),
        WB_TYPECHECKED_PARAM(EVENT, 4),
        WB_TYPECHECKED_PARAM(EVENT, 5),
        WB_TYPECHECKED_PARAM(EVENT, 6),
        WB_TYPECHECKED_PARAM(EVENT, 7));
}

WB_FORCE_INLINE
template <typename RESOURCE, typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result updateResource(
    const RESOURCE,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7,
    const P8& rP8)
{
    RESOURCE::EVENT::typeCheck(rValue, rP1, rP2, rP3, rP4, rP5, rP6, rP7, rP8);
    return updateResourceImpl(
        RESOURCE::LID | TYPE_CHECKED,
        rOptions,
        WB_TYPECHECKED_VALUE(EVENT),
        WB_TYPECHECKED_PARAM(EVENT, 1),
        WB_TYPECHECKED_PARAM(EVENT, 2),
        WB_TYPECHECKED_PARAM(EVENT, 3),
        WB_TYPECHECKED_PARAM(EVENT, 4),
        WB_TYPECHECKED_PARAM(EVENT, 5),
        WB_TYPECHECKED_PARAM(EVENT, 6),
        WB_TYPECHECKED_PARAM(EVENT, 7),
        WB_TYPECHECKED_PARAM(EVENT, 8));
}

private:

template <typename V>
inline Result updateResourceImpl(
    uint32 localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);

    return updateResourceInternal(
        localResourceId,
        rOptions,
        Value(rValue),
        ParameterList::Empty);
}

template <typename V, typename P1>
inline Result updateResourceImpl(
    uint32 localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);

    ParameterListInstance<1> parameters;
    new (&parameters[0]) Value(rP1);

    return updateResourceInternal(
        localResourceId,
        rOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2>
inline Result updateResourceImpl(
    uint32 localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);

    ParameterListInstance<2> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);

    return updateResourceInternal(
        localResourceId,
        rOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3>
inline Result updateResourceImpl(
    uint32 localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);

    ParameterListInstance<3> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);

    return updateResourceInternal(
        localResourceId,
        rOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3, typename P4>
inline Result updateResourceImpl(
    uint32 localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);

    ParameterListInstance<4> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);

    return updateResourceInternal(
        localResourceId,
        rOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5>
inline Result updateResourceImpl(
    uint32 localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);

    ParameterListInstance<5> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);

    return updateResourceInternal(
        localResourceId,
        rOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline Result updateResourceImpl(
    uint32 localResourceId,
    const ResponseOptions& rOptions,
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

    ParameterListInstance<6> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);
    new (&parameters[5]) Value(rP6);

    return updateResourceInternal(
        localResourceId,
        rOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline Result updateResourceImpl(
    uint32 localResourceId,
    const ResponseOptions& rOptions,
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

    ParameterListInstance<8> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);
    new (&parameters[5]) Value(rP6);
    new (&parameters[6]) Value(rP7);

    return updateResourceInternal(
        localResourceId,
        rOptions,
        Value(rValue),
        parameters);
}

template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline Result updateResourceImpl(
    uint32 localResourceId,
    const ResponseOptions& rOptions,
    const V& rValue,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7,
    const P8& rP8)
{
    WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_NOT_the_Value_wrapper);
    WB_STATIC_VERIFY((IsSame<V, ResponseOptions>::value == 0), Parameters_in_wrong_order);

    ParameterListInstance<8> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);
    new (&parameters[5]) Value(rP6);
    new (&parameters[6]) Value(rP7);
    new (&parameters[7]) Value(rP8);

    return updateResourceInternal(
        localResourceId,
        rOptions,
        Value(rValue),
        parameters);
}


#undef WB_VALUE
#undef WB_PARAM
#undef WB_TYPECHECKED_VALUE
#undef WB_TYPECHECKED_PARAM

public:
