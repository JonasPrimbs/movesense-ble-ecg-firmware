#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

private:

// Helper macros so that we don't need to repeat these complex cast operators everywhere

#define WB_PARAM(index) \
    static_cast<typename Api::ParameterType<WB_CONCATENATE(P, index)>::type>(WB_CONCATENATE(rP, index))

#define WB_TYPECHECKED_PARAM(OP, index) \
    static_cast<typename Api::OptionalParameterType< \
        WB_CONCATENATE(P, index), \
        typename Api::ParameterType<typename RESOURCE::OP::Parameters::WB_CONCATENATE(Parameter, index)::Type>::type>::type>(WB_CONCATENATE(rP, index))

WB_FORCE_INLINE
WB_FORCE_INLINE_ATTRIBUTE
static RequestType typeChecked(RequestType requestType)
{
    return static_cast<RequestType>(requestType | TYPE_CHECKED);
}

public:

WB_FORCE_INLINE
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequest(resourceId, REQUEST_GET, rOptions);
}

WB_FORCE_INLINE
template <typename P1>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    return asyncRequest(
        resourceId,
        REQUEST_GET,
        rOptions,
        WB_PARAM(1));
}

WB_FORCE_INLINE
template <typename P1, typename P2>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    return asyncRequest(
        resourceId,
        REQUEST_GET,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    return asyncRequest(
        resourceId,
        REQUEST_GET,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    return asyncRequest(
        resourceId,
        REQUEST_GET,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    return asyncRequest(
        resourceId,
        REQUEST_GET,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    return asyncRequest(
        resourceId,
        REQUEST_GET,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    return asyncRequest(
        resourceId,
        REQUEST_GET,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6),
        WB_PARAM(7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    const RESOURCE,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    RESOURCE::GET::typeCheck();
    return asyncRequest(RESOURCE::ID, typeChecked(REQUEST_GET), rOptions);
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    RESOURCE::GET::typeCheck(rP1);
    return asyncRequest(
        RESOURCE::ID,
        typeChecked(REQUEST_GET),
        rOptions,
        WB_TYPECHECKED_PARAM(GET, 1));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    RESOURCE::GET::typeCheck(rP1, rP2);
    return asyncRequest(
        RESOURCE::ID,
        typeChecked(REQUEST_GET),
        rOptions,
        WB_TYPECHECKED_PARAM(GET, 1),
        WB_TYPECHECKED_PARAM(GET, 2));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    RESOURCE::GET::typeCheck(rP1, rP2, rP3);
    return asyncRequest(
        RESOURCE::ID,
        typeChecked(REQUEST_GET),
        rOptions,
        WB_TYPECHECKED_PARAM(GET, 1),
        WB_TYPECHECKED_PARAM(GET, 2),
        WB_TYPECHECKED_PARAM(GET, 3));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4)
{
    RESOURCE::GET::typeCheck(rP1, rP2, rP3, rP4);
    return asyncRequest(
        RESOURCE::ID,
        typeChecked(REQUEST_GET),
        rOptions,
        WB_TYPECHECKED_PARAM(GET, 1),
        WB_TYPECHECKED_PARAM(GET, 2),
        WB_TYPECHECKED_PARAM(GET, 3),
        WB_TYPECHECKED_PARAM(GET, 4));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5)
{
    RESOURCE::GET::typeCheck(rP1, rP2, rP3, rP4, rP5);
    return asyncRequest(
        RESOURCE::ID,
        typeChecked(REQUEST_GET),
        rOptions,
        WB_TYPECHECKED_PARAM(GET, 1),
        WB_TYPECHECKED_PARAM(GET, 2),
        WB_TYPECHECKED_PARAM(GET, 3),
        WB_TYPECHECKED_PARAM(GET, 4),
        WB_TYPECHECKED_PARAM(GET, 5));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6)
{
    RESOURCE::GET::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6);
    return asyncRequest(
        RESOURCE::ID,
        typeChecked(REQUEST_GET),
        rOptions,
        WB_TYPECHECKED_PARAM(GET, 1),
        WB_TYPECHECKED_PARAM(GET, 2),
        WB_TYPECHECKED_PARAM(GET, 3),
        WB_TYPECHECKED_PARAM(GET, 4),
        WB_TYPECHECKED_PARAM(GET, 5),
        WB_TYPECHECKED_PARAM(GET, 6));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7)
{
    RESOURCE::GET::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7);
    return asyncRequest(
        RESOURCE::ID,
        typeChecked(REQUEST_GET),
        rOptions,
        WB_TYPECHECKED_PARAM(GET, 1),
        WB_TYPECHECKED_PARAM(GET, 2),
        WB_TYPECHECKED_PARAM(GET, 3),
        WB_TYPECHECKED_PARAM(GET, 4),
        WB_TYPECHECKED_PARAM(GET, 5),
        WB_TYPECHECKED_PARAM(GET, 6),
        WB_TYPECHECKED_PARAM(GET, 7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncGet(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7,
    const P2& rP8)
{
    RESOURCE::GET::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7, rP8);
    return asyncRequest(
        RESOURCE::ID,
        typeChecked(REQUEST_GET),
        rOptions,
        WB_TYPECHECKED_PARAM(GET, 1),
        WB_TYPECHECKED_PARAM(GET, 2),
        WB_TYPECHECKED_PARAM(GET, 3),
        WB_TYPECHECKED_PARAM(GET, 4),
        WB_TYPECHECKED_PARAM(GET, 5),
        WB_TYPECHECKED_PARAM(GET, 6),
        WB_TYPECHECKED_PARAM(GET, 7),
        WB_TYPECHECKED_PARAM(GET, 8));
}

WB_FORCE_INLINE
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequest(resourceId, REQUEST_PUT, rOptions);
}

WB_FORCE_INLINE
template <typename P1>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    return asyncRequest(
        resourceId,
        REQUEST_PUT,
        rOptions,
        WB_PARAM(1));
}

WB_FORCE_INLINE
template <typename P1, typename P2>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    return asyncRequest(
        resourceId,
        REQUEST_PUT,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    return asyncRequest(
        resourceId,
        REQUEST_PUT,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    return asyncRequest(
        resourceId,
        REQUEST_PUT,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    return asyncRequest(
        resourceId,
        REQUEST_PUT,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    return asyncRequest(
        resourceId,
        REQUEST_PUT,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    return asyncRequest(
        resourceId,
        REQUEST_PUT,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6),
        WB_PARAM(7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    const RESOURCE,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    RESOURCE::PUT::typeCheck();
    return asyncRequest(RESOURCE::ID, REQUEST_PUT, rOptions);
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    RESOURCE::PUT::typeCheck(rP1);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_PUT, 
        rOptions,
        WB_TYPECHECKED_PARAM(PUT, 1));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    RESOURCE::PUT::typeCheck(rP1, rP2);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_PUT, 
        rOptions,
        WB_TYPECHECKED_PARAM(PUT, 1),
        WB_TYPECHECKED_PARAM(PUT, 2));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    RESOURCE::PUT::typeCheck(rP1, rP2, rP3);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_PUT,
        rOptions,
        WB_TYPECHECKED_PARAM(PUT, 1),
        WB_TYPECHECKED_PARAM(PUT, 2),
        WB_TYPECHECKED_PARAM(PUT, 3));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4)
{
    RESOURCE::PUT::typeCheck(rP1, rP2, rP3, rP4);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_PUT,
        rOptions,
        WB_TYPECHECKED_PARAM(PUT, 1),
        WB_TYPECHECKED_PARAM(PUT, 2),
        WB_TYPECHECKED_PARAM(PUT, 3),
        WB_TYPECHECKED_PARAM(PUT, 4));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5)
{
    RESOURCE::PUT::typeCheck(rP1, rP2, rP3, rP4, rP5);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_PUT,
        rOptions,
        WB_TYPECHECKED_PARAM(PUT, 1),
        WB_TYPECHECKED_PARAM(PUT, 2),
        WB_TYPECHECKED_PARAM(PUT, 3),
        WB_TYPECHECKED_PARAM(PUT, 4),
        WB_TYPECHECKED_PARAM(PUT, 5));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6)
{
    RESOURCE::PUT::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_PUT,
        rOptions,
        WB_TYPECHECKED_PARAM(PUT, 1),
        WB_TYPECHECKED_PARAM(PUT, 2),
        WB_TYPECHECKED_PARAM(PUT, 3),
        WB_TYPECHECKED_PARAM(PUT, 4),
        WB_TYPECHECKED_PARAM(PUT, 5),
        WB_TYPECHECKED_PARAM(PUT, 6));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7)
{
    RESOURCE::PUT::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_PUT,
        rOptions,
        WB_TYPECHECKED_PARAM(PUT, 1),
        WB_TYPECHECKED_PARAM(PUT, 2),
        WB_TYPECHECKED_PARAM(PUT, 3),
        WB_TYPECHECKED_PARAM(PUT, 4),
        WB_TYPECHECKED_PARAM(PUT, 5),
        WB_TYPECHECKED_PARAM(PUT, 6),
        WB_TYPECHECKED_PARAM(PUT, 7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPut(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7,
    const P2& rP8)
{
    RESOURCE::PUT::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7, rP8);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_PUT,
        rOptions,
        WB_TYPECHECKED_PARAM(PUT, 1),
        WB_TYPECHECKED_PARAM(PUT, 2),
        WB_TYPECHECKED_PARAM(PUT, 3),
        WB_TYPECHECKED_PARAM(PUT, 4),
        WB_TYPECHECKED_PARAM(PUT, 5),
        WB_TYPECHECKED_PARAM(PUT, 6),
        WB_TYPECHECKED_PARAM(PUT, 7),
        WB_TYPECHECKED_PARAM(PUT, 8));
}

WB_FORCE_INLINE
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequest(resourceId, REQUEST_POST, rOptions);
}

WB_FORCE_INLINE
template <typename P1>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    return asyncRequest(
        resourceId,
        REQUEST_POST,
        rOptions,
        WB_PARAM(1));
}

