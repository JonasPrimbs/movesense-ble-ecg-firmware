// Copyright (c) Suunto Oy 2014. All rights reserved.
#pragma once

#include "whiteboard/integration/port.h"
#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/ParameterList.h"
#include "whiteboard/ResourceProvider.h"
#include "whiteboard/containers/NullPointerPoolAllocator.h"
#include "whiteboard/comm/internal/IRoutingTableObserver.h"
#include "whiteboard/internal/PathParameterCache.h"
#include "whiteboard/internal/ResourceTree.h"
#include "whiteboard/internal/Status.h"
#include "whiteboard/internal/StreamPool.h"
#include "whiteboard/internal/SubscriptionRegistry.h"
#include "whiteboard/internal/TimerPool.h"
#include "whiteboard/internal/protocol/IMessageHandler.h"
#include "whiteboard/ResourceTree.h"

namespace whiteboard
{

// Forward declarations
class BufferPool;
class Dpc;
class DpcFunctor;
class ExecutionContextMap;
class LocalExecutionContext;
class MessageHandler;
class ResourceProvider;
class ResourceProvider_ResponseOptions;
class ResourceClient;
class ResourceClient_AsyncRequestOptions;
class SuuntoSerial;
class Value;
class WhiteboardCommunication;

namespace internal
{
    struct ResourceSubscriptionData;
    class ResourceSubtree;
    class ResourceTree;
}

// TODO: Remove this when all of internal code has been moved to internal namespace
namespace internal { struct ResourceSubscriptionData;  }
using internal::ResourceSubscriptionData;

namespace services
{
class BypassProvider;
class MetadataProvider;
class PoolStatsProvider;
class SubscriptionsInfoProvider;
}

typedef TypedPool<LocalProviderId, ResourceProvider*> ProviderRegistry;
typedef TypedPool<LocalClientId, ResourceClient*> ClientRegistry;

/**
    Whiteboard core implementation. The Whiteboard provides a high level
    service API for other service provider modules to publish their path
    descriptor based sub-service APIs.
*/
class Whiteboard /* WB_NOTEST_FINAL */ : 
    protected IRoutingTableObserver,
    public IMessageHandler::IHandler
{
protected:
    /**
    * Initializes a new instance of the Whiteboard class. Inheritance is used only by unittest mockups.
    */
    Whiteboard();

    /**
    C++ destructor.
    */
    virtual ~Whiteboard();

public:
    /**
    * Get singleton instance.
    *
    * @return pointer to the Whiteboard singleton.
    */
    static Whiteboard* getInstance();

    /**
    * Get the buffer pool owned by the Whiteboard.
    * @return Reference to the BufferPool instance.
    */
    inline BufferPool& getBufferPool() { return *mpBufferPool; }

    /** Initializes whiteboard
    *
    * @param serialNumber Serial number to use
    * @param rConfiguration Configuration
    */
    void initialize(
        const char* serialNumber,
        const Config& rConfiguration);

    /**
    * Deinitializes internal services and then Whiteboard
    */
    void deinitialize();

    /**
    * Gets status of the Whiteboard
    *
    * @return Status flags
    */
    static StatusFlags probeStatus();

    /**
    *	Gets associated communication class instance
    *
    *	@return Whiteboard communication instance
    */
    inline WhiteboardCommunication& getCommunication();

    /**
    * @return execution context used by the whiteboard
    */
    inline ExecutionContextMap& getExecutionContextMap();

    /**
    * @return timer pool used by the whiteboard
    */
    inline TimerPool& getTimerPool();

    /**
    * @return stream pool used by the whiteboard
    */
    inline StreamPool& getStreamPool();

#ifdef WB_HAVE_PATH_PARAMETERS
    /**
    * @return path variable cache used by this whiteboard
    */
    inline PathParameterCache& getPathParameterCache();
#endif

    /**
    * @return Resource tree used by this whiteboard
    */
    inline internal::ResourceTree& getResourceTree();

    /**
    * @return Resource tree used by this whiteboard
    */
    inline const internal::ResourceTree& getResourceTree() const;

    /*********************
    * Resources
    *********************/

    /** Gets full path of a resource
    *
    * @param resourceId ID of resource which path should be formatted
    * @param path On output contains full path of the resource
    * @return Result of the operation
    */
    Result getFullResourcePath(LocalResourceId localResourceId, char path[WB_MAX_RESOURCE_PATH_LEN]);

    /**
    * Adds a new sub tree to the resource tree.
    *
    * @param parentResourceId Id of the parent resource (sub tree mount point)
    * @param rMetadataMap Metadata of the subtree that should be registered
    * @param hookInstaller Option. Function that install execution context hooks.
    * @return Result of the operation
    */
    Result registerResourceSubtree(
        ResourceId parentResourceId, 
        const MetadataMap& rMetadataMap,
        ResourceTree::HookInstallerFunc* hookInstaller = NULL);

    /**
    * Removes a resource subtree from the resource tree
    *
    * @param rMetadataMap Metadata of the subtree that should be unregistered
    * @return A value indicating whether the resource was successfully removed
    */
    Result unregisterResourceSubtree(const MetadataMap& rMetadataMap);

    /**
    * Returns next available API id for new resource subtree
    */
    metadata::ApiId getNextAvailableApiId();

    /*********************
    * Provider interface
    *********************/

    /**
    *	Allocates ID for resource provider and
    *	binds it to this whiteboard instance
    *
    *	@param rProvider Resource provider instance
    *	@return Result of the operation
    */
    Result bindProvider(ResourceProvider& rProvider);

    /**
    *	Unbinds the provider from this whiteboard instance
    *	and removes its ID.
    *	Note that the resource provider should have unregistered all
    *	its resources before this call.
    *
    *	@param rProvider The resource provider
    *	@return Result of the operation
    */
    Result unbindProvider(ResourceProvider& rProvider);

    /**
    *   Registers array of provider of resources
    *
    *   @note If any the resources fail to register, all already registered
    *   resources are unregister.
    *
    *   @param numberOfResources Number of resources to register
    *   @param pResourceIds Array of resources to register
    *	@param resourceProviderId ID of the resource provider
    *   @return Result of the operation.
    */
    Result registerProviderResources(
        size_t numberOfResources, const LocalResourceId* const pResourceIds, ProviderId resourceProviderId);

    /**
    *   Unregisters array of provider of resources
    *
    *   @note If any the resources fail to unregister, function will continue with next resource.
    *
    *   @param numberOfResources Number of resources to unregister
    *   @param pResourceIds Array of resources to unregister
    *	@param resourceProviderId ID of the resource provider
    *   @param removeSubscribes - OPTIONAL, default true, remove existing subscriptions to the resource.
    *   @return Result of the operation. In case of resource unregister fails, this function will
    *   return first error code of the operation.
    */
    Result unregisterProviderResources(
        size_t numberOfResources, const LocalResourceId* const pResourceIds, ProviderId resourceProviderId, bool removeSubscribes = true);

    /**
    *	Sends resource changed notification for all subscribed clients
    *
    *	@param providerId ID of the resource provider
    *	@param resourceId ID of the associated resource
    *   @param rResponseOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *	@param rValue Current value of the resource
    *	@param rParameters, where the notificationType Type is the first parameter (update, insert, delete)
    *	@return Result of the operation
    */
    Result updateResource(
        ProviderId providerId,
        ResourceId resourceId,
        const ResourceProvider_ResponseOptions& rResponseOptions,
        const Value& rValue,
        const ParameterList& rParameters);

    /**
    *	Returns result for a request
    *
    *	@param providerId ID of the resource provider
    *   @param rRequest Request information
    *	@param resultCode Result code of the request
    *   @param rResponseOptions Options for the response delivery - @see whiteboard::ResourceProvider::ResponseOptions
    *   @param rResultData Response result data
    *	@return Result of the operation
    */
    Result returnResult(
        ProviderId providerId,
        const Request& rRequest,
        Result resultCode,
        const ResourceProvider_ResponseOptions& rResponseOptions,
        const Value& rResultData);

    /*********************
    * For Client interface
    *********************/

    /**
    *	Allocates ID for resource client and
    *	binds it to this whiteboard instance
    *
    *	@param rProvider Resource client instance
    *	@return Result of the operation
    */
    Result bindClient(ResourceClient& rClient);

    /**
    *	Unbinds the client from this whiteboard instance
    *	and removes its ID.
    *	Note that the resource client should have unsubscribed from
    *	all resources before this call.
    *
    *	@param rClient The resource client
    *	@return Result of the operation
    */
    Result unbindClient(ResourceClient& rClient);

    /**
    *	Resolves resource path. Blocks calling thread until request
    *	result has been received.
    *
    *	@param pFullPath Path of the resource
    *	@param rResourceId On successful output contains the resource identifier
    *   @param clientId ID of the client requesting the resource.
    *	@param timeoutMs Request timeout in milliseconds (Timeout applies only to remote requests. If zero, request won't
    *   timeout).
    *	@return Result of the operation
    */
    Result getResource(const char* pFullPath, ResourceId& rResourceId, ClientId clientId, size_t timeoutMs);

    /**
    *	Resolves resource path from local resource trees. Function
    *	will not block, but will fail if resource is not in local resource
    *	trees.
    *
    *	@param pFullPath Path of the resource
    *	@param rResourceId On successful output contains the resource identifier
    *   @param clientId ID of the client requesting the resource.
    *	@return Result of the operation
    */
    Result getLocalResource(const char* pFullPath, ResourceId& rResourceId, ClientId clientId = ID_INVALID_CLIENT);

    /**
    *	Performs asynchronous resource resolving
    *
    *	@param pFullPath Path of the resource
    *	@param clientId ID of the client that will receive the callback.
    *	@param requestId ID of the client request
    *	@param timeoutMs Request timeout in milliseconds (Timeout applies only to remote requests. If zero, request won't
    *   timeout).
    *	@return Result code of the operation
    */
    Result asyncGetResource(const char* pFullPath, ClientId clientId, RequestId requestId, size_t timeoutMs);

    /**
    *	Performs resource release (i.e indicates to Provider's Whiteboard that this client will not
    *   use the given resource anymore). Blocks calling thread until request result has been received.
    *
    *   @param resourceId Id of the resource
    *   @param clientId ID of the client that will receive the callback.
    *   @param timeoutMs Request timeout in milliseconds(Timeout applies only to remote requests.If zero, request won't
    *    timeout).
    *   @return Result code of the operation
    */
    Result releaseResource(ResourceId resourceId, ClientId clientId, size_t timeoutMs);

    /**
    *	Performs asynchronous resource release (i.e indicates to Provider's Whiteboard that this client will not
    *   use the given resource anymore).
    *
    *	@param resourceId Id of the resource
    *	@param clientId ID of the client that will receive the callback.
    *	@param requestId ID of the client request
    *	@param timeoutMs Request timeout in milliseconds (Timeout applies only to remote requests. If zero, request won't
    *   timeout).
    *	@return Result code of the operation
    */
    Result asyncReleaseResource(ResourceId resourceId, ClientId clientId, RequestId requestId, size_t timeoutMs);

    /**
    *	Performs asynchronous request.
    *
    *   @param rRequest Request information
    *	@param rParameters List of request parameters
    *	@param rRequestOptions Request options, @see whiteboard::ResourceClient::AsyncRequestOptions
    *	@return Result code of the operation
    */
    Result asyncRequest(
        const Request& rRequest,
        const ParameterList& rParameters,
        const ResourceClient_AsyncRequestOptions& rOptions);

    /**
    *	Check if resource has any subscriptions.
    *
    *   @param resourceProviderId ID of the resource provider that is doing the checking
    *	@param resourceId ID of the resource that is checked
    *
    *	@return
    *		HTTP_CODE_OK if resource has been subscribed,
    *		HTTP_CODE_NO_CONTENT if resource is not subscribed,
    *		HTTP_CODE_NOT_FOUND indicating an error (provider has not registered the resource)
    */
    Result isResourceSubscribed(ProviderId resourceProviderId, ResourceId resourceId) const;

    /**
    *   Enumerate all the subscribers of the given resource. Calls the given functor for each
    *   subscription. This method is synchronous, so the caller can trust that all callbacks have been called
    *   when this call returns. 
    *
    *   @see whiteboard::ResourceProvider::enumerateSubscriptions
    *
    *   @param providerId ID of the provider that made the enumerate request
    *   @param resourceId ID of the resource that is to be enumrated
    *   @param rFunctor Functor to call for each subscription @see whiteboard::ResourceProvider::IEnumerateSubscriptionsFunctor
    *
    *   @return
    *     HTTP_CODE_OK if resource has been subscribed, and enumerated
    *     HTTP_CODE_NO_CONTENT if resource is not subscribed, enumerating skipped
    *     HTTP_CODE_NOT_FOUND indicating an error (provider has not registered the resource)
    */
    Result enumerateSubscriptions(ProviderId providerId, ResourceId resourceId, ResourceProvider::IEnumerateSubscriptionsFunctor& rFunctor) const;

    /**
    * Searches the execution context maps for a context with the given ID.
    * @param executionContextId ID of the context being sought after.
    * @return Pointer to the execution context, NULL if not found (invalid ID).
    */
    LocalExecutionContext* tryToFindExecutionContextById(ExecutionContextId executionContextId) const;
    
    /*
    * Gets execution context with given ID
    *
    * @param executionContextId ID of the execeution context
    * @return Execution context with given ID
    */
    LocalExecutionContext& getExecutionContextById(ExecutionContextId executionContextId) const;

    /**
    *   Gets the execution context ID of the currently running context.
    *   Note, avoid using this method if the context info is available via base class (e.g. ResourceProvider).
    *   @return ID of the execution context, ID_INVALID_EXECUTION_CONTEXT if called from non-WB thread.
    */
    ExecutionContextId getCurrentContextId();
    
WB_PUBLIC_IN_UNITTESTS(private):

    /** Singleton class may need copy constructor for build time checks */
    /** Prevent usage of default C++ copy constructor */
    Whiteboard(Whiteboard&) DELETED;

    /**
    * These functions are called by LocalExecutionContext
    */
    friend class LocalExecutionContext;

    /** The timer pool notifies timer messages to whiteboard */
    friend class TimerPool;

    /**
    *	Returns result to client for GET RESOURCE or RELEASE RESOURCE requests
    *
    *	@param rRequest Request information
    *	@param resultCode Result code of the request
    *	@return Result of the operation
    */
    Result returnResult(const Request& rRequest, Result resultCode);

    /*********************
    * For Resource interface
    *********************/

    /**
    *	Handles GET RESOURCE request in caller's execution context.
    *
    *   @param rRequest Request information
    *	@param pFullPath Path of the resource to query
    */
    void handleResourceGetRequest(const Request& rRequest, const char* pFullPath) OVERRIDE FINAL;

    /**
    *	Handles RELEASE RESOURCE request in caller's execution context.
    *
    *   @param rRequest Request information
    */
    void handleResourceReleaseRequest(const Request& rRequest) OVERRIDE FINAL;

    /*********************
    * For Provider interface
    *********************/

    /**
    *	Handles request in provider's execution context.
    *
    *   @param rRequest Request information
    *	@param rParameters List of parameters for the request
    */
    void handleProviderRequest(
        const Request& rRequest, const ParameterList& rParameters) OVERRIDE FINAL;

    /**
    *	Handles SUBSCRIBE request in provider's execution context.
    *
    *	@param rRequest Request information
    *	@param rParameters List of path variables and parameters for the request
    */
    void handleProviderSubscribeRequest(
        const Request& rRequest,
        const ParameterList& rPathParametersAndParameters);

    /**
    *	Handles UNSUBSCRIBE request in provider's execution context.
    *
    *	@param rRequest Request information
    *	@param rParameters List of psth variables and parameters for the request
    */
    void handleProviderUnsubscribeRequest(
        const Request& rRequest,
        const ParameterList& rPathParametersAndParameters);

    /**
    *   Sends results of GET, PUT, SUBSCRIBE, UNSUBSCRIBE, POST and DELETE request to client's execution context
    *   This function performs no safety checking. All checks should be done prior calling this function.
    *
    *	@param rRequest Request information
    *	@param resultCode Result code of the request
    *	@param rResult Result data of the request
    *   @param forceAsync A value indicating whether dispatch should always be asychronous
    *	@return Result of the operation
    */
    Result dispatchClientResult(
        const Request& rRequest,
        Result resultCode,
        const Value& rResult,
        bool forceAsync);

    /*********************
    * For Client interface
    *********************/

    /**
    *	Callback for notification messages
    *
    *	@param clientId ID of the client that should receive this message
    *	@param resourceId ID of the associated resource
    *	@param rValue Current value of the resource
    *	@param rParameters The notificationType is the first parameter (update, insert, delete)
    */
    void handleClientOnNotify(ClientId clientId, ResourceId resourceId, const Value& rValue, const ParameterList& rParameters) OVERRIDE FINAL;

    /**
    *	Callback for asynchronous requests
    *
    *   @param rRequest Request information
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    void handleClientResult(
        const Request& rRequest, Result resultCode, const Value& rResultData) OVERRIDE FINAL;

    /**
    *   Callback for asynchronous resource unavailability notification.
    *
    *   @oaram clientId ID of the client to notify
    *   @param resourceId ID of the resource that is unavailable
    */
    void handleClientOnLocalResourceUnavailable(LocalClientId clientId, ResourceId resourceId);

    /**
    *	Sends GET, PUT, SUBSCRIBE, UNSUBSCRIBE, POST and DELETE request to provider's execution context.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *   @param rOptions Request options, @see whiteboard::ResourceClient::AsyncRequestOptions
    *	@return Result of the operation
    */
    Result dispatchProviderRequest(
        const Request& rRequest,
        const ParameterList& rParameters,
        const ResourceClient_AsyncRequestOptions& rOptions);

    /***************************
    * For RoutingTableObserver
    ***************************/

    /**
    *   Callback for routing table changes.
    *
    *   @note This callback can be called (not simultaneously) from multiple threads.
    *
    *   @param rRouteEntry Routing table entry
    *   @param type Type of the notification.
    *   @param previousState Previous state of the routing table entry
    */
    void onRoutingTableChange(
        const RoutingTableEntry& rRouteEntry, RoutingTableNotification type, WB_RES::RouteState previousState) OVERRIDE FINAL;

    /*******************************************************************
    * Remote whiteboard disconnection / local client & provider removal
    ********************************************************************/

    /** A structure to guide tell what type of subscription removal operation to perform */
    class RemoveSubscriptionsParams FINAL
    {
    public:
        enum Type
        {
            WHITEBOARD_DISCONNECT, //< Remote whiteboard lost, remove all subscriptions from that wb
            CLIENT_LOST,           //< Local client lost, remove subscriptions from that client
        };

        const ExecutionContextId executionContextId;
        const Type type;
        const union Ids
        {
            Ids(WhiteboardId id) : whiteboardId(id) {}
            Ids(uint16 id) : clientId(id) {}

            WhiteboardId whiteboardId;
            LocalClientId clientId;
        } id;

        inline RemoveSubscriptionsParams(
            ExecutionContextId executionContextId_, Type type_, WhiteboardId whiteboardId_) :
            executionContextId(executionContextId_),
            type(type_),
            id(whiteboardId_ )
        {
        }

        inline RemoveSubscriptionsParams(
            ExecutionContextId executionContextId_, Type type_, uint16 clientId_) :
            executionContextId(executionContextId_),
            type(type_),
            id(clientId_)
        {
        }
    private:
        RemoveSubscriptionsParams() DELETED;
        RemoveSubscriptionsParams(const RemoveSubscriptionsParams& rOrig) DELETED;
        RemoveSubscriptionsParams& operator=(const RemoveSubscriptionsParams& rRhs) DELETED;
    };

    /** 
    Removes subscriptions related to given execution context. 
    
    SHALL BE CALLED in the execution context given as a parameter.
    REQUIRES EXCLUSIVE ACCESS, WB MUTEX SHALL BE LOCKED DURING THIS CALL

    @param executionContextId [in] executionContext where this message is being called
    @param clientId [in] Client of which subscriptions to remove.
    @param all [in] Remove all subscriptions of all clients from execution context in the clientId
    */
    void removeSubscriptions(const RemoveSubscriptionsParams& params);

    /**
    Handler for remote whiteboard disconnection status notifications; 
    Informs all clients and providers about the connection status change of the remote whiteboard.
    Also cleans up subscriptions to that whiteboard.

    SHALL BE CALLED in the execution context given as a parameter.

    @param executionContextId [in] executionContext where this message is being called
    @param whiteboardId [in] Local Id of the remote whiteboard that has been disconnected.
    */
    void handleRemoteWhiteboardDisconnectedNotification(
        ExecutionContextId executionContextId, 
        WhiteboardId whiteboardId);

    /**
    Handler for local client removal; Removes subscriptions of the given local client.

    SHALL BE CALLED in the execution context given as a parameter.

    @param executionContextId [in] executionContext where this message is being called
    @param clientId [in] Client of which subscriptions to remove.
    */
    void handleLocalClientRemove(ExecutionContextId executionContextId, LocalClientId clientId);

    /***************************
    * For timers
    ***************************/
    /**
    This is called by the whiteboard::Timer from OS SW timer thread's time, when provider timer expires.

    @param executionContextId ID of the execution context, where to dispatch this timer message
    @param providerId ID of the provider who should receive this timer message
    @param timerId ID of the timer that expired

    @see whiteboard::ResourceProvider::startTimer
    @see whiteboard::Timer
    */
    void onProviderOsTimerMessage(ExecutionContextId executionContextId, LocalProviderId providerId, TimerId timerId);

    /**
    This is called by the whiteboard::Timer from OS SW timer thread's time, when client timer expires.

    @param executionContextId ID of the execution context, where to dispatch this timer message
    @param providerId ID of the provider who should receive this timer message
    @param timerId ID of the timer that expired

    @see whiteboard::ResourceProvider::startTimer
    @see whiteboard::Timer
    */
    void onClientOsTimerMessage(ExecutionContextId executionContextId, LocalClientId clientId, TimerId timerId);

    /**
    This message called by the whiteboard::LocalExecutionContext, when timer message
    has been dispatched to providers execution context.

    @param providerId ID of the provider who should receive this timer message
    @param timerId ID of the timer that expired

    @see whiteboard::Whiteboard::onProviderOsTimerMessage
    */
    void handleProviderTimerMessage(LocalProviderId providerId, TimerId timerId);

    /**
    This message called by the whiteboard::LocalExecutionContext, when timer message
    has been dispatched to client's execution context.

    @param clientId ID of the client who should receive this timer message
    @param timerId ID of the timer that expired

    @see whiteboard::Whiteboard::onClientOsTimerMessage
    */
    void handleClientTimerMessage(LocalClientId clientId, TimerId timerId);

private:
    /** WhiteboardCommunication and DPC classes access following functions */
    friend class WhiteboardCommunication;
    friend class Dpc;
    friend class DpcFunctor;
    friend class IExecutionContext;
    friend class SuuntoSerial;
    friend class services::BypassProvider;
    friend class services::MetadataProvider;
    friend class services::PoolStatsProvider;
    friend class services::SubscriptionsInfoProvider;
    friend class WhiteboardMockup;
#if WB_UNITTEST_BUILD
    friend class ResourceClient;
#endif

    /**
    * Gets execution context with given local client ID
    *
    * @param localClientId ID of the local client
    * @return Execution context of local client with given ID
    */
    LocalExecutionContext& getExecutionContextByLocalClientId(LocalClientId localClientId) const;

    /**
    * Gets resource metadata with given ID
    *
    * @param localResourceId ID of the local resource
    * @param rMetadata [out] On output contains resource metadata
    * @return A value indicating whether the reource was found
    */
    bool getResourceMetadataById(
        LocalResourceId localResourceId, ResourceMetadata& rMetadata);

    /**
    * Gets resource provider ID of given resource
    *
    * @param resourceId ID that specifies the resource which provider should be returned
    * @return ID of the provider or ID_INVALID_PROVIDER if resource or provider is not found
    */
    ProviderId getResourceProviderIdByResourceId(ResourceId resourceId) const;

    /**
    * Bypasses provider of given resource
    *
    * @param resourceId ID of the resource
    * @param newProviderId ID of the new provider
    * @param rOldProviderId [out] On output contains ID of the old provider
    * @return Result of the operation
    */
    Result bypassProvider(
        ResourceId resourceId, LocalProviderId newProviderId, LocalProviderId& rOldProviderId);

    /**
    * Gets local resource client with given ID
    *
    * @param clientId ID of the client
    * @return Local resource client or NULL if client with given ID is not registered
    */
    ResourceClient* getLocalClientById(ClientId clientId) const;

    /**
    * Gets local resource client with given ID
    *
    * @param localClientId ID of the local client
    * @return Local resource client or NULL if client with given ID is not registered
    */
    ResourceClient* getLocalClientById(LocalClientId localClientId) const;

    /**
    * Ask the Whiteboard instance for a RequestClient instance with the given name string.
    * @param clientName Pointer to the name string.
    * @return Pointer to the instance, NULL if not found
    */
    ResourceClient* getLocalClientByName(const char* clientName) const;

    /**
    * Gets local resource provider with given ID
    *
    * @param providerId ID of the provider
    * @return Local resource provider or NULL if provider with given ID is not registered
    */
    ResourceProvider* getLocalProviderById(ProviderId providerId) const;
    ResourceProvider* getLocalProviderById(LocalProviderId providerId) const;

    /**
    * Gets next request ID
    * @param pUserRequestId copies the new resource ID to user as well, if pUserRequestId is not null.
    * @return Request ID for a new request
    */
    inline RequestId getNextRequestId(RequestId* pUserRequestId);

    /**
    * Checks current execution context
    *
    * @param executionContextId ID of the expected execution context
    */
    inline void checkExecutionContext(ExecutionContextId executionContextId) const;

    /**
    * Checks that provider has registered the resource (this also ensures that resource and
    * provider are in same context)
    *
    * @param resourceProviderId ID of the resource provider
    * @param rSubscriptionData The resource subscription information
    * @return Result of the operation. HTTP_CODE_OK if everything is good.
    */
    inline Result checkProviderRegistration(
        ProviderId resourceProviderId, const ResourceSubscriptionData& rSubscriptionData) const;

    /**
    * Checks the resource+operation security mask against the client execution context security mask
    *
    * @param clientId ID of the client
    * @param rResourceMetadata Resource metadata
    * @param rSubscriptionData The resource subscription information
    * @param requestType Type of the request to be done for the resource
    *
    * @return HTTP_CODE_OK if succeeds, else HTTP_CODE_UNAUTHORIZED
    */
    inline bool checkSecurityMask(
        ClientId clientId,
        const ResourceMetadata& rResourceMetadata,
        const ResourceSubscriptionData& rSubscriptionData,
        RequestType requestType) const;

    /**
    * Helper to check if a given resource is being bypassed by the bypassprovider for testing purposes.
    *
    * @param rResource [in] Resource to check 
    * @return true if bypass provider has bypassed the actual resource, else false
    */
    bool isBypassedResource(const ResourceSubscriptionData& rSubscriptionData) const;

    /**
    *   Gets the resource object of the givenId, performs context check and provider resource registration check first.
    *
    * @param resourceProviderId ID of the resource provider that is doing the checking
    * @param resourceId ID of the resource that is checked
    * @param rMetadata On output contains resource metadata
    * @param rSubscriptionData On output contains resource's subscription information
    *
    *	@return
    *		HTTP_CODE_OK if resource has been subscribed,
    *		HTTP_CODE_NO_CONTENT if resource is not subscribed,
    *		HTTP_CODE_NOT_FOUND indicating an error (provider has not registered the resource)
    */
    Result getResourceCheckProviderRegAndContext(
        ProviderId resourceProviderId,
        ResourceId resourceId,
        ResourceMetadata& rMetadata,
        ResourceSubscriptionData& rSubscriptionData) const;

protected:
    /// Serial number of whiteboard
    SuuntoSerial mSerial;

    /// Handle to mutex that protects whiteboard's internal resources.
    WbRecursiveMutexHandle mMutex;

    /// Whiteboard communication instance
    WhiteboardCommunication* mpCommunication;

    /// ExecutionContextMap instance
    ExecutionContextMap* mpExecutionContextMap;

    /// Timer pool
    TimerPool* mpTimerPool;

    /// Buffer pool
    BufferPool* mpBufferPool;

    /** Internal pool for ongoing PUT stream requests */
    StreamPool* mpStreamPool;

    /// Resource tree instance
    internal::ResourceTree mResourceTree;

    /// Registry for binded providers
    ProviderRegistry* mpProviderRegistry;

    /// Registry for binded clients
    ClientRegistry* mpClientRegistry;

    // Subscription registry
    SubscriptionRegistry* mpSubscriptionRegistry;

#ifdef WB_HAVE_PATH_PARAMETERS
    /// Cache for path variables of the whiteboard clients
    PathParameterCache* mpPathParameterCache;
#endif

    /// Request handlers
    typedef void (ResourceProvider::*RequestHandler)(const Request&, const ParameterList&);
    static const RequestHandler RequestHandlers[4];

    /// Result handlers
    typedef void (ResourceClient::*ResultHandler)(RequestId, ResourceId, Result);
    static const ResultHandler ResultHandlers[2];

    typedef void (ResourceClient::*ResultWithValueHandler)(RequestId, ResourceId, Result, const Value&);
    static const ResultWithValueHandler ResultWithValueHandlers[6];
};

inline WhiteboardCommunication& Whiteboard::getCommunication()
{
    WB_DEBUG_ASSERT(mpCommunication != NULL);
    return *mpCommunication;
}

inline ExecutionContextMap& Whiteboard::getExecutionContextMap()
{
    WB_DEBUG_ASSERT(mpExecutionContextMap != NULL);
    return *mpExecutionContextMap;
}

inline TimerPool& Whiteboard::getTimerPool()
{
    WB_DEBUG_ASSERT(mpTimerPool != NULL);
    return *mpTimerPool;
}

inline StreamPool& Whiteboard::getStreamPool()
{
    WB_DEBUG_ASSERT(mpStreamPool != NULL);
    return *mpStreamPool;
}

#ifdef WB_HAVE_PATH_PARAMETERS
inline PathParameterCache& Whiteboard::getPathParameterCache()
{
    WB_DEBUG_ASSERT(mpPathParameterCache != NULL);
    return *mpPathParameterCache;
}
#endif

inline internal::ResourceTree& Whiteboard::getResourceTree()
{
    return mResourceTree;
}

inline const internal::ResourceTree& Whiteboard::getResourceTree() const
{
    return mResourceTree;
}

} // namespace whiteboard
