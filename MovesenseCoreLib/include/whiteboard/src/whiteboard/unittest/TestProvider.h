#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/DpcFunctor.h"
#include "whiteboard/Initialization.h"
#include "whiteboard/ResourceProvider.h"
#include "whiteboard/unittest/ScopedResource.h"

#if WB_UNITTEST_BUILD

namespace whiteboard
{

// Forward declarations
class WhiteboardMockup;

template <typename T> class TestValue
{
private:
    T mValue;

public:
    TestValue(const T& rValue) : mValue(rValue) {}
    TestValue() {}
    ~TestValue() {}

    operator const T&() const { return mValue; }

    T& operator=(const T& rOther)
    {
        mValue = rOther;
        return mValue;
    }
};

template <> class TestValue<char*>
{
private:
    char* mValue;

public:
    TestValue() : mValue(NULL) {}

    ~TestValue() { WbMemFree(mValue); }

    operator char* const&() const { return mValue; }

    char*& operator=(const char* other)
    {
        WbMemFree(mValue);

        size_t length = strlen(other);
        mValue = static_cast<char*>(WbMemAlloc(length + 1));
        memcpy(mValue, other, length + 1);
        return mValue;
    }
};

/** Base class for test providers */
class TestProviderBase : public ResourceProvider
{
public:
    /**
     *	Constructor
     *	@param pName Name of the resource.
     *	@param executionContextId Providers execution context.
     *   @param rWhiteboard Whiteboard instance to use
     */
    TestProviderBase(const char* pName, ExecutionContextId executionContextId, Whiteboard& rWhiteboard);

    /**
     * Constructor
     *
     * @param pName Name of the resource.
     * @param rWhiteboard Whiteboard instance to use
     */
    TestProviderBase(const char* pName, WhiteboardMockup& rWhiteboard);

    /** Desctructor */
    virtual ~TestProviderBase();

    /** registerProviderResource */
    Result callRegisterProviderResource(const LocalResourceId localResourceId)
    {
        return DpcFunctor::syncQueueOnce<Result, ResourceProvider, LocalResourceId>(
            getExecutionContextId(), getWhiteboard(), this, &ResourceProvider::registerProviderResource, localResourceId);
    }

    /** unregisterProviderResource */
    Result callUnregisterProviderResource(const LocalResourceId localResourceId)
    {
        return DpcFunctor::syncQueueOnce<Result, ResourceProvider, LocalResourceId>(
            getExecutionContextId(), getWhiteboard(), this, &ResourceProvider::unregisterProviderResource, localResourceId);
    }

protected:
    /** Gets the associated whiteboard instance
     *
     * @return Whiteboard instance
     */
    virtual Whiteboard& getWhiteboard() OVERRIDE FINAL { return mrWhiteboard; }

    /**
     *	GET request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onGetRequest(const Request& rRequest, const ParameterList& /*rParameters*/) OVERRIDE
    {
        return returnResult(rRequest, HTTP_CODE_OK);
    }

    /**
     *	PUT request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onPutRequest(const Request& rRequest, const ParameterList& /*rParameters*/) OVERRIDE
    {
        return returnResult(rRequest, HTTP_CODE_OK);
    }

    /**
     *	POST request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onPostRequest(const Request& rRequest, const ParameterList& /*rParameters*/) OVERRIDE
    {
        return returnResult(rRequest, HTTP_CODE_OK);
    }

    /**
     *	DELETE request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onDeleteRequest(const Request& rRequest, const ParameterList& /*rParameters*/) OVERRIDE
    {
        return returnResult(rRequest, HTTP_CODE_OK);
    }

private:
    /** Because ResourceProvider constructor binds to whiteboard before virtual table has been fully
     *  initialized, we need to fix the binding to our whiteboard */
    void fixBinding();

    /** ResourceProvider's destructor's unbinding will fail if we have not restored
     *   original binding */
    void unfixBinding();

