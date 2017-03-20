#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <gtest/gtest.h>
#include "whiteboard/ResourceClient.h"
#include "whiteboard/DpcFunctor.h"

#if WB_UNITTEST_BUILD

#if _MSC_VER
#define strdup _strdup
#endif

namespace whiteboard
{

/// Wait until client has got a result.
/// @return true if result was received, false if not.
template <class TClient> bool WaitOneResult(TClient& client)
{
    for (size_t i = 0; i < 2000; ++i)
    {
        if (client.hasResultBeenReceived())
            return true;

        WbThreadSleepMs(1);
    }
    return false;
}

/// Wait until client has got a notification.
/// @return true if result was received, false if not.
template <class TClient> bool WaitUntilNotification(TClient& client, size_t notificationCount)
{
    for (size_t i = 0; i < 2000; ++i)
    {
        if (client.getNotificationCount() >= notificationCount)
            return true;

        WbThreadSleepMs(1);
    }
    return false;
}

template <class TProvider> bool WaitUntilRegistered(TProvider& provider)
{
    for (size_t i = 0; i < 2000; ++i)
    {
        if (provider.isRegistered())
            return true;

        WbThreadSleepMs(1);
    }
    return false;
}


/** Base class that stores client request results */
class TestResultBase
{
public:
    TestResultBase()
        : mResourceId(ID_INVALID_RESOURCE),
        mCallResultCode(HTTP_CODE_INTERNAL_SERVER_ERROR),
        mReturnResultCode(HTTP_CODE_INTERNAL_SERVER_ERROR),
        mValueDataTypeId(0),
        mHasValue(false)
    {
    }

    inline operator bool() const
    {
        return RETURN_OK(mCallResultCode) && mValidResponse && RETURN_OK(mReturnResultCode);
    }

    inline bool checkGetResourceResult(const ResourceId expectedResourceId) const
    {
        return *this && (expectedResourceId == mResourceId);
    }

    inline bool checkAsyncCallResult(Result expectedCallResult) const
    {
        return expectedCallResult == mCallResultCode;
    }

    inline bool checkResult(Result expectedReturnResult) const
    {
        return mValidResponse && RETURN_OK(mCallResultCode) && (expectedReturnResult == mReturnResultCode);
    }

    inline bool checkValueType(ValueType expectedValueType) const
    {
        return RETURN_OK(mCallResultCode) && (expectedValueType == Value::getType(mValueDataTypeId));
    }

    ResourceId mResourceId;
    Result mCallResultCode;
    bool mValidResponse;
    Result mReturnResultCode;
    LocalDataTypeId mValueDataTypeId;
    bool mHasValue;
};

/** Class that stores client request results */
template <typename R>
class TestResult : public TestResultBase
{
public:
    TestResult()
    {
    }

    inline TestResult<R> operator=(const TestResult<R>& rResult)
    {
        mResourceId = rResult.mResourceId;
        mCallResultCode = rResult.mCallResultCode;
        mValidResponse = rResult.mValidResponse;
        mReturnResultCode = rResult.mReturnResultCode;
        mValueDataTypeId = rResult.mValueDataTypeId;
        mHasValue = rResult.mHasValue;
        mValue = rResult.mValue;
        return *this;
    }

    inline bool checkValue(const R& expectedValue) const
    {
        return *this && mHasValue && (expectedValue == mValue);
    }

    inline void setValue(const typename Value::ConvertResult<R>::type rValue)
    {
        mHasValue = true;
        mValue = rValue;
    }

    R mValue;
};

/** Base class for specialized versions that allocates memory for string results */
class StringTestResultBase : public TestResultBase
{
public:
    StringTestResultBase()
        : mValue(NULL)
    {
    }

    virtual ~StringTestResultBase()
    {
        if (mValue)
        {
            free(mValue);
        }
    }

    inline bool checkValue(const char* expectedValue) const
    {
        if (!*this || !mHasValue) return false;
        if (expectedValue == NULL) return mValue == NULL;
        if (mValue == NULL) return false;
        return !strcmp(expectedValue, mValue);
    }

    inline void setValue(const char* rValue)
    {
        mHasValue = true;
        if (rValue != NULL)
        {
            mValue = strdup(rValue);
        }
    }

