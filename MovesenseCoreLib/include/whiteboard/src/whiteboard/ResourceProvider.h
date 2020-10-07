#pragma once
// Copyright (c) Suunto Oy 2014. All rights reserved.

#include "whiteboard/ApiHelpers.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/Functor.h"
#include "whiteboard/ParameterList.h"
#include "whiteboard/Result.h"
#include "whiteboard/Request.h"
#include "whiteboard/Value.h"

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs" // Caused by the WB_STATIC_VERIFY calls below
#endif

WB_HEADER_CHECK_DEFINE(WB_HAVE_DEBUG_NAMES);
WB_HEADER_CHECK_DEFINE(WB_UNITTEST_BUILD);

namespace whiteboard
{

// Forward declarations
class ResourceProviderAccessor;
class Whiteboard;

/** Options for the response can be given with this class. */
class WB_API ResourceProvider_ResponseOptions
{
public:
    /** Constructor
    *
    * @param forceAsync [in] Forces the request to be performed asynchronously, even if the resource is in the same 
    *                        context as the client.
    */
    EXPLICIT ResourceProvider_ResponseOptions(bool forceAsync)
        : mForceAsync(forceAsync)
    {}

    /**
    @return true if the operation should be explicitly performed asyncronously.
    By default whiteboard performs operation directly, if the requested resource is in the same context.
    */
    inline bool getForceAsync() const { return mForceAsync; }

    /// Empty async request options, that can be used if no options required for the operation
    static const ResourceProvider_ResponseOptions Empty;

    /// Convinience to set response as asynchronoys always
    static const ResourceProvider_ResponseOptions ForceAsync;

private:
    /** Prevent use of default constructor */
    ResourceProvider_ResponseOptions() DELETED;

private:
    /** A value indicating whether request should be performed asynchronously */
    bool mForceAsync;
};

/**
    Base class for all whiteboard resource providers
*/
class WB_API ResourceProvider
{
public:

    // This alias allows forward declaration of the options in internal classes
    typedef ResourceProvider_ResponseOptions ResponseOptions;

    /**
    *	Initializes a new instance of the ResourceProvider class
    *
    *	@param pProviderName Name of the provider
    *	@param executionContextId ID of provider's execution context
    */
    ResourceProvider(const char* pProviderName, ExecutionContextId executionContextId);

    /**
    *	Destructor. Unbinds this provider.
    */
    virtual ~ResourceProvider();

    /**
        Get the name of a resource provider.
    */
    const char* getName() const;

    /**
    *	Gets ID of the provider
    *
    *	@return ID of the registered provider
    */
    ProviderId getId() const;

    /**
    *	Gets local ID of the provider
    *
    *	@return Local ID of the registered provider
    */
    LocalProviderId getLocalProviderId() const;

    /** Gets provider's execution context
    *
    * @return Reference to the provider's execution context
    */
    ExecutionContextId getExecutionContextId() const;

    /**
    *	Resolves local resource path.
    *
    *	@param pFullPath Path of the resource
    *	@param rResourceId On successful output contains the resource identifier
    *	@return Result of the operation
    */
    Result getLocalResource(const char* pFullPath, ResourceId& rResourceId);

    /**
    *   Registers array of provider of resources
    *
    *   @note If any the resources fail to register, all already registered
    *   resources are unregister.
    *
    *   @param numberOfResources Number of resources to register
    *   @param pResourceIds Array of resources to register
    *   @return Result of the operation.
    */
    Result registerProviderResources(size_t numberOfResources, const LocalResourceId* const pResourceIds);

    /**
    *	Registers provider for a resource
    *
    *	@param resourceId ID of the associated resource
    *	@return Result of the operation
    */
    inline Result registerProviderResource(LocalResourceId resourceId)
    {
        return registerProviderResources(1, &resourceId);
    }
        