WB_FORCE_INLINE
template <typename P1, typename P2>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    return asyncRequest(
        resourceId,
        REQUEST_POST,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    return asyncRequest(
        resourceId,
        REQUEST_POST,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    return asyncRequest(
        resourceId,
        REQUEST_POST,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    return asyncRequest(
        resourceId,
        REQUEST_POST,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    return asyncRequest(
        resourceId,
        REQUEST_POST,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    return asyncRequest(
        resourceId,
        REQUEST_POST,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6),
        WB_PARAM(7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    const RESOURCE,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    RESOURCE::POST::typeCheck();
    return asyncRequest(RESOURCE::ID, REQUEST_POST, rOptions);
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    RESOURCE::POST::typeCheck(rP1);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_POST,
        rOptions,
        WB_TYPECHECKED_PARAM(POST, 1));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    RESOURCE::POST::typeCheck(rP1, rP2);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_POST,
        rOptions,
        WB_TYPECHECKED_PARAM(POST, 1),
        WB_TYPECHECKED_PARAM(POST, 2));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    RESOURCE::POST::typeCheck(rP1, rP2, rP3);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_POST,
        rOptions,
        WB_TYPECHECKED_PARAM(POST, 1),
        WB_TYPECHECKED_PARAM(POST, 2),
        WB_TYPECHECKED_PARAM(POST, 3));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4)
{
    RESOURCE::POST::typeCheck(rP1, rP2, rP3, rP4);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_POST,
        rOptions,
        WB_TYPECHECKED_PARAM(POST, 1),
        WB_TYPECHECKED_PARAM(POST, 2),
        WB_TYPECHECKED_PARAM(POST, 3),
        WB_TYPECHECKED_PARAM(POST, 4));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5)
{
    RESOURCE::POST::typeCheck(rP1, rP2, rP3, rP4, rP5);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_POST,
        rOptions,
        WB_TYPECHECKED_PARAM(POST, 1),
        WB_TYPECHECKED_PARAM(POST, 2),
        WB_TYPECHECKED_PARAM(POST, 3),
        WB_TYPECHECKED_PARAM(POST, 4),
        WB_TYPECHECKED_PARAM(POST, 5));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6)
{
    RESOURCE::POST::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_POST, 
        rOptions,
        WB_TYPECHECKED_PARAM(POST, 1),
        WB_TYPECHECKED_PARAM(POST, 2),
        WB_TYPECHECKED_PARAM(POST, 3),
        WB_TYPECHECKED_PARAM(POST, 4),
        WB_TYPECHECKED_PARAM(POST, 5),
        WB_TYPECHECKED_PARAM(POST, 6));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7)
{
    RESOURCE::POST::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_POST,
        rOptions,
        WB_TYPECHECKED_PARAM(POST, 1),
        WB_TYPECHECKED_PARAM(POST, 2),
        WB_TYPECHECKED_PARAM(POST, 3),
        WB_TYPECHECKED_PARAM(POST, 4),
        WB_TYPECHECKED_PARAM(POST, 5),
        WB_TYPECHECKED_PARAM(POST, 6),
        WB_TYPECHECKED_PARAM(POST, 7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncPost(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7,
    const P2& rP8)
{
    RESOURCE::POST::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7, rP8);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_POST, 
        rOptions,
        WB_TYPECHECKED_PARAM(POST, 1),
        WB_TYPECHECKED_PARAM(POST, 2),
        WB_TYPECHECKED_PARAM(POST, 3),
        WB_TYPECHECKED_PARAM(POST, 4),
        WB_TYPECHECKED_PARAM(POST, 5),
        WB_TYPECHECKED_PARAM(POST, 6),
        WB_TYPECHECKED_PARAM(POST, 7),
        WB_TYPECHECKED_PARAM(POST, 8));
}

