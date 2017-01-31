#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

inline Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_GET,
        ParameterList::Empty);
}

template <typename P1>
inline Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    ParameterListInstance<1> parameters;
    new (&parameters[0]) Value(rP1);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_GET,
        parameters);
}

template <typename P1, typename P2>
inline Result asyncGet(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    ParameterListInstance<2> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_GET,
        parameters);
}

template <typename P1, typename P2, typename P3>
inline Result asyncGet(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_GET,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4>
inline Result asyncGet(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_GET,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5>
inline Result asyncGet(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_GET,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline Result asyncGet(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_GET,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline Result asyncGet(
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
        rOptions,
        REQUEST_GET,
        parameters);
}

inline Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_PUT,
        ParameterList::Empty);
}

template <typename P1>
inline Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    ParameterListInstance<1> parameters;
    new (&parameters[0]) Value(rP1);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_PUT,
        parameters);
}

template <typename P1, typename P2>
inline Result asyncPut(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    ParameterListInstance<2> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_PUT,
        parameters);
}

template <typename P1, typename P2, typename P3>
inline Result asyncPut(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_PUT,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4>
inline Result asyncPut(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_PUT,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5>
inline Result asyncPut(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_PUT,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline Result asyncPut(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_PUT,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline Result asyncPut(
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
        rOptions,
        REQUEST_PUT,
        parameters);
}

inline Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_POST,
        ParameterList::Empty);
}

template <typename P1>
inline Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    ParameterListInstance<1> parameters;
    new (&parameters[0]) Value(rP1);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_POST,
        parameters);
}

template <typename P1, typename P2>
inline Result asyncPost(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    ParameterListInstance<2> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_POST,
        parameters);
}

template <typename P1, typename P2, typename P3>
inline Result asyncPost(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_POST,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4>
inline Result asyncPost(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_POST,
        parameters);
}

inline Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_DELETE,
        ParameterList::Empty);
}

template <typename P1>
inline Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    ParameterListInstance<1> parameters;
    new (&parameters[0]) Value(rP1);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_DELETE,
        parameters);
}

template <typename P1, typename P2>
inline Result asyncDelete(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    ParameterListInstance<2> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_DELETE,
        parameters);
}

template <typename P1, typename P2, typename P3>
inline Result asyncDelete(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_DELETE,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4>
inline Result asyncDelete(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_DELETE,
        parameters);
}

inline Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_SUBSCRIBE,
        ParameterList::Empty);
}

template <typename P1>
inline Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    ParameterListInstance<1> parameters;
    new (&parameters[0]) Value(rP1);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_SUBSCRIBE,
        parameters);
}

template <typename P1, typename P2>
inline Result asyncSubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    ParameterListInstance<2> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_SUBSCRIBE,
        parameters);
}

template <typename P1, typename P2, typename P3>
inline Result asyncSubscribe(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_SUBSCRIBE,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4>
inline Result asyncSubscribe(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_SUBSCRIBE,
        parameters);
}

inline Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions = AsyncRequestOptions::Empty)
{
    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_UNSUBSCRIBE,
        ParameterList::Empty);
}

template <typename P1>
inline Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1)
{
    ParameterListInstance<1> parameters;
    new (&parameters[0]) Value(rP1);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_UNSUBSCRIBE,
        parameters);
}

template <typename P1, typename P2>
inline Result asyncUnsubscribe(
    ResourceId resourceId,
    const AsyncRequestOptions& rOptions,
    const P1& rP1,
    const P2& rP2)
{
    ParameterListInstance<2> parameters;
    new (&parameters[0]) Value(rP1);
    new (&parameters[1]) Value(rP2);

    return asyncRequestInternal(
        resourceId,
        rOptions,
        REQUEST_UNSUBSCRIBE,
        parameters);
}

template <typename P1, typename P2, typename P3>
inline Result asyncUnsubscribe(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_UNSUBSCRIBE,
        parameters);
}

template <typename P1, typename P2, typename P3, typename P4>
inline Result asyncUnsubscribe(
    ResourceId resourceId,
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
        rOptions,
        REQUEST_UNSUBSCRIBE,
        parameters);
}
