#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

******************************************************************************/
/**
@file ProviderHelper.hpp
*/

#include <whiteboard/ResourceClient.h>

#define RET_IF_ERROR(a)                                                                                                          \
    do                                                                                                                           \
    {                                                                                                                            \
        whiteboard::Result _result_ = (a);                                                                                       \
        if (!RETURN_OKC(_result_))                                                                                               \
        {                                                                                                                        \
            DEBUG_ASSERT(0);                                                                                                     \
            return _result_;                                                                                                     \
        }                                                                                                                        \
    } while (0)

#define LOG_IF_ERROR(a)                                                                                                          \
    do                                                                                                                           \
    {                                                                                                                            \
        whiteboard::Result _result_ = (a);                                                                                       \
        if (!RETURN_OKC(_result_))                                                                                               \
        {                                                                                                                        \
            DEBUG_ASSERT(0);                                                                                                     \
            ;                                                                                                                    \
        }                                                                                                                        \
    } while (0)

/**
*   Helper function to call asyncSubscribe.
*   @return True if Successful.
*/
inline bool doAsyncSubscribe(
    whiteboard::ResourceClient* pClient,
    whiteboard::ResourceId resourceId,
    const whiteboard::ResourceClient::AsyncRequestOptions& rOptions = whiteboard::ResourceClient::AsyncRequestOptions::Empty)
{
    /** Call whiteboard asyncSubscribe */
    whiteboard::Result result = pClient->asyncSubscribe(resourceId, rOptions);
    return wb::RETURN_OK(result);
}

/**
*   Helper function to call asyncUnubscribe.
*   @return True if Successful.
*/
inline bool doAsyncUnsubscribe(
    whiteboard::ResourceClient* pClient,
    whiteboard::ResourceId resourceId,
    const whiteboard::ResourceClient::AsyncRequestOptions& rOptions = whiteboard::ResourceClient::AsyncRequestOptions::Empty)
{
    /** Call whiteboard asyncSubscribe */
    whiteboard::Result result = pClient->asyncUnsubscribe(resourceId, rOptions);
    return wb::RETURN_OK(result);
}

#define ONGET_CASE(a, b)                                                                                                         \
    case R_ID(a):                                                                                                                \
        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, b);                                                      \
        return
#define ONGET_CASE_EMPTY(a)                                                                                                      \
    case R_ID(a):                                                                                                                \
        returnResult(request, wb::HTTP_CODE_OK);                                                                                 \
        return
#define ONGET_CASE_TEXT(a, b)                                                                                                    \
    case R_ID(a):                                                                                                                \
        do                                                                                                                       \
        {                                                                                                                        \
            nea::Text<32> __tmp = b;                                                                                             \
            returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, __tmp.c_str());                                      \
            return;                                                                                                              \
        } while (0)
#define ONGET_CASE_TYPE(a, b, c)                                                                                                 \
    case R_ID(a):                                                                                                                \
        do                                                                                                                       \
        {                                                                                                                        \
            c __tmp = (b);                                                                                                       \
            returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, __tmp);                                              \
            return;                                                                                                              \
        } while (0)
#define RANGE_RETURN                                                                                                             \
    /* TRUE */                                                                                                                   \
    {                                                                                                                            \
        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, __tmp);                                                  \
        return;                                                                                                                  \
    }                                                                                                                            \
    else                                                                                                                         \
    /* FALSE */                                                                                                                  \
    {                                                                                                                            \
        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, wb::NoType::NoValue);                                    \
        return;                                                                                                                  \
    }
#define ONGET_CASE_TYPE_RANGE(a, b, c, min, max)                                                                                 \
    case R_ID(a):                                                                                                                \
        do                                                                                                                       \
        {                                                                                                                        \
            c __tmp = static_cast<c>(b);                                                                                         \
            if (((b) >= (min)) && ((b) <= (max))) RANGE_RETURN                                                                   \
        } while (0)