    /**
    *   Registers array of provider of resources
    *
    *   @note If any the resources fail to register, all already registered
    *   resources are unregister.
    *
    *   @param rResourceIds Array of resources to register
    *   @return Result of the operation.
    */
    template <size_t NUMBER_OF_RESOURCES>
    inline Result registerProviderResources(const LocalResourceId(&rResourceIds)[NUMBER_OF_RESOURCES])
    {
        return registerProviderResources(NUMBER_OF_RESOURCES, &rResourceIds[0]);
    }

    /**
    *	Unregisters provider from resources
    *
    *   @param numberOfResources Number of resources to unregister
    *   @param pResourceIds Array of resources to unregister
    *	@return Result of the operation
    */
    Result unregisterProviderResources(size_t numberOfResources, const LocalResourceId* const pResourceIds);

    /**
    *	Unregisters provider from a resource
    *
    *	@param resourceId ID of the associated resource
    *	@return Result of the operation
    */
    inline Result unregisterProviderResource(LocalResourceId resourceId)
    {
        return unregisterProviderResources(1, &resourceId);
    }

    /**
    *   Unregisters provider from a resources
    *
    *   @note If any the resources fail to unregister, function will continue with next resource.
    *
    *   @param rResourceIds Array of resources to unregister
    *   @return Result of the operation. In case of resource unregister fails, this function will
    *   return first error code of the operation.
    */
    template <size_t NUMBER_OF_RESOURCES>
    inline Result unregisterProviderResources(const LocalResourceId(&rResourceIds)[NUMBER_OF_RESOURCES])
    {
        return unregisterProviderResources(NUMBER_OF_RESOURCES, &rResourceIds[0]);
    }

    /**
    *	Returns result for a request
    *
    *	@tparam V Native type for the result to be returned
    *
    *	@param rRequest Request information
    *	@param resultCode Result code of the request
    *	@param rOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *	@param rResult Result data of the request
    */
    WB_FORCE_INLINE
    template <typename V = NoType>
    WB_FORCE_INLINE_ATTRIBUTE void returnResult(
        const Request& rRequest,
        Result resultCode,
        const ResponseOptions& rOptions = ResponseOptions::Empty,
        const V& result = NoType::NoValue)
    {
        WB_STATIC_VERIFY((IsSame<V, Value>::value == 0), Raw_value_must_be_given_not_Value_wrapper);
        return returnResultImpl(
            rRequest,
            resultCode,
            rOptions,
            static_cast<typename Api::ParameterType<V>::type>(result));
    }

    /**
    *	Returns result for a request
    *
    *	@tparam V Native type for the result to be returned
    *
    *	@param rRequest Request information
    *	@param resultCode Result code of the request
    *	@param rOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *	@param rResult Result data of the request
    */
    WB_FORCE_INLINE
    template <
        typename RESPONSE, typename V = NoType,
        typename EnableIf<!IsSame<RESPONSE, Result>::value, int>::type = 0>
    WB_FORCE_INLINE_ATTRIBUTE void returnResult(
        const Request& rRequest,
        const RESPONSE& rResponse,
        const ResponseOptions& rOptions = ResponseOptions::Empty,
        const V& result = NoType::NoValue)
    {
        RESPONSE::typeCheck(result);
        return returnResultImpl(
            rRequest,
            static_cast<Result>(rResponse) | TYPE_CHECKED,
            rOptions,
            static_cast<typename Api::OptionalParameterType<
                V, typename Api::ParameterType<typename RESPONSE::Type>::type>::type>(result));
    }

    /**
    *	Sends resource changed notification for all subscribed clients
    *
    *	@tparam V Native type for the updateResource result.
    *	@tparam P1 .. P8 Optional. Native type for the update resource parameters.
    *
    *	@param resourceId ID of the associated resource
    *	@param rOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *	@param rValue Current value of the resource
    *	@param rP1 .. rP8 If path parameters are used, add the path parameter as a parameter. Path parameter closest to the
    *   root shall be given first
    */
    template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
    inline Result updateResource(
        LocalResourceId localResourceId,
        const ResponseOptions& rOptions,
        const V& rValue,
        const P1& rP1,
        const P2& rP2,
        const P3& rP3,
        const P4& rP4,
        const P5& rP5,
        const P6& rP6,
        const P7& rP7,
        const P8& rP8);