    char* mValue;
};

template <>
class TestResult<char*> : public StringTestResultBase
{
public:
    inline TestResult()
    {
    }

    inline TestResult(const TestResult<char*>& other)
    {
        *this = other;
    }

    inline TestResult<char*>& operator=(const TestResult<char*>& other)
    {
        *static_cast<TestResultBase*>(this) = static_cast<const TestResultBase&>(other);

        if (mValue)
        {
            free(mValue);
            mValue = NULL;
        }

        if (other.mValue != NULL)
        {
            mValue = strdup(other.mValue);
        }

        return *this;
    }
};

template <>
class TestResult<const char*> : public StringTestResultBase
{
public:
    inline TestResult()
    {
    }

    inline TestResult(const TestResult<const char*>& other)
    {
        *this = other;
    }

    inline TestResult<const char*>& operator=(const TestResult<const char*>& other)
    {
        *static_cast<TestResultBase*>(this) = static_cast<const TestResultBase&>(other);

        if (mValue)
        {
            free(mValue);
            mValue = NULL;
        }

        if (other.mValue != NULL)
        {
            mValue = strdup(other.mValue);
        }

        return *this;
    }
};

/** GTEST helpers */
#define EXPECT_WBGETRESOURCERESULT(expectedResId, testResult)  EXPECT_TRUE(testResult.checkGetResourceResult(expectedResId))
#define EXPECT_WBCALLRESULT(expectedCallResult, asyncTestResult)  EXPECT_TRUE(asyncTestResult.checkAsyncCallResult(expectedCallResult))
#define EXPECT_WBRESULT(expectedReturnResult, testResult)  EXPECT_TRUE(testResult.checkResult(expectedReturnResult))
#define EXPECT_WBVALUETYPE(expectedValueType, testResult)  EXPECT_TRUE(testResult.checkValueType(expectedValueType))
#define EXPECT_WBVALUE(expectedValue, testResult)  EXPECT_TRUE(testResult.checkValue(expectedValue))
#define EXPECT_WBVALUESTR(expectedValue, testResult)  EXPECT_TRUE(testResult.checkValue(expectedValue))

/** Helper value to indicate that TestClientWithDefaults should use resource ID that was resolved with previous
 * callGetResource or callAsyncGetResourceAndWait
 */
extern const ResourceId CURRENT_RESOURCE;

/**
*	Test client class.
*/
template <
    typename DefaultGetReturnType = NoType, typename DefaultPutReturnType = NoType,
    typename DefaultPostReturnType = NoType, typename DefaultDeleteReturnType = NoType,
    typename DefaultSubscribeReturnType = NoType, typename DefaultUnsubscribeReturnType = NoType>
class TestClientWithDefaults : public ResourceClient
{
public:
    /**
    *	Constructor
    *	@param ExecutionContextId Client's execution context ID.
    */
    TestClientWithDefaults(ExecutionContextId executionContextId)
        : ResourceClient(WBDEBUG_NAME(__FUNCTION__), executionContextId),
          mTestResourceId(ID_INVALID_RESOURCE),
          mMutualExclusionSemaphore(WbSemaphoreCreate(1, 1)),
          mpResultHandler(NULL),
          mResultSemaphore(WbSemaphoreCreate(0, 1)),
          mNotificationCount(0)
    {
    }

    /** Destructor */
    virtual ~TestClientWithDefaults()
    { 
        // Wait until previous commands have been processed
        if (!WbSemaphoreTryWait(mMutualExclusionSemaphore, 5000))
        {
            // Previous command did not complete
            EXPECT_TRUE(false);
        }

        WbSemaphoreDelete(mMutualExclusionSemaphore);
        WbSemaphoreDelete(mResultSemaphore);
    }

    /**
    *	Function to request provider resource.
    *	@param resourcePath Local resource path.
    *	@return True if operation was successful.
    */
    TestResult<NoType> callGetResource(const char* resourcePath)
    {
        TestResult<NoType> result = callGetResourceInternal(resourcePath);
        mTestResourceId = (result.mReturnResultCode == HTTP_CODE_OK) ? result.mResourceId : ID_INVALID_RESOURCE;
        return result;
    }

