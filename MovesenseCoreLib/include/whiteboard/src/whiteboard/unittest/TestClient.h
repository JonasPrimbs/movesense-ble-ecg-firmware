#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <gtest/gtest.h>
#include "whiteboard/DpcFunctor.h"
#include "whiteboard/Initialization.h"
#include "whiteboard/unittest/TestClientBase.h"
#include "whiteboard/unittest/TestResult.h"

#ifdef WB_HAVE_BATCHING_PARAMETERS
#include "whiteboard/BatchingParameters.h"
#endif

#if WB_UNITTEST_BUILD

namespace whiteboard
{

/**
*	Test client class.
*/
template <
    typename DefaultGetReturnType = NoType, typename DefaultPutReturnType = NoType,
    typename DefaultPostReturnType = NoType, typename DefaultDeleteReturnType = NoType,
    typename DefaultSubscribeReturnType = NoType, typename DefaultUnsubscribeReturnType = NoType>
class TestClientWithDefaults : public TestClientBase
{
public:
    /**
    *	Constructor
    *	@param ExecutionContextId Client's execution context ID.
    */
    TestClientWithDefaults(ExecutionContextId executionContextId, Whiteboard& rWhiteboard = getWhiteboardInstance())
        : TestClientBase(WBDEBUG_NAME(__FUNCTION__), executionContextId, rWhiteboard),
          mTestResourceId(ID_INVALID_RESOURCE),
          mUpdateCurrentTestResource(false),
          mMutualExclusionSemaphore(WbSemaphoreCreate(1, 1)),
          mpResult(NULL),
          mResultSemaphore(WbSemaphoreCreate(0, 1)),
          mNotificationCount(0),
          mTimeoutMs(0)
    {
    }

    /**
    *	Constructor
    *	@param ExecutionContextId Client's execution context ID.
    */
    TestClientWithDefaults(WhiteboardMockup& rWhiteboard)
        : TestClientBase(WBDEBUG_NAME(__FUNCTION__), rWhiteboard),
        mTestResourceId(ID_INVALID_RESOURCE),
        mUpdateCurrentTestResource(false),
        mMutualExclusionSemaphore(WbSemaphoreCreate(1, 1)),
        mpResult(NULL),
        mResultSemaphore(WbSemaphoreCreate(0, 1)),
        mNotificationCount(0),
        mTimeoutMs(0)
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
            : mResourceId(resourceId), mResourcePath(NULL), mpClient(NULL)
        {
        }

        ResourceIdOrPath(const ResourceId::Value resourceIdValue)
            : mResourceId(resourceIdValue), mResourcePath(NULL), mpClient(NULL)
        {
        }

        ResourceIdOrPath(const char* resourcePath)
            : mResourceId(ID_INVALID_RESOURCE), mResourcePath(resourcePath), mpClient(NULL)
        {
        }

        ~ResourceIdOrPath()
        {
            // Release resource on exit
            if (mResourcePath && mpClient)
            {
                mpClient->callAsyncReleaseResourceAndWaitResult(mResourceId);
            }
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
                // Don't mix clients with same ResourceIdOrPath instance
                WB_ASSERT((mpClient == NULL) || (mpClient == &rClient));
                mpClient = &rClient;

                TestResult<NoType> result = rClient.callGetResourceInternal(mResourcePath);
                return result ? result.mResourceId : ResourceId(ID_INVALID_RESOURCE);
            }
        }