protected:
    Whiteboard& mrWhiteboard;
    bool mBindingFixed;
};

template <typename T, typename METADATA_CREATOR = MetadataCreator> class TestProvider : public TestProviderBase
{
public:
    /**
     *	Constructor
     *	@param name Name of the resource.
     *	@param executionContextId Providers execution context.
     *	@param registerProvider Flag to enable registration of provider resource. Default true.
     *   @param rWhiteboard Whiteboard instance to use
     */
    TestProvider(const char* name,
                 ExecutionContextId executionContextId,
                 bool registerProvider = true,
                 Whiteboard& rWhiteboard = getWhiteboardInstance())
        : TestProviderBase(name, executionContextId, rWhiteboard),
          mResourceImpl(ID_INVALID_EXECUTION_CONTEXT, ID_INVALID_LOCAL_RESOURCE, rWhiteboard),
          mResource(name, executionContextId, 0, rWhiteboard),
          mpResource(&mResource),
          mRegisterProvider(registerProvider),
          mGetRequestCounter(0),
          mPutRequestCounter(0),
          mPostRequestCounter(0),
          mDeleteRequestCounter(0),
          mSubscribeRequestCounter(0),
          mUnsubscribeRequestCounter(0)
    {
        if (mRegisterProvider)
        {
            registerTestResourceInWbContext();
        }
    }

    /**
     *	Constructor
     *	@param name Name of the resource.
     *	@param localResourceId Local resource ID to register.
     *	@param executionContextId Providers execution context.
     *	@param registerProvider Flag to enable registration of provider resource. Default true.
     *   @param rWhiteboard Whiteboard instance to use
     */
    TestProvider(const char* name,
                 LocalResourceId localResourceId,
                 ExecutionContextId executionContextId,
                 bool registerProvider = true,
                 Whiteboard& rWhiteboard = getWhiteboardInstance())
        : TestProviderBase(name, executionContextId, rWhiteboard),
          mResourceImpl(executionContextId, localResourceId, rWhiteboard),
          mResource("N/A", executionContextId, 0, rWhiteboard),
          mpResource(&mResourceImpl),
          mRegisterProvider(registerProvider),
          mGetRequestCounter(0),
          mPutRequestCounter(0),
          mPostRequestCounter(0),
          mDeleteRequestCounter(0),
          mSubscribeRequestCounter(0),
          mUnsubscribeRequestCounter(0)
    {
        if (mRegisterProvider)
        {
            registerTestResourceInWbContext();
        }
    }

    /**
     *	Constructor
     *	@param name Name of the resource.
     *	@param executionContextId Providers execution context.
     *	@param registerProvider Flag to enable registration of provider resource.
     *   @param rInitialValue Initial value
     *   @param rWhiteboard Whiteboard instance to use
     */
    TestProvider(const char* name,
                 ExecutionContextId executionContextId,
                 bool registerProvider,
                 const T& rInitialValue,
                 Whiteboard& rWhiteboard = getWhiteboardInstance())
        : TestProviderBase(name, executionContextId, rWhiteboard),
          mResourceImpl(ID_INVALID_EXECUTION_CONTEXT, ID_INVALID_LOCAL_RESOURCE, rWhiteboard),
          mResource(name, executionContextId, 0, rWhiteboard),
          mpResource(&mResource),
          mRegisterProvider(registerProvider),
          mGetRequestCounter(0),
          mPutRequestCounter(0),
          mPostRequestCounter(0),
          mDeleteRequestCounter(0),
          mSubscribeRequestCounter(0),
          mUnsubscribeRequestCounter(0)
    {
        setValue(rInitialValue);
        if (mRegisterProvider)
        {
            registerTestResourceInWbContext();
        }
    }

    /**
     *	Constructor
     *	@param name Name of the resource.
     *	@param registerProvider Flag to enable registration of provider resource. Default true.
     *   @param rWhiteboard Whiteboard instance to use
     */
    TestProvider(const char* name, bool registerProvider, WhiteboardMockup& rWhiteboard)
        : TestProviderBase(name, rWhiteboard),
          mResourceImpl(ID_INVALID_EXECUTION_CONTEXT, ID_INVALID_LOCAL_RESOURCE, rWhiteboard),
          mResource(name, 0, rWhiteboard),
          mpResource(&mResource),
          mRegisterProvider(registerProvider),
          mGetRequestCounter(0),
          mPutRequestCounter(0),
          mPostRequestCounter(0),
          mDeleteRequestCounter(0),
          mSubscribeRequestCounter(0),
          mUnsubscribeRequestCounter(0)
    {
        if (mRegisterProvider)
        {
            registerTestResourceInWbContext();
        }
    }

    /**
     *	Constructor
     *	@param name Name of the resource.
     *	@param executionContextId Providers execution context.
     *	@param registerProvider Flag to enable registration of provider resource.
     *   @param rInitialValue Initial value
     *   @param rWhiteboard Whiteboard instance to use
     */
    TestProvider(const char* name, bool registerProvider, const T& rInitialValue, WhiteboardMockup& rWhiteboard)
        : TestProviderBase(name, rWhiteboard),
          mResourceImpl(ID_INVALID_EXECUTION_CONTEXT, ID_INVALID_LOCAL_RESOURCE, rWhiteboard),
          mResource(name, 0, rWhiteboard),
          mpResource(&mResource),
          mRegisterProvider(registerProvider),
          mGetRequestCounter(0),
          mPutRequestCounter(0),
          mPostRequestCounter(0),
          mDeleteRequestCounter(0),
          mSubscribeRequestCounter(0),
          mUnsubscribeRequestCounter(0)
    {
        setValue(rInitialValue);
        if (mRegisterProvider)
        {
            registerTestResourceInWbContext();
        }
    }

    /** Desctructor */
    virtual ~TestProvider()
    {
        if (mRegisterProvider)
        {
            unregisterTestResourceInWbContext();
        }
    }

    /**
     * Performs resource registration inside correct execution context
     */
    void registerTestResourceInWbContext()
    {
        if (!ExecutionContext::isCurrentThread(getExecutionContextId(), mrWhiteboard))
        {
            DpcFunctor::syncQueueOnce(
                getExecutionContextId(), mrWhiteboard, this, &TestProvider::registerTestResourceInWbContext);
        }
        else
        {
            registerProviderResource(mpResource->getId().localResourceId);
        }
    }

    /**
     * Performs resource unregistration inside correct execution context
     */
    void unregisterTestResourceInWbContext()
    {
        if (!ExecutionContext::isCurrentThread(getExecutionContextId(), mrWhiteboard))
        {
            DpcFunctor::syncQueueOnce(
                getExecutionContextId(), mrWhiteboard, this, &TestProvider::unregisterTestResourceInWbContext);
        }
        else
        {
            unregisterProviderResource(mpResource->getId().localResourceId);
        }
    }

    /**
     *	Function to set member value mValue used for get, put and notify.
     *	@param rValue Value to be set.
     */
    void setValue(const T& rValue) { mValue = rValue; }

    /**
     *	Function to read member variable mValue.
     *	@return mValue Value used in provider operations.
     */
    const T getValue() const { return mValue; }

    /**
     *	Function to get resource ID of the provider.
     *	@return Resource ID of the provider.
     */
    const ResourceId getResourceId() const { return mpResource->getId(); }

    /**
     *	Function to notify subscribers. Will pass value of member variable mValue.
     */
    void notifySubscribers()
    {
        NotifyStatus status(*this, mpResource->getId(), mValue);
        DpcFunctor::queueOnce(getExecutionContextId(), mrWhiteboard, &TestProvider::notifyDpcHandler, &status);
        WbSemaphoreTryWait(status.mComplete, 500);
    }

    /**
     *  Test helper for waiting GET requests.
     *
     *  @param requests How many requests TestProvider should have had received before returning
     *  @param timeout How many milliseconds to wait for requests (internally 10 ms resolution)
     *  @return Result of the operation, true if all requests were received withing the timeout
     */
    virtual bool waitUntilReceivedGetRequests(const int32 requests, const int32 timeout = 1000)
    {
        return waitUntilReceivedRequests(mGetRequestCounter, requests, timeout);
    }

    /**
     *	GET request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onGetRequest(const Request& rRequest, const ParameterList& /* rParameters */) OVERRIDE
    {
        if (rRequest.getResourceId() == mpResource->getId())
        {
            returnResult(rRequest, HTTP_CODE_OK, ResourceProvider::ResponseOptions::Empty, static_cast<const T&>(mValue));
        }
        else
        {
            returnResult(rRequest, HTTP_CODE_OK);
        }

        ++mGetRequestCounter;
    }

    /**
     *  Test helper for waiting PUT requests.
     *
     *  @param requests How many requests TestProvider should have had received before returning
     *  @param timeout How many milliseconds to wait for requests (internally 10 ms resolution)
     *  @return Result of the operation, true if all requests were received withing the timeout
     */
    virtual bool waitUntilReceivedPutRequests(const int32 requests, const int32 timeout = 1000)
    {
        return waitUntilReceivedRequests(mPutRequestCounter, requests, timeout);
    }

    /**
     *	PUT request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onPutRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE
    {
        if (rRequest.getResourceId() == mpResource->getId())
        {
            mValue = rParameters[0].convertTo<T>();
        }

        returnResult(rRequest, HTTP_CODE_OK);
        ++mPutRequestCounter;
    }

    /**
     *  Test helper for waiting POST requests.
     *
     *  @param requests How many requests TestProvider should have had received before returning
     *  @param timeout How many milliseconds to wait for requests (internally 10 ms resolution)
     *  @return Result of the operation, true if all requests were received withing the timeout
     */
    virtual bool waitUntilReceivedPostRequests(const int32 requests, const int32 timeout = 1000)
    {
        return waitUntilReceivedRequests(mPostRequestCounter, requests, timeout);
    }

    /**
     *	POST request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onPostRequest(const Request& rRequest, const ParameterList& /* rParameters */) OVERRIDE
    {
        returnResult(rRequest, HTTP_CODE_OK);
        ++mPostRequestCounter;
    }

    /**
     *  Test helper for waiting DELETE requests.
     *
     *  @param requests How many requests TestProvider should have had received before returning
     *  @param timeout How many milliseconds to wait for requests (internally 10 ms resolution)
     *  @return Result of the operation, true if all requests were received withing the timeout
     */
    virtual bool waitUntilReceivedDeleteRequests(const int32 requests, const int32 timeout = 1000)
    {
        return waitUntilReceivedRequests(mDeleteRequestCounter, requests, timeout);
    }

    /**
     *	DELETE request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onDeleteRequest(const Request& rRequest, const ParameterList& /*rParameters*/) OVERRIDE
    {
        returnResult(rRequest, HTTP_CODE_OK);
        ++mDeleteRequestCounter;
    }

    /**
     *  Test helper for waiting SUBSCRIBE requests.
     *
     *  @param requests How many requests TestProvider should have had received before returning
     *  @param timeout How many milliseconds to wait for requests (internally 10 ms resolution)
     *  @return Result of the operation, true if all requests were received withing the timeout
     */
    virtual bool waitUntilReceivedSubscribeRequests(const int32 requests, const int32 timeout = 1000)
    {
        return waitUntilReceivedRequests(mSubscribeRequestCounter, requests, timeout);
    }

    /**
     *	SUBSCRIBE request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onSubscribe(const Request& rRequest, const ParameterList& /* rParameters */) OVERRIDE
    {
        returnResult(rRequest, HTTP_CODE_OK);
        ++mSubscribeRequestCounter;
    }

    /**
     *  Test helper for waiting UNSUBSCRIBE requests.
     *
     *  @param requests How many requests TestProvider should have had received before returning
     *  @param timeout How many milliseconds to wait for requests (internally 10 ms resolution)
     *  @return Result of the operation, true if all requests were received withing the timeout
     */
    virtual bool waitUntilReceivedUnsubscribeRequests(const int32 requests, const int32 timeout = 1000)
    {
        return waitUntilReceivedRequests(mUnsubscribeRequestCounter, requests, timeout);
    }

    /**
     *	UNSUBSCRIBE request handler.
     *
     *	@param rRequest Request information
     *	@param rParameters List of parameters for the request
     *	@return Result of the operation
     */
    virtual void onUnsubscribe(const Request& rRequest, const ParameterList& /*rParameters*/) OVERRIDE
    {
        returnResult(rRequest, HTTP_CODE_OK);
        ++mUnsubscribeRequestCounter;
    }