    /**
    *	Function to request provider resource.
    *	@param resourcePath Local resource path.
    *	@return True if operation was successful.
    */
    TestResult<NoType> callAsyncGetResourceAndWaitResult(const char* resourcePath)
    {
        TestResult<NoType> result = callAsyncOpAndWaitResult(
            &TestClientWithDefaults::asyncGetResourceDpcHandler, ID_INVALID_RESOURCE, resourcePath, NoType::NoValue, NoType::NoValue);

        mTestResourceId = RETURN_OK(result.mCallResultCode) && (result.mReturnResultCode == HTTP_CODE_OK) ? result.mResourceId : ID_INVALID_RESOURCE;
        return result;
    }

    /**
    *	Function to release provider resource.
    *	@param resourceId ID of the reource to release.
    *	@return True if operation was successful.
    */
    TestResult<NoType> callReleaseResource(ResourceId resourceId)
    {
        TestResult<NoType> result;

        // Wait until previous commands have been processed
        if (!WbSemaphoreTryWait(mMutualExclusionSemaphore, 5000))
        {
            // Previous command did not complete
            EXPECT_TRUE(false);
            return result;
        }

        OpWrapper<NoType, NoType, NoType, NoType> wrapper(
            result, resourceId, NoType::NoValue, NoType::NoValue, NoType::NoValue);

        result.mValidResponse = true;
        result.mCallResultCode = HTTP_CODE_OK;
        result.mReturnResultCode = DpcFunctor::syncQueueOnce<Result, TestClientWithDefaults, OpWrapper<NoType, NoType, NoType, NoType>&>(
            getExecutionContextId(), this, &TestClientWithDefaults::releaseResourceDpcHandler, wrapper);

        WbSemaphoreRelease(mMutualExclusionSemaphore);
        return result;
    }

    /**
    *	Function to release provider resource.
    *	@param resourceId ID of the reource to release.
    *	@return True if operation was successful.
    */
    TestResult<NoType> callAsyncReleaseResourceAndWaitResult(ResourceId resourceId)
    {
        resourceId = ResourceIdOrPath(resourceId).getId(*this);
        return callAsyncOpAndWaitResult(
            &TestClientWithDefaults::asyncReleaseResourceDpcHandler, resourceId, NoType::NoValue, NoType::NoValue, NoType::NoValue);
    }

    /** Helper class for resolving resource IDs in command parameters.
     *
     * We cannot resolve at compile time if the first parameter to callAsyncXXXAndWait functions
     * is ResourceID::Value or uint32, because they are interchangeable. We define resource IDs
     * as ResourceID::Value=uint32, because we want to compare them in switch case labels. We cannot
     * compare ResourceIDs because although they are consts it is not guaranteed that the value won't
     * be changed using const_cast. C++11 constexpr would rescue us and we could have cleaner
     * nonambiquos interface.
     */
    class ResourceIdOrPath
    {
    public:
        ResourceIdOrPath(const ResourceId resourceId)
            : mResourceId(resourceId), mResourcePath(NULL)
        {
        }

        ResourceIdOrPath(const ResourceId::Value resourceIdValue)
            : mResourceId(resourceIdValue), mResourcePath(NULL)
        {
        }

        ResourceIdOrPath(const char* resourcePath)
            : mResourceId(ID_INVALID_RESOURCE), mResourcePath(resourcePath)
        {
        }

        ResourceId getId(TestClientWithDefaults& rClient) const
        {
            if (mResourceId == CURRENT_RESOURCE)
            {
                return rClient.mTestResourceId;
            }
            else if (mResourceId != ID_INVALID_RESOURCE)
            {
                return mResourceId;
            }
            else
            {
                TestResult<NoType> result = rClient.callGetResourceInternal(mResourcePath);
                return result ? result.mResourceId : ID_INVALID_RESOURCE;
            }
        }

        ResourceId mResourceId;
        const char* mResourcePath;
    };