    // Include overloads that have different number of parameters (we don't want to clutter this header)
    #include "ResourceProvider_UpdateResourceOverloads.h"

    /**
    *	Check if resource has any subscriptions.
    *
    *	@param resourceId ID of the resource that is checked
    *	@return
    *		HTTP_CODE_OK if resource has been subscribed,
    *		HTTP_CODE_NO_CONTENT if resource is not subscribed,
    *		HTTP_CODE_NOT_FOUND indicating an error (provider has not registered the resource)
    */
    Result isResourceSubscribed(ResourceId resourceId);

    /** 
    EnumerateSubscriptions requires functor with 3 parameters, this definition for that.

    @see whiteboard::ResourceProvider::enumerateSubscriptions
    */
    typedef IFunctor3<void, ResourceId, ClientId, const ParameterList&> IEnumerateSubscriptionsFunctor;
    
    /** 
    Wrapper struct for defining EnumerateSubscriptions functor where the class that should be called is still left as a template parameter
    */
    template <typename T> struct EnumerateSubscriptions
    {
        /// Functor for enumerateSubscriptions, @see whiteboard::ResourceProvider::enumerateSubscriptions
        typedef MemberFunctor3<T, void, ResourceId, ClientId, const ParameterList&> Functor;
    };

    /**
    *	Enumerate all the subscribers of the given resource. Calls callback onEnumerateSubscribers for each
    *   subscription. This method is synchronous, so the caller can trust that all callbacks have been called
    *   when this call returns.
    *
    *   @see whiteboard::ResourceProvider::EnumerateSubscriptions
    *   @see whiteboard::ResourceProvider::EnumerateSubscriptions::Functor
    *
    *	@param resourceId ID of the resource that is to be enumrated
    *   @param rFunctor Functor to call for each subscription @see whiteboard::ResourceProvider::IEnumerateSubscriptionsFunctor
    *
    *	@return
    *		HTTP_CODE_OK if resource has been subscribed, and enumerated
    *		HTTP_CODE_NO_CONTENT if resource is not subscribed, enumerating skipped
    *		HTTP_CODE_NOT_FOUND indicating an error (provider has not registered the resource)
    *
    *  * Example Usage:
    * @code
    *   // The onEnumerateSubscriptions is a method of the MyProvider with signature: 
    *   // void onEnumerateSubscriptions(ResourceId resourceId, ClientId clientId, const ParameterList& rPathParameters);
    *   ResourceProvider::EnumerateSubscriptions<MyProvider>::Functor functor(this, &MyProvider::onEnumerateSubscriptions);
    *   enumerateSubscriptions(mTestResource, functor);
    * @endcode
    */
    Result enumerateSubscriptions(ResourceId resourceId, IEnumerateSubscriptionsFunctor& rFunctor);

#ifdef WB_HAVE_TIMERS
    /**
    *	Starts a continuous timer with given period. Override whiteboard::ResourceProvider::onTimer to handle timer notifications.
    *   
    *   Note: If previous timer message has not been processed, for example due to execution context blocking on long operation -
    *   and short interval continuous timer is in use, the timer messages might be filtered out to prevent flooding the message
    *   queues.
    *
    *   Note: Starting a timer from interrupt is not supported.
    *
    *	@param periodMs Timer in milliseconds how often the timer should fire.
    *	@param isContinous If true, continuous timer, else single shot.
    *
    *	@return ID of the started timer or ID_INVALID_TIMER, if starting a new timer fails (WB_MAX_NUMBER_OF_CLIENT_TIMERS is
    *           exceeded).
    *
    *	@see whiteboard::ResourceProvider::onTimer
    */
    TimerId startTimer(size_t periodMs, bool isContinuous = false);

