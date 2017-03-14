#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/DpcFunctor.h"
#include "whiteboard/ResourceProvider.h"
#include "whiteboard/unittest/ScopedResource.h"

#if WB_UNITTEST_BUILD

namespace whiteboard
{

template <typename T> class TestValue
{
private:
    T mValue;

public:
    TestValue() {}
    ~TestValue() {}

    operator const T&() const
    {
        return mValue;
    }

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
    TestValue()
        : mValue(NULL)
    {}

    ~TestValue()
    {
        WbMemFree(mValue);
    }

    operator char* const&() const
    {
        return mValue;
    }

    char*& operator=(const char* other)
    {
        WbMemFree(mValue);

        size_t length = strlen(other);
        mValue = static_cast<char*>(WbMemAlloc(length + 1));
        memcpy(mValue, other, length + 1);
        return mValue;
    }
};

class TestProviderBase : public ResourceProvider
{
public:
    /**
    *	Constructor
    *	@param pName Name of the resource.
    *	@param executionContextId Providers execution context.
    *	@param registerProvider Flag to enable registration of provider resource. Default true.
    *   @param rWhiteboard Whiteboard instance to use
    */
    TestProviderBase(
        const char* pName,
        ExecutionContextId executionContextId,
        Whiteboard& rWhiteboard)
        : ResourceProvider(WBDEBUG_NAME(__FUNCTION__), executionContextId),
        mrWhiteboard(rWhiteboard),
        mBindingFixed(false)
    {
        fixBinding();
    }

    /** Desctructor */
    virtual ~TestProviderBase()
    {
        if (mBindingFixed) unfixBinding();
    }

protected:
    /** Gets the associated whiteboard instance
    *
    * @return Whiteboard instance
    */
    virtual Whiteboard& getWhiteboard() OVERRIDE FINAL
    {
        return mrWhiteboard;
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

template <typename T, typename METADATA_CREATOR = MetadataCreator>
class TestProvider : public TestProviderBase
{
public:
    /**
    *	Constructor
    *	@param name Name of the resource.
    *	@param executionContextId Providers execution context.
    *	@param registerProvider Flag to enable registration of provider resource. Default true.
    *   @param rWhiteboard Whiteboard instance to use
    */
    TestProvider(
        const char* name,
        ExecutionContextId executionContextId,
        bool registerProvider = true,
        Whiteboard& rWhiteboard = getWhiteboardInstance())
        : TestProviderBase(name, executionContextId, rWhiteboard),
          mResource(name, executionContextId, 0, rWhiteboard),
          mRegisterProvider(registerProvider)
    {
        if (mRegisterProvider)
        {
            DpcFunctor::syncQueueOnce(executionContextId, mrWhiteboard, this, &TestProvider::initInWbContext);
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
    TestProvider(
        const char* name,
        ExecutionContextId executionContextId,
        bool registerProvider,
        const T& rInitialValue,
        Whiteboard& rWhiteboard = getWhiteboardInstance())
        : TestProviderBase(name, executionContextId, rWhiteboard),
          mResource(name, executionContextId, 0, rWhiteboard),
          mRegisterProvider(registerProvider)
    {
        setValue(rInitialValue);
        if (mRegisterProvider)
        {
            DpcFunctor::syncQueueOnce(executionContextId, mrWhiteboard, this, &TestProvider::initInWbContext);
        }
    }

    /** Desctructor */
    virtual ~TestProvider()
    {
        if (mRegisterProvider)
        {
            DpcFunctor::syncQueueOnce(this->ResourceProvider::getExecutionContextId(), mrWhiteboard, this, &TestProvider::deinitInWbContext);
        }
    }

    /**
    Functor callback for init DPC
    */
    void initInWbContext()
    {
        this->ResourceProvider::registerProviderResource(mResource.getId().localResourceId);
    }

    /**
    Functor callback for deinit DPC
    */
    void deinitInWbContext()
    {
        this->ResourceProvider::unregisterProviderResource(mResource.getId().localResourceId);
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
    const ResourceId getResourceId() const { return mResource.getId(); }

    /**
    *	Function to notify subscribers. Will pass value of member variable mValue.
    */
    void notifySubscribers()
    {
        NotifyStatus status(*this, mResource.getId(), mValue);
        DpcFunctor::queueOnce(getExecutionContextId(), mrWhiteboard, &TestProvider::notifyDpcHandler, &status);
        WbSemaphoreTryWait(status.mComplete, 500);
    }

    /**
    *	GET request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *	@return Result of the operation
    */
    virtual void onGetRequest(
        const Request& rRequest, const ParameterList& /* rParameters */) OVERRIDE
    {
        WB_ASSERT(rRequest.getResourceId() == mResource.getId());
        return returnResult(
            rRequest, HTTP_CODE_OK, ResourceProvider::ResponseOptions::Empty, static_cast<T>(mValue));
    }

    /**
    *	PUT request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *	@return Result of the operation
    */
    virtual void onPutRequest(
        const Request& rRequest, const ParameterList& rParameters) OVERRIDE
    {
        this->mValue = rParameters[0].convertTo<T>();
        return returnResult(rRequest, HTTP_CODE_OK);
    }

    /**
    *	POST request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *	@return Result of the operation
    */
    virtual void onPostRequest(
        const Request& rRequest, const ParameterList& /*rParameters*/) OVERRIDE
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
    virtual void onDeleteRequest(
        const Request& rRequest, const ParameterList& /*rParameters*/) OVERRIDE
    {
        return returnResult(rRequest, HTTP_CODE_OK);
    }

protected:
    ScopedResource<T, METADATA_CREATOR> mResource;
    bool mRegisterProvider;
    TestValue<typename RemoveAll<T>::type> mValue;

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
        status->mProvider.updateResource(status->mResourceId, ResourceProvider::ResponseOptions::Empty, status->mrValue);
        WbSemaphoreRelease(status->mComplete);
    }
};

} // namespace whiteboard

#endif