    /** asyncGet */
    template <typename R = DefaultGetReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncGetAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            TestResult<R> result;
            result.mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOpAndWaitResult<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncGetDpcHandler, resourceId, param1, param2, param3);
    }

    /** asyncPut */
    template <typename R = DefaultPutReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncPutAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            TestResult<R> result;
            result.mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOpAndWaitResult<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncPutDpcHandler, resourceId, param1, param2, param3);
    }

    /** asyncPost */
    template <typename R = DefaultPostReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncPostAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            TestResult<R> result;
            result.mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOpAndWaitResult<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncPostDpcHandler, resourceId, param1, param2, param3);
    }

    /** asyncDelete */
    template <typename R = DefaultDeleteReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncDeleteAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            TestResult<R> result;
            result.mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOpAndWaitResult<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncDeleteDpcHandler, resourceId, param1, param2, param3);
    }

    /** asyncSubscribe */
    template <typename R = DefaultSubscribeReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncSubscribeAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            TestResult<R> result;
            result.mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOpAndWaitResult<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncSubscribeDpcHandler, resourceId, param1, param2, param3);
    }

    /** asyncUnsubscribe */
    template <typename R = DefaultUnsubscribeReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncUnsubscribeAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            TestResult<R> result;
            result.mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOpAndWaitResult<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncUnsubscribeDpcHandler, resourceId, param1, param2, param3);
    }

    /**
    *	Function to reset notification count.
    */
    void resetNoficationCount() { mNotificationCount = 0; }

    /**
    *	Function to get notifiation count
    *	@return Current notification count.
    */
    size_t getNotificationCount() const { return mNotificationCount; }