    /**
    *   Reschedules the continuous timer with a new period; The existing timer is stopped and restarted (but not deallocated).
    *
    *   Note: A timer message with the previous timer interval might have been queued before processing the rescheduling.
    *   and hence the first time might fire in period less than the new period.
    *
    *   @param timerId Id of the timer that is running
    *	@param periodMs New period in milliseconds how often the timer should fire.
    *
    *	@see whiteboard::ResourceClient::onTimer
    */
    bool rescheduleTimer(TimerId timerId, size_t periodMs);

    /**
    *	Stops a timer started with startTimer. No notifications of the given timer are received after this call returns
    *
    *   Note: In case of continuous timer, a notification might be received after the stop if the stop is called from
    *   another thread than the providers execution context. Hence using timer start / stop from another thread is discouraged.
    *
    *   Note: Stopping a timer from interrupt is not supported.
    *
    *	@param timerId Id of the timer to stop
    *
    *	@return true on success, false if timer was already stopped or this provider did not own the timer, or if id was illegal.
    *
    *	@see whiteboard::ResourceProvider::startTimer
    */
    bool stopTimer(TimerId timerId);
#endif

    /**
    *   This unsafe method is only for adapting other interfaces on top of Whiteboard!
    *
    *	Returns result for a request
    *
    *	@param rRequest Request information
    *	@param resultCode Result code of the request
    *   @param rOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *	@param rResult Result data of the request
    */
    WB_FORCE_INLINE
    WB_FORCE_INLINE_ATTRIBUTE void returnResultVariant(
        const Request& rRequest, Result resultCode, const ResponseOptions& rOptions, const Value& rResult)
    {
        return returnResultInternal(rRequest, resultCode, rOptions, rResult);
    }

    /**
    *   This unsafe method is only for adapting other interfaces on top of Whiteboard!
    *
    *	Sends resource changed notification for all subscribed clients
    *
    *	@param localResourceId ID of the associated resource
    *   @param rOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *	@param rValue Current value of the resource
    *	@param rParameterList If path parameters are used, add the path parameters to the parameter list. Path parameter closest to
    *   the root is first in the list.
    *	@return Result of the operation
    */
    WB_FORCE_INLINE
    Result WB_FORCE_INLINE_ATTRIBUTE updateResourceVariant(
        LocalResourceId localResourceId, const ResponseOptions& rOptions, const Value& rValue, const ParameterList& rParameterList)
    {
        return updateResourceInternal(localResourceId, rOptions, rValue, rParameterList);
    }

protected:
    /** These functions are called only by Whiteboard */
    friend class Whiteboard;
    friend class TimerPool;

#if WB_UNITTEST_BUILD
    friend class TestProviderBase;

    /** Gets the associated whiteboard instance
    *
    * @return Whiteboard instance
    */
    virtual Whiteboard& getWhiteboard();
#endif

    /**
    *	Sets ID of the provider
    *
    *	@param providerId New ID for the provider
    */
    void setLocalProviderId(const LocalProviderId providerId);

    /**
    *	GET request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request, if resource path contains path parameters,
    *			the path parameters are added to the beginning of the parameter list.
    *	@return Result of the operation
    */
    virtual void onGetRequest(const Request& rRequest, const ParameterList& rParameters);

    /**
    *	PUT request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request, if resource path contains path parameters,
    *			the path parameters are added to the beginning of the parameter list.
    *	@return Result of the operation
    */
    virtual void onPutRequest(const Request& rRequest, const ParameterList& rParameters);

#ifdef WB_HAVE_DEPRECATED_BYTE_STREAM
    /**
    *   PUT stream handler.
    *   This is invoked when a client posts a PUT request with a stream parameter as one of the parameters.
    *
    *   @param rRequest Request information
    *   @param rStreamValue Reference to the Value pointing to the stream type itself
    */
    virtual void onPutStream(const Request& rRequest, const Value& rStreamValue);
#endif