WB_FORCE_INLINE
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequest(resourceId, REQUEST_DELETE, rOptions);
}

WB_FORCE_INLINE
template <typename P1>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    return asyncRequest(
        resourceId,
        REQUEST_DELETE,
        rOptions,
        WB_PARAM(1));
}

WB_FORCE_INLINE
template <typename P1, typename P2>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    return asyncRequest(
        resourceId,
        REQUEST_DELETE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    return asyncRequest(
        resourceId,
        REQUEST_DELETE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    return asyncRequest(
        resourceId,
        REQUEST_DELETE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    return asyncRequest(
        resourceId,
        REQUEST_DELETE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    return asyncRequest(
        resourceId,
        REQUEST_DELETE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    return asyncRequest(
        resourceId,
        REQUEST_DELETE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6),
        WB_PARAM(7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    const RESOURCE,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    RESOURCE::DELETE::typeCheck();
    return asyncRequest(RESOURCE::ID, REQUEST_DELETE, rOptions);
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    RESOURCE::DELETE::typeCheck(rP1);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_DELETE, 
        rOptions,
        WB_TYPECHECKED_PARAM(DELETE, 1));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    RESOURCE::DELETE::typeCheck(rP1, rP2);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_DELETE, 
        rOptions,
        WB_TYPECHECKED_PARAM(DELETE, 1),
        WB_TYPECHECKED_PARAM(DELETE, 2));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    RESOURCE::DELETE::typeCheck(rP1, rP2, rP3);
    return asyncRequest(
        RESOURCE::ID, 
        REQUEST_DELETE, 
        rOptions,
        WB_TYPECHECKED_PARAM(DELETE, 1),
        WB_TYPECHECKED_PARAM(DELETE, 2),
        WB_TYPECHECKED_PARAM(DELETE, 3));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4)
{
    RESOURCE::DELETE::typeCheck(rP1, rP2, rP3, rP4);
    return asyncRequest(
        RESOURCE::ID, 
        REQUEST_DELETE, 
        rOptions,
        WB_TYPECHECKED_PARAM(DELETE, 1),
        WB_TYPECHECKED_PARAM(DELETE, 2),
        WB_TYPECHECKED_PARAM(DELETE, 3),
        WB_TYPECHECKED_PARAM(DELETE, 4));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5)
{
    RESOURCE::DELETE::typeCheck(rP1, rP2, rP3, rP4, rP5);
    return asyncRequest(
        RESOURCE::ID, 
        REQUEST_DELETE, 
        rOptions,
        WB_TYPECHECKED_PARAM(DELETE, 1),
        WB_TYPECHECKED_PARAM(DELETE, 2),
        WB_TYPECHECKED_PARAM(DELETE, 3),
        WB_TYPECHECKED_PARAM(DELETE, 4),
        WB_TYPECHECKED_PARAM(DELETE, 5));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6)
{
    RESOURCE::DELETE::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6);
    return asyncRequest(
        RESOURCE::ID, 
        REQUEST_DELETE, 
        rOptions,
        WB_TYPECHECKED_PARAM(DELETE, 1),
        WB_TYPECHECKED_PARAM(DELETE, 2),
        WB_TYPECHECKED_PARAM(DELETE, 3),
        WB_TYPECHECKED_PARAM(DELETE, 4),
        WB_TYPECHECKED_PARAM(DELETE, 5),
        WB_TYPECHECKED_PARAM(DELETE, 6));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7)
{
    RESOURCE::DELETE::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_DELETE, 
        rOptions,
        WB_TYPECHECKED_PARAM(DELETE, 1),
        WB_TYPECHECKED_PARAM(DELETE, 2),
        WB_TYPECHECKED_PARAM(DELETE, 3),
        WB_TYPECHECKED_PARAM(DELETE, 4),
        WB_TYPECHECKED_PARAM(DELETE, 5),
        WB_TYPECHECKED_PARAM(DELETE, 6),
        WB_TYPECHECKED_PARAM(DELETE, 7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncDelete(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7,
    const P2& rP8)
{
    RESOURCE::DELETE::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7, rP8);
    return asyncRequest(
        RESOURCE::ID, 
        REQUEST_DELETE, 
        rOptions,
        WB_TYPECHECKED_PARAM(DELETE, 1),
        WB_TYPECHECKED_PARAM(DELETE, 2),
        WB_TYPECHECKED_PARAM(DELETE, 3),
        WB_TYPECHECKED_PARAM(DELETE, 4),
        WB_TYPECHECKED_PARAM(DELETE, 5),
        WB_TYPECHECKED_PARAM(DELETE, 6),
        WB_TYPECHECKED_PARAM(DELETE, 7),
        WB_TYPECHECKED_PARAM(DELETE, 8));
}

WB_FORCE_INLINE
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequest(resourceId, REQUEST_SUBSCRIBE, rOptions);
}

WB_FORCE_INLINE
template <typename P1>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    return asyncRequest(
        resourceId,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_PARAM(1));
}

WB_FORCE_INLINE
template <typename P1, typename P2>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    return asyncRequest(
        resourceId,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    return asyncRequest(
        resourceId,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    return asyncRequest(
        resourceId,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    return asyncRequest(
        resourceId,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    return asyncRequest(
        resourceId,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    return asyncRequest(
        resourceId,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6),
        WB_PARAM(7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    RESOURCE::SUBSCRIBE::typeCheck();
    return asyncRequest(RESOURCE::ID, REQUEST_SUBSCRIBE, rOptions);
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    RESOURCE::SUBSCRIBE::typeCheck(rP1);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 1));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    RESOURCE::SUBSCRIBE::typeCheck(rP1, rP2);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 2));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    RESOURCE::SUBSCRIBE::typeCheck(rP1, rP2, rP3);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 3));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4)
{
    RESOURCE::SUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 4));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5)
{
    RESOURCE::SUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4, rP5);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 4),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 5));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6)
{
    RESOURCE::SUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 4),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 5),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 6));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7)
{
    RESOURCE::SUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 4),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 5),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 6),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncSubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7,
    const P2& rP8)
{
    RESOURCE::SUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7, rP8);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_SUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 4),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 5),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 6),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 7),
        WB_TYPECHECKED_PARAM(SUBSCRIBE, 8));
}