private:
    /** Actual getResource implementation */
    TestResult<NoType> callGetResourceInternal(const char* resourcePath)
    {
        TestResult<NoType> result;

        // Wait until previous commands have been processed
        if (!WbSemaphoreTryWait(mMutualExclusionSemaphore, 5000))
        {
            // Previous command did not complete
            EXPECT_TRUE(false);
            return result;
        }

        OpWrapper<NoType, const char*, NoType, NoType> wrapper(
            result, ID_INVALID_RESOURCE, resourcePath, NoType::NoValue, NoType::NoValue);

        result.mValidResponse = true;
        result.mCallResultCode = HTTP_CODE_OK;
        result.mReturnResultCode = DpcFunctor::syncQueueOnce<Result, TestClientWithDefaults, OpWrapper<NoType, const char*, NoType, NoType>&>(
            getExecutionContextId(), this, &TestClientWithDefaults::getResourceDpcHandler, wrapper);

        WbSemaphoreRelease(mMutualExclusionSemaphore);
        return result;
    }

    /** Internal interface for objects that can handle operation results */
    class IResultHandler
    {
    public:
        /** Destructor */
        virtual ~IResultHandler() { }

        /** Handles result of asyncGetResource */
        virtual void handleGetResourceResult(RequestId requestId, Result resultCode, const ResourceId resourceId) = 0;

        /** Handles result of asyncReleaseResource */
        virtual void handleReleaseResourceResult(RequestId requestId, Result resultCode, const ResourceId resourceId) = 0;

        /** Handles result of asyncXxxRequest */
        virtual void handleResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResult) = 0;
    };

    /** Operation related data storage class */
    template <typename R, typename P1, typename P2, typename P3>
    class OpWrapper FINAL : public IResultHandler
    {
    public:
        OpWrapper(
            TestResult<R>& rResult,
            ResourceId resourceId, 
            const P1& rParam1,
            const P2& rParam2,
            const P3& rParam3) :
            mrResult(rResult),
            mResourceId(resourceId),
            mrParam1(rParam1),
            mrParam2(rParam2),
            mrParam3(rParam3)
        {
        }

        /** Destructor */
        virtual ~OpWrapper() {}

        void handleGetResourceResult(RequestId requestId, Result resultCode, const ResourceId resourceId) OVERRIDE
        {
            mrResult.mValidResponse = mRequestId == requestId;
            mrResult.mReturnResultCode = resultCode;
            mrResult.mResourceId = resourceId;
        }

        void handleReleaseResourceResult(RequestId requestId, Result resultCode, const ResourceId resourceId) OVERRIDE
        {
            mrResult.mValidResponse = (mRequestId == requestId) && (mResourceId == resourceId);
            mrResult.mReturnResultCode = resultCode;
            mrResult.mResourceId = resourceId;
        }

        void handleResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResult) OVERRIDE
        {
            mrResult.mValidResponse = (mRequestId == requestId) && (mResourceId == resourceId);
            mrResult.mReturnResultCode = resultCode;
            mrResult.mValueDataTypeId = rResult.getSenderDataTypeId();
            if (mrResult.mValueDataTypeId == Value::DataTypeId<R>::value)
            {
                mrResult.setValue(rResult.convertTo<R>());
            }
        }

        TestResult<R>& mrResult;
        ResourceId mResourceId;
        const P1& mrParam1;
        const P2& mrParam2;
        const P3& mrParam3;
        RequestId mRequestId;
    };

    /** Implementation of asynchronous operations */
    template <typename R, typename P1, typename P2, typename P3>
    TestResult<R> callAsyncOpAndWaitResult(
        Result(TestClientWithDefaults::*pFnMemberFunction)(OpWrapper<R, P1, P2, P3>&),
        ResourceId resourceId,
        const P1& param1,
        const P2& param2,
        const P3& param3)
    {
        TestResult<R> result;

        // Wait until previous commands have been processed
        if (!WbSemaphoreTryWait(mMutualExclusionSemaphore, 5000))
        {
            // Previous command did not complete
            EXPECT_TRUE(false);
            return result;
        }

        OpWrapper<R, P1, P2, P3> wrapper(result, resourceId, param1, param2, param3);
        mpResultHandler = &wrapper;

        result.mCallResultCode = DpcFunctor::syncQueueOnce<Result, TestClientWithDefaults, OpWrapper<R, P1, P2, P3>&>(
            getExecutionContextId(), this, pFnMemberFunction, wrapper);
        if (!RETURN_OK(result.mCallResultCode))
        {
            mpResultHandler = NULL;
            WbSemaphoreRelease(mMutualExclusionSemaphore);
        }
        else if (!WbSemaphoreTryWait(mResultSemaphore, 5000))
        {
            // No response from provider
            EXPECT_TRUE(false);
            mpResultHandler = NULL;

            // Release semaphore that next call can begin, even if
            // we might crash if the provider decides to answer very late
            WbSemaphoreRelease(mMutualExclusionSemaphore);
        }

        return result;
    }

    /**
     DPC callbacks
    */

    Result getResourceDpcHandler(OpWrapper<NoType, const char*, NoType, NoType>& wrapper)
    {
        return getResource(wrapper.mrParam1, wrapper.mrResult.mResourceId);
    }

    Result releaseResourceDpcHandler(OpWrapper<NoType, NoType, NoType, NoType>& wrapper)
    {
        return releaseResource(wrapper.mResourceId);
    }

    Result asyncGetResourceDpcHandler(OpWrapper<NoType, const char*, NoType, NoType>& wrapper)
    {
        return asyncGetResource(
            wrapper.mrParam1, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId));
    }

    Result asyncReleaseResourceDpcHandler(OpWrapper<NoType, NoType, NoType, NoType>& wrapper)
    {
        return asyncReleaseResource(wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId));
    }

    template <typename R, typename P1, typename P2, typename P3>
    Result asyncGetDpcHandler(OpWrapper<R, P1, P2, P3>& wrapper)
    {
        if (IsSame<P1, NoType>::value)
        {
            return asyncGet(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId));
        }
        else if (IsSame<P2, NoType>::value)
        {
            return asyncGet(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncGet(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncGet(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename R, typename P1, typename P2, typename P3>
    Result asyncPutDpcHandler(OpWrapper<R, P1, P2, P3>& wrapper)
    {
        if (IsSame<P1, NoType>::value)
        {
            return asyncPut(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId));
        }
        else if (IsSame<P2, NoType>::value)
        {
            return asyncPut(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncPut(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncPut(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename R, typename P1, typename P2, typename P3>
    Result asyncPostDpcHandler(OpWrapper<R, P1, P2, P3>& wrapper)
    {
        if (IsSame<P1, NoType>::value)
        {
            return asyncPost(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId));
        }
        else if (IsSame<P2, NoType>::value)
        {
            return asyncPost(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncPost(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncPost(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename R, typename P1, typename P2, typename P3>
    Result asyncDeleteDpcHandler(OpWrapper<R, P1, P2, P3>& wrapper)
    {
        if (IsSame<P1, NoType>::value)
        {
            return asyncDelete(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId));
        }
        else if (IsSame<P2, NoType>::value)
        {
            return asyncDelete(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncDelete(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncDelete(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename R, typename P1, typename P2, typename P3>
    Result asyncSubscribeDpcHandler(OpWrapper<R, P1, P2, P3>& wrapper)
    {
        if (IsSame<P1, NoType>::value)
        {
            return asyncSubscribe(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId));
        }
        else if (IsSame<P2, NoType>::value)
        {
            return asyncSubscribe(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncSubscribe(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncSubscribe(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename R, typename P1, typename P2, typename P3>
    Result asyncUnsubscribeDpcHandler(OpWrapper<R, P1, P2, P3>& wrapper)
    {
        if (IsSame<P1, NoType>::value)
        {
            return asyncUnsubscribe(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId));
        }
        else if (IsSame<P2, NoType>::value)
        {
            return asyncUnsubscribe(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncUnsubscribe(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncUnsubscribe(
                wrapper.mResourceId, ResourceClient::AsyncRequestOptions(&wrapper.mRequestId), wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    /** Request response handlers */
    void onResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& result)
    {
        if (mpResultHandler != NULL)
        {
            mpResultHandler->handleResult(requestId, resourceId, resultCode, result);
            mpResultHandler = NULL;
        }

        WbSemaphoreRelease(mResultSemaphore);
        WbSemaphoreRelease(mMutualExclusionSemaphore);
    }

protected:
    /** Whiteboard callback function for asyncGetResource request. */
    void onGetResourceResult(RequestId requestId, ResourceId resourceId, Result resultCode) OVERRIDE
    {
        if (mpResultHandler != NULL)
        {
            mpResultHandler->handleGetResourceResult(requestId, resultCode, resourceId);
            mpResultHandler = NULL;
        }

        WbSemaphoreRelease(mResultSemaphore);
        WbSemaphoreRelease(mMutualExclusionSemaphore);
    }

    void onReleaseResourceResult(RequestId requestId, ResourceId resourceId, Result resultCode) OVERRIDE
    {
        if (mpResultHandler != NULL)
        {
            mpResultHandler->handleReleaseResourceResult(requestId, resultCode, resourceId);
            mpResultHandler = NULL;
        }

        WbSemaphoreRelease(mResultSemaphore);
        WbSemaphoreRelease(mMutualExclusionSemaphore);
    }

    /** Whiteboard callback function for asyncGet request. */
    void onGetResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& result) OVERRIDE
    {
        onResult(requestId, resourceId, resultCode, result);
    }

    /** Whiteboard callback function for asyncPut request. */
    void onPutResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& result) OVERRIDE
    {
        onResult(requestId, resourceId, resultCode, result);
    }

    /** Whiteboard callback function for asyncPost request. */
    void onPostResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& result) OVERRIDE
    {
        onResult(requestId, resourceId, resultCode, result);
    }

    /** Whiteboard callback function for asyncDelete request. */
    void onDeleteResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& result) OVERRIDE
    {
        onResult(requestId, resourceId, resultCode, result);
    }

    /** Whiteboard callback function for asyncSubscribe request. */
    void onSubscribeResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& result) OVERRIDE
    {
        onResult(requestId, resourceId, resultCode, result);
    }

    /** Whiteboard callback function for asyncUnsubscribe request. */
    void onUnsubscribeResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& result) OVERRIDE
    {
        onResult(requestId, resourceId, resultCode, result);
    }

    /** Whiteboard callback function for subscribed events. */
    void onNotify(ResourceId /* resourceId */, const Value& /*result*/, const ParameterList& /* rParameters */) OVERRIDE
    {
        this->mNotificationCount++;
    }

public:
    /** Just make these public for test convenience */
    ResourceId mTestResourceId;
private:
    WbSemaphoreHandle mMutualExclusionSemaphore;
    IResultHandler* mpResultHandler;
    WbSemaphoreHandle mResultSemaphore;
    volatile size_t mNotificationCount;
};

typedef TestClientWithDefaults<> TestClient;

} // namespace whiteboard

#endif