    /**
    *	POST request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request, if resource path contains path parameters,
    *			the path parameters are added to the beginning of the parameter list.
    *	@return Result of the operation
    */
    virtual void onPostRequest(const Request& rRequest, const ParameterList& rParameters);

    /**
    *	DELETE request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request, if resource path contains path parameters,
    *			the path parameters are added to the beginning of the parameter list.
    *	@return Result of the operation
    */
    virtual void onDeleteRequest(const Request& rRequest, const ParameterList& rParameters);

    /**
    *	Subscribe notification callback.
    *
    *	Note that if provider transfers the ownership of the rRequest to Provider's own request object
    *	by copying the request object, the subscription is always added to Whiteboard's subscription list -
    *	even if provider later returns error code in returnResult.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request, if resource path contains path parameters,
    *			the path parameters are added to the beginning of the parameter list.
    *	@return Result of the operation
    */
    virtual void onSubscribe(const Request& rRequest, const ParameterList& rParameters);

    /**
    *	Unsubscribe notification callback.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request, if resource path contains path parameters,
    *			the path parameters are added to the beginning of the parameter list.
    *	@return Result of the operation.
    */
    virtual void onUnsubscribe(const Request& rRequest, const ParameterList& rParameters);

#ifdef WB_HAVE_TIMERS
    /**
    *	Callback for timer notifications.
    *
    *	@param timerId ID of the timer that has expired.
    *
    *	@see whiteboard::ResourceProvider::startTimer
    *	@see whiteboard::ResourceProvider::stopTimer
    */
    virtual void onTimer(TimerId timerId);
#endif

#ifdef WB_HAVE_TIMED_DPC
    /**
    *   Queue an low priority timed DPC to be run after deltaTime. A timed DPC is freed upon callback execution
    *   if the onTimedDpc(...) implementation returns false, in this case no cancellation is necessary.
    *
    *   @param deltaTimeMs Delta in ms to the time when the ::onTimedDpc callback is to be executed.
    *   @return A valid TimedDpcId value if successful, ID_INVALID_TIMED_DPC if unsuccessful.
    */
    TimedDpcId queueTimedDpc(size_t deltaTimeMs);

    /**
    *   Cancel a timed DPC.
    *
    *   @param timedDpcId Reference to the ID of the timed DPC. Will be invalidated.
    *   @return HTTP_CODE_OK if successful.
    *           HTTP_CODE_NOT_FOUND if a DPC with given stamp was not found for the entity ID.
    *           HTTP_CODE_RANGE_NOT_SATISFIABLE if given timed DPC id is out of bounds.
    */
    Result cancelTimedDpc(TimedDpcId& timedDpcId);

    /**
    *   Cancel all timed DPCs queued by the resource provider.
    *
    *   @return Returns the amount of DPCs that were active and cancelled.
    */
    uint32 cancelAllTimedDpcs();

    /**
    *   Callback for timed DPCs (low priority).
    *
    *   @param timedDpcId ID of the timed DPC given by queueTimedDcp(...).
    *   @return ZERO if no reschedule required, a value in milliseconds to indicate time to next reschedule.
    */
    virtual size_t onTimedDpc(TimedDpcId timedDpcId);
#endif

    /**
    *  Whiteboard disconnect notification handler.
    *
    *  This can be used for example to cleanup possible subscription related information of clients from 
    *  the remote whiteboard.
    *
    *  @param whiteboardId ID of the whiteboard that has been disconnected.
    *
    *  @see whiteboard::ResourceProvider::onSubscribe
    *  @see whiteboard::ResourceProvider::onUnsubscribe
    */
    virtual void onRemoteWhiteboardDisconnected(WhiteboardId whiteboardId);
    
    /**
    * Local client 'disconnect' notification handler.
    *
    *  This can be used for example to cleanup possible subscription related information of the client.
    *
    *  @see whiteboard::ResourceProvider::onSubscribe
    *  @see whiteboard::ResourceProvider::onUnsubscribe
    */
    virtual void onClientUnavailable(ClientId clientId);

private:

