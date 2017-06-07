#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/ResourceProvider.h"
#include "whiteboard/ResourceClient.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"
#include "whiteboard/containers/RequestMap.h"

WB_HEADER_CHECK_DEFINE(WB_HAVE_BYPASS)

namespace whiteboard
{
namespace services
{

/** Base class for bypassed resource */
class BypassResource : public IDynamicallyAllocatable
{
public:
    /** Constructor 
     *
     * @param resourceId ID of the bypassed resource
     * @param isGetOperation A value indicating wether the information was retrieved from GET or SUBSCRIBE
     * @param hasExtraParameters A value indicating wether the operation has extra parameters
     * @param valueType Type of the value
     */
    BypassResource(
        ResourceId resourceId, bool isGetOperation, bool hasExtraParameters, ValueType valueType);

    /** Destructor */
    virtual ~BypassResource();

    void update(ResourceProvider& rResourceProvider);
    virtual Value& getValue() = 0;

    ResourceId mResourceId;
    uint8 mIsGetOperation : 1;
    uint8 mHasExtraParameters : 1;
    ValueType mValueType;
    LocalProviderId mOriginalProvider;
    BypassResource* mpNext;

private:
    /** Prevent use of default constructor */
    BypassResource() DELETED;
    /** Prevent use of copy constructor */
    BypassResource(BypassResource&) DELETED;
    /** Prevent use of assigment operator */
    const BypassResource& operator=(const BypassResource&) DELETED;
};

/** Type specific data storage class template for bypassed resource */
template <typename T> class Data FINAL : public BypassResource
{
public:
    Data(ResourceId resourceId, bool isGetOperation, bool hasExtraParameters)
        : BypassResource(resourceId, isGetOperation, hasExtraParameters, Value::NativeValueType<T>::value),
          mValue()
    {
        memset(&mData, 0, sizeof(T));
    }

    virtual Value& getValue() OVERRIDE { return mValue; }
    void setValue(const T& rData)
    {
        mData = rData;
        mValue.assign(Value(mData));
    }

private:
    /** Prevent use of default constructor */
    Data() DELETED;
    /** Prevent use of copy constructor */
    Data(Data&) DELETED;
    /** Prevent use of assigment operator */
    const Data& operator=(const Data&) DELETED;

    // The data
    T mData;

    /// Value object
    Value mValue;
};

/** Type specific data storage class for bypassed resource */
template <> class Data<char*> FINAL : public BypassResource
{
public:
    Data(ResourceId resourceId, bool isGetOperation, bool hasExtraParameters)
        : BypassResource(resourceId, isGetOperation, hasExtraParameters, WB_TYPE_STRING),
          mData(static_cast<char*>(WbMemAlloc(1)))
    {
        *mData = '\0';
    }

    virtual ~Data()
    {
        WbMemFree(mData);
    }

    Value& getValue() OVERRIDE { return mValue; }
    void setValue(const char* data)
    {
        size_t length = strlen(data);
        if (length > strlen(mData))
        {
            WbMemFree(mData);
            mData = static_cast<char*>(WbMemAlloc(length + 1));
        }

        memcpy(mData, data, length + 1);
        mValue.assign(Value(mData));
    }

private:
    /** Prevent use of default constructor */
    Data() DELETED;
    /** Prevent use of copy constructor */
    Data(Data&) DELETED;
    /** Prevent use of assigment operator */
    const Data& operator=(const Data&) DELETED;

    /// String data
    char* mData;

    /// Value object
    Value mValue;
};

/** Type specific data storage class template for bypassed resource */
class UnknownStructureBypassResource FINAL : public BypassResource
{
public:
    /** Constructor
     *
     * @param resourceId ID of the bypassed resource
     */
    UnknownStructureBypassResource(ResourceId resourceId, bool isGetOperation, bool hasExtraParameters)
        : BypassResource(resourceId, isGetOperation, hasExtraParameters, Value::NativeValueType<UnknownStructure>::value),
          mUnknownStructure(),
          mValue()
    { }