#define ONGET_CASE_TYPE_RANGE_MAX(a, b, c, max)                                                                                  \
    case R_ID(a):                                                                                                                \
        do                                                                                                                       \
        {                                                                                                                        \
            c __tmp = static_cast<c>(b);                                                                                         \
            if ((b) >= (min)) RANGE_RETURN                                                                                       \
        } while (0)
#define ONGET_CASE_FLOAT_NAN(a, b)                                                                                               \
    case R_ID(a):                                                                                                                \
        do                                                                                                                       \
        {                                                                                                                        \
            float __tmp = b;                                                                                                     \
            if (isnan(__tmp))                                                                                                    \
                returnResult(request, a::GET::HTTP_CODE_NO_CONTENT(), ResponseOptions::Empty);                                   \
            else                                                                                                                 \
                returnResult(request, a::GET::HTTP_CODE_OK(), ResponseOptions::Empty, __tmp);                                    \
            return;                                                                                                              \
        } while (0)

#define ONSUB_CASE(a, b)                                                                                                         \
    case R_ID(a):                                                                                                                \
        returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, b);                                                      \
        return
#define ONSUB_CASE_TEXT(a, b)                                                                                                    \
    case R_ID(a):                                                                                                                \
        do                                                                                                                       \
        {                                                                                                                        \
            nea::Text<32> __tmp = b;                                                                                             \
            returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, __tmp.c_str());                                      \
            return;                                                                                                              \
        } while (0)
#define ONSUB_CASE_TYPE(a, b, c)                                                                                                 \
    case R_ID(a):                                                                                                                \
        do                                                                                                                       \
        {                                                                                                                        \
            c __tmp = (b);                                                                                                       \
            returnResult(request, wb::HTTP_CODE_OK, ResponseOptions::Empty, __tmp);                                              \
            return;                                                                                                              \
        } while (0)
#define ONSUB_CASE_FLOAT_NAN(a, b)                                                                                               \
    case R_ID(a):                                                                                                                \
        do                                                                                                                       \
        {                                                                                                                        \
            float __tmp = b;                                                                                                     \
            if (isnan(__tmp))                                                                                                    \
                returnResult(request, a::SUBSCRIBE::HTTP_CODE_NO_CONTENT(), ResponseOptions::Empty);                             \
            else                                                                                                                 \
                returnResult(request, a::SUBSCRIBE::HTTP_CODE_OK(), ResponseOptions::Empty, __tmp);                              \
            return;                                                                                                              \
        } while (0)

void doAsyncSubscribeResources(whiteboard::ResourceClient* pClient,
                               const whiteboard::ResourceId resourceIds[],
                               const uint8_t size,
                               const bool criticalResources = true);
void doAsyncUnsubscribeResources(whiteboard::ResourceClient* pClient,
                                 const whiteboard::ResourceId resourceIds[],
                                 const uint8_t size);

/** Interface for easily subscribing to multiple resources at once.
* TODO: Probably should be moved to be as whiteboard as registerProviderResources is. */
template <uint8_t size>
void doAsyncSubscribeResources(whiteboard::ResourceClient* pClient,
                               const whiteboard::ResourceId (&resourceIds)[size],
                               const bool criticalResources = true)
{
    doAsyncSubscribeResources(pClient, resourceIds, size, criticalResources);
}

/** Interface for easily unsubscribing from multiple resources at once.
* TODO: Probably should be moved to be as whiteboard as unregisterProviderResources is. */
template <uint8_t size>
void doAsyncUnsubscribeResources(whiteboard::ResourceClient* pClient, const whiteboard::ResourceId (&resourceIds)[size])
{
    return doAsyncUnsubscribeResources(pClient, resourceIds, size);
}

inline bool isDebugBuild()
{
#ifdef BUILD_WITH_DEBUG
    return true;
#else
    return false;
#endif
}