WB_FORCE_INLINE
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequest(resourceId, REQUEST_UNSUBSCRIBE, rOptions);
}

WB_FORCE_INLINE
template <typename P1>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    return asyncRequest(
        resourceId,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_PARAM(1));
}

WB_FORCE_INLINE
template <typename P1, typename P2>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    return asyncRequest(
        resourceId,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    return asyncRequest(
        resourceId,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    return asyncRequest(
        resourceId,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    return asyncRequest(
        resourceId,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    return asyncRequest(
        resourceId,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6));
}

WB_FORCE_INLINE
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    return asyncRequest(
        resourceId,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_PARAM(1),
        WB_PARAM(2),
        WB_PARAM(3),
        WB_PARAM(4),
        WB_PARAM(5),
        WB_PARAM(6),
        WB_PARAM(7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    RESOURCE::UNSUBSCRIBE::typeCheck();
    return asyncRequest(RESOURCE::ID, REQUEST_UNSUBSCRIBE, rOptions);
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    RESOURCE::UNSUBSCRIBE::typeCheck(rP1);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 1));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    RESOURCE::UNSUBSCRIBE::typeCheck(rP1, rP2);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 2));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    RESOURCE::UNSUBSCRIBE::typeCheck(rP1, rP2, rP3);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 3));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4)
{
    RESOURCE::UNSUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 4));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5)
{
    RESOURCE::UNSUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4, rP5);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 4),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 5));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6)
{
    RESOURCE::UNSUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 4),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 5),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 6));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7)
{
    RESOURCE::UNSUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 4),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 5),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 6),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 7));
}