        ResourceId mResourceId;
        const char* mResourcePath;
        mutable TestClientWithDefaults* mpClient;
    };

    /*******************************
    * Synchronous functions
    ********************************/

    /**
    *	Function to request provider resource.
    *	@param resourcePath Local resource path.
    *	@return True if operation was successful.
    */
    TestResult<NoType> callGetResource(const char* resourcePath)
    {
        TestResult<NoType> result = callGetResourceInternal(resourcePath);
        mTestResourceId = (result.mReturnResultCode == HTTP_CODE_OK) ? result.mResourceId : ResourceId(ID_INVALID_RESOURCE);
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

        OpWrapper<NoType, NoType, NoType> wrapper(NoType::NoValue, NoType::NoValue, NoType::NoValue);
        mpResult = &result;
        mResourceId = resourceId;

        result.mValidResponse = true;
        result.mCallResultCode = HTTP_CODE_OK;
        result.mReturnResultCode = DpcFunctor::syncQueueOnce<Result, TestClientWithDefaults, OpWrapper<NoType, NoType, NoType>&>(
            getExecutionContextId(), getWhiteboard(), this, &TestClientWithDefaults::releaseResourceDpcHandler, wrapper);

        WbSemaphoreRelease(mMutualExclusionSemaphore);
        return result;
    }

    /*********************************
    * Asynchronous functions
    **********************************/

    /**
    *	Function to request provider resource.
    *	@param resourcePath Local resource path.
    *	@return True if operation was successful.
    */
    AsyncTestResult<NoType> callAsyncGetResource(const char* resourcePath)
    {
        return callAsyncOp<NoType, const char*, NoType, NoType>(
            &TestClientWithDefaults::asyncGetResourceDpcHandler, ID_INVALID_RESOURCE, resourcePath, NoType::NoValue, NoType::NoValue);
    }

    /**
    *	Function to release provider resource.
    *	@param resourceId ID of the reource to release.
    *	@return True if operation was successful.
    */
    AsyncTestResult<NoType> callAsyncReleaseResource(ResourceId resourceId)
    {
        // This is to update current resource for the test client
        resourceId = ResourceIdOrPath(resourceId).getId(*this);

        // Start async op
        return callAsyncOp<NoType, NoType, NoType, NoType>(
            &TestClientWithDefaults::asyncReleaseResourceDpcHandler, resourceId, NoType::NoValue, NoType::NoValue, NoType::NoValue);
    }

    /** asyncGet */
    template <typename R = DefaultGetReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    AsyncTestResult<R> callAsyncGet(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool noResponse = false,
        bool forceReceiverDataType = false)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            AsyncTestResult<R> result;
            static_cast<TestResult<R>&>(result).mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOp<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncGetDpcHandler, resourceId, param1, param2, param3, forceAsync, true, noResponse, forceReceiverDataType);
    }

    /** asyncPut */
    template <typename R = DefaultPutReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    AsyncTestResult<R> callAsyncPut(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool noResponse = false,
        bool forceReceiverDataType = false)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            AsyncTestResult<R> result;
            static_cast<TestResult<R>&>(result).mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOp<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncPutDpcHandler, resourceId, param1, param2, param3, forceAsync, true, noResponse, forceReceiverDataType);
    }

    /** asyncPost */
    template <typename R = DefaultPostReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    AsyncTestResult<R> callAsyncPost(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool noResponse = false,
        bool forceReceiverDataType = false)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            AsyncTestResult<R> result;
            static_cast<TestResult<R>&>(result).mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOp<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncPostDpcHandler, resourceId, param1, param2, param3, forceAsync, true, noResponse, forceReceiverDataType);
    }

    /** asyncDelete */
    template <typename R = DefaultDeleteReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    AsyncTestResult<R> callAsyncDelete(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool noResponse = false,
        bool forceReceiverDataType = false)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            AsyncTestResult<R> result;
            static_cast<TestResult<R>&>(result).mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOp<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncDeleteDpcHandler, resourceId, param1, param2, param3, forceAsync, true, noResponse, forceReceiverDataType);
    }

    /** asyncSubscribe */
    template <typename R = DefaultSubscribeReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    AsyncTestResult<R> callAsyncSubscribe(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool isCritical = true,
        bool noResponse = false,
        bool forceReceiverDataType = false)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            AsyncTestResult<R> result;
            static_cast<TestResult<R>&>(result).mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOp<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncSubscribeDpcHandler, resourceId, param1, param2, param3, forceAsync, isCritical, noResponse, forceReceiverDataType);
    }

    /** asyncUnsubscribe */
    template <typename R = DefaultUnsubscribeReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    AsyncTestResult<R> callAsyncUnsubscribe(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool noResponse = false,
        bool forceReceiverDataType = false)
    {
        ResourceId resourceId = rResourceIdOrPath.getId(*this);
        if (resourceId == ID_INVALID_RESOURCE)
        {
            // Failed to resolve resource
            AsyncTestResult<R> result;
            static_cast<TestResult<R>&>(result).mCallResultCode = HTTP_CODE_NOT_FOUND;
            return result;
        }

        return callAsyncOp<R, P1, P2, P3>(
            &TestClientWithDefaults::asyncUnsubscribeDpcHandler, resourceId, param1, param2, param3, forceAsync, noResponse, forceReceiverDataType);
    }

    /** Waits for asynchronous operation to complete */
    bool wait()
    {
        const size_t timeoutMs = WB_MAX(
            500,
            (mTimeoutMs >= WB_INFINITE - 2000) ? WB_INFINITE : (mTimeoutMs + 2000));
        if (!WbSemaphoreTryWait(mResultSemaphore, timeoutMs))
        {
            // No response from provider. Local requests should never timeout
            // and WBComm should generate timeout error for remote requests.
            EXPECT_TRUE(false);

            mpResult = NULL;

            // Release semaphore that next call can begin, even if
            // we might crash if the provider decides to answer very late
            WbSemaphoreRelease(mMutualExclusionSemaphore);
            return false;
        }

        return true;
    }

    /*********************************
    * Asynchronous functions with wait
    **********************************/

    /**
    *	Function to request provider resource.
    *	@param resourcePath Local resource path.
    *	@return True if operation was successful.
    */
    TestResult<NoType> callAsyncGetResourceAndWaitResult(const char* resourcePath)
    {
        TestResult<NoType> result = callAsyncOpAndWaitResult<NoType, const char*, NoType, NoType>(
            &TestClientWithDefaults::asyncGetResourceDpcHandler, ID_INVALID_RESOURCE, resourcePath, NoType::NoValue, NoType::NoValue);

        mTestResourceId = RETURN_OK(result.mCallResultCode) && (result.mReturnResultCode == HTTP_CODE_OK) ? result.mResourceId : ResourceId(ID_INVALID_RESOURCE);
        return result;
    }

    /**
    *	Function to release provider resource.
    *	@param resourceId ID of the reource to release.
    *	@return True if operation was successful.
    */
    TestResult<NoType> callAsyncReleaseResourceAndWaitResult(ResourceId resourceId)
    {
        // This is to update current resource for the test client
        resourceId = ResourceIdOrPath(resourceId).getId(*this);

        // Start async op
        return callAsyncOpAndWaitResult<NoType, NoType, NoType, NoType>(
            &TestClientWithDefaults::asyncReleaseResourceDpcHandler, resourceId, NoType::NoValue, NoType::NoValue, NoType::NoValue);
    }

    /** asyncGet */
    template <typename R = DefaultGetReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncGetAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool forceReceiverDataType = false)
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
            &TestClientWithDefaults::asyncGetDpcHandler, resourceId, param1, param2, param3, forceAsync, false, forceReceiverDataType);
    }

    /** asyncPut */
    template <typename R = DefaultPutReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncPutAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool forceReceiverDataType = false)
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
            &TestClientWithDefaults::asyncPutDpcHandler, resourceId, param1, param2, param3, forceAsync, false, forceReceiverDataType);
    }

    /** asyncPost */
    template <typename R = DefaultPostReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncPostAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool forceReceiverDataType = false)
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
            &TestClientWithDefaults::asyncPostDpcHandler, resourceId, param1, param2, param3, forceAsync, false, forceReceiverDataType);
    }

    /** asyncDelete */
    template <typename R = DefaultDeleteReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncDeleteAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool forceReceiverDataType = false)
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
            &TestClientWithDefaults::asyncDeleteDpcHandler, resourceId, param1, param2, param3, forceAsync, false, forceReceiverDataType);
    }

    /** asyncSubscribe */
    template <typename R = DefaultSubscribeReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncSubscribeAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool isCritical = true,
        bool forceReceiverDataType = false