    /**
    *	Returns result for a request
    *
    *	@tparam V Native type for the result to be returned
    *
    *	@param rRequest Request information
    *	@param resultCode Result code of the request piggy backed with type check info
    *	@param rOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *	@param rResult Result data of the request
    */
    template <typename V = NoType>
    inline void returnResultImpl(
        const Request& rRequest,
        uint32 resultCode,
        const ResponseOptions& rOptions = ResponseOptions::Empty,
        const V& result = NoType::NoValue)
    {
        return returnResultInternal(
            rRequest,
            resultCode,
            rOptions,
            (IsSame<V, NoType>::value == 1) ? Value::Empty : Value(result));
    }

    /**
    *	Returns result for a request
    *
    *	@param rRequest Request information
    *	@param resultCode Result code of the request piggy backed with type check info
    *   @param rOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *	@param rResult Result data of the request
    */
    void returnResultInternal(
        const Request& rRequest, uint32 resultCode, const ResponseOptions& rOptions, const Value& result);

    /**
    *	Sends resource changed notification for all subscribed clients
    *
    *	@param localResourceId ID of the associated resource piggy backed with type check info
    *   @param rOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *	@param rValue Current value of the resource
    *	@param rParameterList If path parameters are used, add the path parameters to the parameter list. Path parameter closest to
    *   the root is first in the list.
    *	@return Result of the operation
    */
    Result updateResourceInternal(
        uint32 localResourceId, const ResponseOptions& rOptions, const Value& value, const ParameterList& rParameterList);

#ifdef WB_HAVE_DEPRECATED_BYTE_STREAM
    /**
    *   System part of PUT handler. Depending on data will call onPutRequest or onPutStream
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request, if resource path contains path parameters,
    *			the path parameters are added to the beginning of the parameter list.
    */
    void onPutSys(const Request& rRequest, const ParameterList& rParameters);
#endif

private:

    /** 
    BypassProvider needs to access the ProviderId when it bypasses a resource to tell WB security checks and typechecks
    that the resource held by this provider has been bypassed and checks should be skipped.
    */
    friend class ResourceProviderAccessor;

#ifdef WB_HAVE_DEBUG_NAMES
    /**
    * Name of the provider
    */
    const char* mpName;
#endif

    /** Tells if this provider bypasses a resource */
    bool mIsBypass;

    /** Reserved for future use */
    uint8 mReserved : 4;

    /** ID of the execution context */
    ExecutionContextId mExecutionContextId : 4;

    /**
    * ID of the provider
    */
    LocalProviderId mId;

protected:

    /** Helper constant that allows us to piggyback type check info on result code and local resource ID
    * parameter which generates more optimal code. */
    static const uint32 TYPE_CHECKED = 65536;

private:

    // disallow C++ default constructor and copy constructor usage
    ResourceProvider() DELETED;
    ResourceProvider(ResourceProvider&) DELETED;
};

template <typename V, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline Result ResourceProvider::updateResource(
    LocalResourceId localResourceId,
    const ResourceProvider::ResponseOptions& rOptions,
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
    return updateResourceImpl(
        localResourceId,
        rOptions,
        static_cast<typename Api::ParameterType<V>::type>(rValue),
        static_cast<typename Api::ParameterType<P1>::type>(rP1),
        static_cast<typename Api::ParameterType<P2>::type>(rP2),
        static_cast<typename Api::ParameterType<P3>::type>(rP3),
        static_cast<typename Api::ParameterType<P4>::type>(rP4),
        static_cast<typename Api::ParameterType<P5>::type>(rP5),
        static_cast<typename Api::ParameterType<P6>::type>(rP6),
        static_cast<typename Api::ParameterType<P7>::type>(rP7),
        static_cast<typename Api::ParameterType<P8>::type>(rP8));
}

} // namespace whiteboard

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