    /** Destructor */
    ~UnknownStructureBypassResource()
    {
    }

    /** @return The value wrapper of the unknown structure held by this BypassResource */
    Value& getValue() OVERRIDE
    { 
        return mValue;
    }

    /**  Takes copy of the given unknown structure, deallocates previous one if required.
    @param rUnknownStructure [in] The actual unknown structure
    */
    inline void setValue(const UnknownStructure& rUnknownStructure) 
    {
        mUnknownStructure = rUnknownStructure.copy();
        mValue.assign(Value(mUnknownStructure));
    };

private:
    /// Dynamically allocated buffer that will contain the unknown structure with one byte alignment between structure header and the data
    UnknownStructure mUnknownStructure;

    /// Value that stores current deserialization state of the unknown structure
    Value mValue;

    /** Prevent use of default constructor */
    UnknownStructureBypassResource() DELETED;
    /** Prevent use of copy constructor */
    UnknownStructureBypassResource(UnknownStructureBypassResource&) DELETED;
    /** Prevent use of assigment operator */
    const UnknownStructureBypassResource& operator=(const UnknownStructureBypassResource&) DELETED;
};

/** Data empty, typeless class for remote bypassed resources */
class RemotelyBypassedResource FINAL : public BypassResource
{
public:
    RemotelyBypassedResource(ResourceId resourceId)
        : BypassResource(resourceId, false, false, WB_TYPE_NONE) {}

    virtual Value& getValue() OVERRIDE 
    { 
        WB_ASSERT(0);
        return const_cast<Value&>(Value::Empty);
    }

private:
    /** Prevent use of default constructor */
    RemotelyBypassedResource() DELETED;
    /** Prevent use of copy constructor */
    RemotelyBypassedResource(RemotelyBypassedResource&) DELETED;
    /** Prevent use of assigment operator */
    const RemotelyBypassedResource& operator=(const RemotelyBypassedResource&) DELETED;
};

/** Provider for setting up and restoring bypassed resources
*
*	Will create SubstituteProvider object for each required execution context and
*	register it serve the bypassed resources.
*/
class BypassProvider : protected ResourceProvider, private ResourceClient
{
public:
    /** Initializes a new instance of BypassProvider
    */
    BypassProvider();

    /** Destructor */
    virtual ~BypassProvider();

    /** Initializes BypassProvider. */
    bool initSubmodule();

    /** Deinitializes BypassProvider. */
    void deinitSubmodule();

protected: // <-- Protected due to unittests

    /** Substitute provider for serving bypassed resources in specific execution context
    *
    *	This will create BypassResource object for each bypassed resources and
    *	overtake the original resource provider.
    */
    class SubstituteProvider FINAL : public ResourceProvider, public IDynamicallyAllocatable, private ResourceClient
    {
    public:
        SubstituteProvider(ExecutionContextId executionContextId);
        ~SubstituteProvider();
        void removeBypasses();
        void registerSubstitute();
        void unregisterSubstitute();
        bool add(BypassResource* pBypassResource);
        bool remove(BypassResource* pBypassResource);
        BypassResource* find(ResourceId resourceId);

        using ResourceProvider::getExecutionContextId;

        BypassResource* mpFirst;
        BypassResource* mpAddThis;
        WbSemaphoreHandle mSema;
        SubstituteProvider* mpNext;

        ResourceId mRemoteResourceId;
        bool mIsRemoteProvider;
        bool mRemovalQueued;
        RequestMap<4, void> mOngoingRequests;

    private:

        /// Common handler for GET and SUBSCRIBE
        void onRequest(const Request& rRequest, const ParameterList& rParameters);

        /// @see whiteboard::ResourceProvider::onGetRequest
        void onGetRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

        /// @see whiteboard::ResourceProvider::onPutRequest
        void onPutRequest(const Request& rRequest, const ParameterList& rParameterss) OVERRIDE;