#ifdef WB_HAVE_BATCHING_PARAMETERS
        , BatchingParameters batchingParameters = BatchingParameters()
#endif
    )
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
            &TestClientWithDefaults::asyncSubscribeDpcHandler, resourceId, param1, param2, param3, forceAsync, isCritical, forceReceiverDataType
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , batchingParameters
#endif
            );
    }

    /** asyncUnsubscribe */
    template <typename R = DefaultUnsubscribeReturnType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType>
    TestResult<R> callAsyncUnsubscribeAndWaitResult(
        const ResourceIdOrPath& rResourceIdOrPath,
        const P1& param1 = NoType::NoValue,
        const P2& param2 = NoType::NoValue,
        const P3& param3 = NoType::NoValue,
        bool forceAsync = false,
        bool forceReceiverDataType = false)
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
            &TestClientWithDefaults::asyncUnsubscribeDpcHandler, resourceId, param1, param2, param3, forceAsync, false, forceReceiverDataType);
    }

    /*********************************
    * Misc functions
    **********************************/

    /**
    *	Function to reset notification count.
    */
    void resetNoficationCount() { mNotificationCount = 0; }

    /**
    *	Function to get notification count
    *	@return Current notification count.
    */
    size_t getNotificationCount() const { return mNotificationCount; }

    /**
    *	Function to set request timeout for remote requests
    *	@param timeoutMs New timeout in milliseconds
    */
    void setTimeout(size_t timeoutMs) { mTimeoutMs = timeoutMs; }

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

        OpWrapper<const char*, NoType, NoType> wrapper(resourcePath, NoType::NoValue, NoType::NoValue);
        mpResult = &result;
        mResourceId = ID_INVALID_RESOURCE;

        result.mValidResponse = true;
        result.mCallResultCode = HTTP_CODE_OK;
        result.mReturnResultCode = DpcFunctor::syncQueueOnce<Result, TestClientWithDefaults, OpWrapper<const char*, NoType, NoType>&>(
            getExecutionContextId(), getWhiteboard(), this, &TestClientWithDefaults::getResourceDpcHandler, wrapper);

        WbSemaphoreRelease(mMutualExclusionSemaphore);
        return result;
    }

    /** Operation related data storage class */
    template <typename P1, typename P2, typename P3>
    class OpWrapper FINAL
    {
    public:
        OpWrapper(
            const P1& rParam1,
            const P2& rParam2,
            const P3& rParam3,
            bool forceAsync = false,
            bool isCritical = true,
            bool noResponse = false,
            bool forceReceiverDataType = false
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , BatchingParameters batchingParameters = BatchingParameters()
#endif
        ) :
            mrParam1(rParam1),
            mrParam2(rParam2),
            mrParam3(rParam3),
            mForceAsync(forceAsync),
            mIsCritical(isCritical),
            mNoResponse(noResponse),
            mForceReceiverDataType(forceReceiverDataType)
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , mBatchingParameters(batchingParameters)
#endif
        {
        }

        /** Destructor */
        ~OpWrapper() {}

        const P1& mrParam1;
        const P2& mrParam2;
        const P3& mrParam3;
        bool mForceAsync;
        bool mIsCritical;
        bool mNoResponse;
        bool mForceReceiverDataType;
#ifdef WB_HAVE_BATCHING_PARAMETERS
        BatchingParameters mBatchingParameters;
#endif
    };

    /** Implementation of asynchronous operations */
    template <typename R, typename P1, typename P2, typename P3>
    AsyncTestResult<R> callAsyncOp(
        Result(TestClientWithDefaults::*pFnMemberFunction)(OpWrapper<P1, P2, P3>&),
        ResourceId resourceId,
        const P1& param1,
        const P2& param2,
        const P3& param3,
        bool forceAsync = false,
        bool isCritical = true,
        bool noResponse = false,
        bool forceReceiverDataType = false
#ifdef WB_HAVE_BATCHING_PARAMETERS
        , BatchingParameters batchingParameters = BatchingParameters()
#endif
    )
    {
        AsyncTestResult<R> result;

        // Wait until previous commands have been processed
        if (!WbSemaphoreTryWait(mMutualExclusionSemaphore, 5000))
        {
            // Previous command did not complete
            EXPECT_TRUE(false);
            return result;
        }

        TestResult<R>& rSyncResult = static_cast<TestResult<R>&>(result);
        OpWrapper<P1, P2, P3> wrapper(param1, param2, param3, forceAsync, isCritical, noResponse, forceReceiverDataType
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , batchingParameters
#endif
        );
        mpResult = &rSyncResult;
        mResourceId = resourceId;
        mUpdateCurrentTestResource = resourceId == ID_INVALID_RESOURCE;

        rSyncResult.mCallResultCode = DpcFunctor::syncQueueOnce<Result, TestClientWithDefaults, OpWrapper<P1, P2, P3>&>(
            getExecutionContextId(), getWhiteboard(), this, pFnMemberFunction, wrapper);
        if (!RETURN_OK(rSyncResult.mCallResultCode) || noResponse)
        {
            mpResult = NULL;
            WbSemaphoreRelease(mMutualExclusionSemaphore);
        }

        return result;
    }

    /** Implementation of asynchronous operations with wait */
    template <typename R, typename P1, typename P2, typename P3>
    TestResult<R> callAsyncOpAndWaitResult(
        Result(TestClientWithDefaults::*pFnMemberFunction)(OpWrapper<P1, P2, P3>&),
        ResourceId resourceId,
        const P1& param1,
        const P2& param2,
        const P3& param3,
        bool forceAsync = true,
        bool isCritical = false,
        bool forceReceiverDataType = false
#ifdef WB_HAVE_BATCHING_PARAMETERS
        , BatchingParameters batchingParameters = BatchingParameters()
#endif
    )
    {
        TestResult<R> result;

        // Wait until previous commands have been processed
        if (!WbSemaphoreTryWait(mMutualExclusionSemaphore, 5000))
        {
            // Previous command did not complete
            EXPECT_TRUE(false);
            return result;
        }

        OpWrapper<P1, P2, P3> wrapper(param1, param2, param3, forceAsync, isCritical, false, forceReceiverDataType
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , batchingParameters
#endif
        );
        mpResult = &result;
        mResourceId = resourceId;
        mUpdateCurrentTestResource = resourceId == ID_INVALID_RESOURCE;

        result.mCallResultCode = DpcFunctor::syncQueueOnce<Result, TestClientWithDefaults, OpWrapper<P1, P2, P3>&>(
            getExecutionContextId(), getWhiteboard(), this, pFnMemberFunction, wrapper);
        if (!RETURN_OK(result.mCallResultCode))
        {
            mpResult = NULL;
            WbSemaphoreRelease(mMutualExclusionSemaphore);
        }
        else
        {
            wait();
        }

        return result;
    }

    /**
     DPC callbacks
    */

    Result getResourceDpcHandler(OpWrapper<const char*, NoType, NoType>& wrapper)
    {
        return getResource(wrapper.mrParam1, mpResult->mResourceId);
    }

    Result releaseResourceDpcHandler(OpWrapper<NoType, NoType, NoType>& /*wrapper*/)
    {
        return releaseResource(mResourceId);
    }

    Result asyncGetResourceDpcHandler(OpWrapper<const char*, NoType, NoType>& wrapper)
    {
        return asyncGetResource(
            wrapper.mrParam1, ResourceClient::AsyncRequestOptions(&mRequestId, mTimeoutMs));
    }

    Result asyncReleaseResourceDpcHandler(OpWrapper<NoType, NoType, NoType>& /*wrapper*/)
    {
        return asyncReleaseResource(
            mResourceId, ResourceClient::AsyncRequestOptions(&mRequestId, mTimeoutMs));
    }

    template <typename P1, typename P2, typename P3>
    Result asyncGetDpcHandler(OpWrapper<P1, P2, P3>& wrapper)
    {
        const ResourceClient::AsyncRequestOptions options(&mRequestId, mTimeoutMs, false, true, wrapper.mNoResponse, wrapper.mForceReceiverDataType
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , wrapper.mBatchingParameters
#endif
        );

        if (IsSame<P1, NoType>::value && IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncGet(mResourceId, options);
        }
        else if (IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncGet(
                mResourceId, options, wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncGet(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncGet(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename P1, typename P2, typename P3>
    Result asyncPutDpcHandler(OpWrapper<P1, P2, P3>& wrapper)
    {
        const ResourceClient::AsyncRequestOptions options(&mRequestId, mTimeoutMs, false, true, wrapper.mNoResponse, wrapper.mForceReceiverDataType
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , wrapper.mBatchingParameters
#endif
        );

        if (IsSame<P1, NoType>::value && IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncPut(
                mResourceId, options);
        }
        else if (IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncPut(
                mResourceId, options, wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncPut(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncPut(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename P1, typename P2, typename P3>
    Result asyncPostDpcHandler(OpWrapper<P1, P2, P3>& wrapper)
    {
        const ResourceClient::AsyncRequestOptions options(&mRequestId, mTimeoutMs, false, true, wrapper.mNoResponse, wrapper.mForceReceiverDataType
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , wrapper.mBatchingParameters
#endif
        );

        if (IsSame<P1, NoType>::value && IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncPost(
                mResourceId, options);
        }
        else if (IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncPost(
                mResourceId, options, wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncPost(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncPost(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename P1, typename P2, typename P3>
    Result asyncDeleteDpcHandler(OpWrapper<P1, P2, P3>& wrapper)
    {
        const ResourceClient::AsyncRequestOptions options(&mRequestId, mTimeoutMs, false, true, wrapper.mNoResponse, wrapper.mForceReceiverDataType
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , wrapper.mBatchingParameters
#endif
        );

        if (IsSame<P1, NoType>::value && IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncDelete(
                mResourceId, options);
        }
        else if (IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncDelete(
                mResourceId, options, wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncDelete(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncDelete(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename P1, typename P2, typename P3>
    Result asyncSubscribeDpcHandler(OpWrapper<P1, P2, P3>& wrapper)
    {
        const ResourceClient::AsyncRequestOptions options(&mRequestId, mTimeoutMs, wrapper.mForceAsync, wrapper.mIsCritical, wrapper.mNoResponse, wrapper.mForceReceiverDataType
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , wrapper.mBatchingParameters
#endif
        );
        if (IsSame<P1, NoType>::value && IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncSubscribe(
                mResourceId, options);
        }
        else if (IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncSubscribe(
                mResourceId, options, wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncSubscribe(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncSubscribe(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    template <typename P1, typename P2, typename P3>
    Result asyncUnsubscribeDpcHandler(OpWrapper<P1, P2, P3>& wrapper)
    {
        const ResourceClient::AsyncRequestOptions options(&mRequestId, mTimeoutMs, false, true, wrapper.mNoResponse, wrapper.mForceReceiverDataType
#ifdef WB_HAVE_BATCHING_PARAMETERS
            , wrapper.mBatchingParameters
#endif
        );

        if (IsSame<P1, NoType>::value && IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncUnsubscribe(
                mResourceId, options);
        }
        else if (IsSame<P2, NoType>::value && IsSame<P3, NoType>::value)
        {
            return asyncUnsubscribe(
                mResourceId, options, wrapper.mrParam1);
        }
        else if (IsSame<P3, NoType>::value)
        {
            return asyncUnsubscribe(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2);
        }
        else
        {
            return asyncUnsubscribe(
                mResourceId, options, wrapper.mrParam1, wrapper.mrParam2, wrapper.mrParam3);
        }
    }

    /** Compare ResourceIds without instance ID as it isn't known at call time */
    static bool isSameResourceId(ResourceId request, ResourceId result)
    {
        request.instanceId = 0;
        result.instanceId = 0;
        return request == result;
    }

    /** Handles result of asyncGetResource */
    void handleGetResourceResult(RequestId requestId, Result resultCode, const ResourceId resourceId)
    {
        mpResult->mValidResponse = mRequestId == requestId;
        mpResult->mReturnResultCode = resultCode;
        mpResult->mResourceId = resourceId;
    }

    /** Handles result of asyncReleaseResource */
    void handleReleaseResourceResult(RequestId requestId, Result resultCode, const ResourceId resourceId)
    {
        mpResult->mValidResponse = (mRequestId == requestId) && isSameResourceId(mResourceId, resourceId);
        mpResult->mReturnResultCode = resultCode;
        mpResult->mResourceId = resourceId;
    }

    /** Handles result of asyncXxxRequest */
    void handleResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResult)
    {
        mpResult->mValidResponse = (mRequestId == requestId) && isSameResourceId(mResourceId, resourceId);
        mpResult->mReturnResultCode = resultCode;
        mpResult->mValueDataTypeId = rResult.getSenderDataTypeId();
        mpResult->setValue(rResult);
    }

    /** Request response handlers */
    void onResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& result)
    {
        if (mpResult != NULL)
        {
            handleResult(requestId, resourceId, resultCode, result);
            mpResult = NULL;
        }

        WbSemaphoreRelease(mResultSemaphore);
        WbSemaphoreRelease(mMutualExclusionSemaphore);
    }

protected:
    /** Whiteboard callback function for asyncGetResource request. */
    void onGetResourceResult(RequestId requestId, ResourceId resourceId, Result resultCode) OVERRIDE
    {
        if (mpResult != NULL)
        {
            handleGetResourceResult(requestId, resultCode, resourceId);
            mpResult = NULL;
        }
        
        if (mUpdateCurrentTestResource)
        {
            mTestResourceId = (resultCode == HTTP_CODE_OK) ? resourceId : ResourceId(ID_INVALID_RESOURCE);
        }

        WbSemaphoreRelease(mResultSemaphore);
        WbSemaphoreRelease(mMutualExclusionSemaphore);
    }

    void onReleaseResourceResult(RequestId requestId, ResourceId resourceId, Result resultCode) OVERRIDE
    {
        if (mpResult != NULL)
        {
            handleReleaseResourceResult(requestId, resultCode, resourceId);
            mpResult = NULL;
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
    /** Current test resource */
    ResourceId mTestResourceId;

private:
    /** A value indicating whether current test resource should be set */
    bool mUpdateCurrentTestResource;

    /** Semaphore that provides mutual exclusion */
    WbSemaphoreHandle mMutualExclusionSemaphore;

    /** Current result */
    TestResultBase* mpResult;

    /** ID of resource associated with current request */
    ResourceId mResourceId;

    /** ID of current request */
    RequestId mRequestId;

    /** Semaphore that is signaled when result is reveived */
    WbSemaphoreHandle mResultSemaphore;
    
    /** Number of notifications received */
    volatile size_t mNotificationCount;

    /** Request timeout */
    size_t mTimeoutMs;
};

typedef TestClientWithDefaults<> TestClient;

} // namespace whiteboard

#endif