WB_FORCE_INLINE
template <
    typename RESOURCE, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
    typename EnableIf<!IsSame<RESOURCE, ResourceId>::value && !IsSame<RESOURCE, ResourceId::Value>::value, int>::type = 0>
WB_FORCE_INLINE_ATTRIBUTE Result asyncUnsubscribe(
    const RESOURCE,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P1& rP4,
    const P2& rP5,
    const P3& rP6,
    const P1& rP7,
    const P2& rP8)
{
    RESOURCE::UNSUBSCRIBE::typeCheck(rP1, rP2, rP3, rP4, rP5, rP6, rP7, rP8);
    return asyncRequest(
        RESOURCE::ID,
        REQUEST_UNSUBSCRIBE,
        rOptions,
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 1),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 2),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 3),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 4),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 5),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 6),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 7),
        WB_TYPECHECKED_PARAM(UNSUBSCRIBE, 8));
}

private:

inline Result asyncRequest(
    ResourceId resourceId,
    RequestType requestType,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequestInternal(
        resourceId,
        requestType,
        rOptions,
        ParameterList::Empty);
}

template <typename P1>
inline Result asyncRequest(
    ResourceId resourceId,
    RequestType requestType,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    ParameterListInstance<1> parameters;
    new (&parameters[0]) Value(rP1);

    return asyncRequestInternal(
        resourceId,
        requestType,
        rOptions,
        parameters);
}