        /// @see whiteboard::ResourceProvider::onSubscribe
        virtual void onSubscribe(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

        /// Common handler for local GET and SUBSCRIBE operation results
        void onLocalCurrentValueResult(
            ResourceId resourceId, Result resultCode, const Value& rResultData);

        /// Common handler for remote GET and SUBSCRIBE operation results
        void onRemoteProviderResult(
            RequestType requestType, RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResultData);

        /// @see whiteabord::ResourceClient::onGetResult
        void onGetResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResultData) OVERRIDE;

        /// @see whiteboard::ResourceClient::onPutResult
        virtual void onPutResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResultData) OVERRIDE;

        /// @see whitebord::ResourceClient::onSubscribeResult
        virtual void onSubscribeResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResultData) OVERRIDE;

        /// @see whiteabord::ResourceProvider::onRemoteWhiteboardDisconnected()
        virtual void onRemoteWhiteboardDisconnected(WhiteboardId whiteboardId) OVERRIDE;

        /** Prevent use of default constructor */
        SubstituteProvider() DELETED;
        /** Prevent use of copy constructor */
        SubstituteProvider(SubstituteProvider&) DELETED;
        /** Prevent use of assigment operator */
        const SubstituteProvider& operator=(const SubstituteProvider&) DELETED;
    };

    /** Finds SubstituteProvider with given executionContextId and resource id
     *
     * @param executionContextId ID of the expected execution context.
     * @param bypassResourceId Optional. ID of the expected resource.
     * @return Reference to matching SubstituteProvider
     */
    SubstituteProvider& find(ExecutionContextId executionContextId, ResourceId bypassResourceId = ID_INVALID_RESOURCE);

    /// @see whiteabord::ResourceProvider::onGetRequest
    void onGetRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

    /// @see whiteabord::ResourceProvider::onPutRequest
    void onPutRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

    /** Gets information about the bypassed resource
    * 
    * @param resourceId ID of the resource that should be bypassed
    * @param rIsGetOperation On output contains a value indicating whether the information is from GET operation or SUBSCRIBE notify
    * @param rHasExtraParameters On output contains a value indicating wether operation needs extra parameters
    * @param rResponseDataType On output contains data type of the response
    * @return A value indicating wether the operation completed successfully
    */
    bool getResourceInfo(
        ResourceId resourceId, bool& rIsGetOperation, bool& rHasExtraParameters, ValueType& rResponseDataType);

    /**
    * Handler for PUT requests that specify that a provider should be bypassed remotely.
    *
    * @param rRequest Request information
    * @param rParameters first parameter is the path name (string) of the resource to be bypassed
    * @param bypassResourceId bypass resource ID
    * @return result of remote bypassing operation
    */
    Result handleRemoteBypassing(
        const Request& rRequest, 
        const ParameterList& rParameters,
        ResourceId bypassResourceId);

    ///  @see whiteabord::ResourceClient::onGetRequest
    void onGetResourceResult(RequestId requestId, ResourceId resourceId, Result resultCode) OVERRIDE;

    /** Helper method for returning bypass result
     *
     * @param rRequest Request information
     * @param result Request result
     * @param resourceId ID of the bypassed resource
     */
    void returnBypassResult(const Request& rRequest, Result result, ResourceId resourceId);

    /**
    *	Handler for PUT requests that specify that a provider should be bypassed remotely.
    *
    *	@param bypassResourceId bypass resource ID
    *	@return result of local bypassing operation
    */
    Result handleLocalBypassing(ResourceId bypassResourceId);

    /// Helper to unregister given substitute provider
    void unregisterSubstituteProvider(SubstituteProvider& rSubstitute);

    /** Prevent use of copy constructor */
    BypassProvider(BypassProvider&) DELETED;
    /** Prevent use of assigment operator */
    const BypassProvider& operator=(const BypassProvider&) DELETED;

protected:
    /** First substitute provider */
    SubstituteProvider* mpFirst;

    /** Semaphore used to synchronize substitute unregistration */
    WbSemaphoreHandle mUnregisterSemaphore;

    /** Request map for pending remote bypass requests */
    RequestMap<4, ResourceId> mOngoingRemoteBypassRequests;
};

} // namespace services
} // namespace whiteboard