private:
    /** Helper function for waiting requests to be received */
    bool waitUntilReceivedRequests(volatile int32& counter, const int32 requests, const int32 timeoutMs)
    {
        WB_ASSERT(requests);
        WB_ASSERT(!ExecutionContext::isCurrentThread(getExecutionContextId(), mrWhiteboard));

        const int32 timerResolution = 10;
        const int32 maxSleeps = timeoutMs < timerResolution ? 1 : timeoutMs / timerResolution;
        int32 sleepCounter = 0;

        while (1)
        {
            if (counter < requests)
            {
                if (sleepCounter++ < maxSleeps)
                {
                    // Wait for the requests to come
                    WbThreadSleepMs(10);
                }
                else
                {
                    // Timeout, not all requests have been received
                    return false;
                }
            }
            else
            {
                // Counter limit achieved, all requests have been received
                return true;
            }
        }
    }

protected:
    /** Resource associated with the provider if using local resource */
    ScopedResourceInstantiator mResourceImpl;

    /** Resource associated with the provider if using dynamic resource */
    ScopedResource<T, METADATA_CREATOR> mResource;

    /** Pointer to resource associated with the provider, either local or dynamic */
    ScopedResourceImpl* mpResource;

    /** A value indicating whether provider has registered the resource automatically */
    bool mRegisterProvider;

    /** Current value of the resource */
    TestValue<typename RemoveAll<T>::type> mValue;

    /** Counter for incoming GET requests */
    volatile int32 mGetRequestCounter;

    /** Counter for incoming PUT requests */
    volatile int32 mPutRequestCounter;

    /** Counter for incoming POST requests */
    volatile int32 mPostRequestCounter;

    /** Counter for incoming DELETE requests */
    volatile int32 mDeleteRequestCounter;

    /** Counter for incoming SUBSCRIBE requests */
    volatile int32 mSubscribeRequestCounter;

    /** Counter for incoming UNSUBSCRIBE requests */
    volatile int32 mUnsubscribeRequestCounter;

    class NotifyStatus
    {
    public:
        NotifyStatus(ResourceProvider& provider, ResourceId resourceId, const T& rValue)
            : mProvider(provider), mResourceId(resourceId), mrValue(rValue), mComplete(WbSemaphoreCreate(0, 1))
        {
        }
        ~NotifyStatus() { WbSemaphoreDelete(mComplete); }

        ResourceProvider& mProvider;
        ResourceId mResourceId;
        const T& mrValue;
        WbSemaphoreHandle mComplete;
    };

    static void notifyDpcHandler(NotifyStatus* status)
    {
        status->mProvider.updateResource(
            status->mResourceId.localResourceId, ResourceProvider::ResponseOptions::Empty, status->mrValue);
        WbSemaphoreRelease(status->mComplete);
    }
};

} // namespace whiteboard

#endif