template <typename P1, typename P2>
inline Result asyncRequest(
    ResourceId resourceId,
    RequestType requestType,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    ParameterListInstance<2> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);

    return asyncRequestInternal(
        resourceId,
        requestType,
        rOptions,
        parameters);
}

template <typename P1, typename P2, typename P3>
inline Result asyncRequest(
    ResourceId resourceId,
    RequestType requestType,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3)
{
    ParameterListInstance<3> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);

    return asyncRequestInternal(
        resourceId,
        requestType,
        rOptions,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4>
inline Result asyncRequest(
    ResourceId resourceId,
    RequestType requestType,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4)
{
    ParameterListInstance<4> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);

    return asyncRequestInternal(
        resourceId,
        requestType,
        rOptions,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5>
inline Result asyncRequest(
    ResourceId resourceId,
    RequestType requestType,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5)
{
    ParameterListInstance<5> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);

    return asyncRequestInternal(
        resourceId,
        requestType,
        rOptions,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline Result asyncRequest(
    ResourceId resourceId,
    RequestType requestType,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6)
{
    ParameterListInstance<6> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);
    new (&parameters[5]) Value(rP6);

    return asyncRequestInternal(
        resourceId,
        requestType,
        rOptions,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline Result asyncRequest(
    ResourceId resourceId,
    RequestType requestType,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7)
{
    ParameterListInstance<7> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);
    new (&parameters[5]) Value(rP6);
    new (&parameters[6]) Value(rP7);

    return asyncRequestInternal(
        resourceId,
        requestType,
        rOptions,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline Result asyncRequest(
    ResourceId resourceId,
    RequestType requestType,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2,
    const P3& rP3,
    const P4& rP4,
    const P5& rP5,
    const P6& rP6,
    const P7& rP7,
    const P8& rP8)
{
    ParameterListInstance<8> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);
    new (&parameters[2]) Value(rP3);
    new (&parameters[3]) Value(rP4);
    new (&parameters[4]) Value(rP5);
    new (&parameters[5]) Value(rP6);
    new (&parameters[6]) Value(rP7);
    new (&parameters[7]) Value(rP8);

    return asyncRequestInternal(
        resourceId,
        requestType, 
        rOptions,
        parameters);
}

#undef WB_PARAM
#undef WB_TYPECHECKED_PARAM

public:
