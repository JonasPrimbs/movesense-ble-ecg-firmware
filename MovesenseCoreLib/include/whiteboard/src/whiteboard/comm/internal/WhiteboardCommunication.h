#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/containers/BlockingFreeListPoolAllocator.h"
#include "whiteboard/containers/TypedPool.h"
#include "whiteboard/internal/EventProcessor.h"
#include "whiteboard/internal/IExecutionContext.h"
#include "whiteboard/internal/Status.h"
#include "whiteboard/comm/internal/RoutingTable.h"
#include "whiteboard/internal/protocol/IMessageDispatcher.h"
#include "whiteboard/metadata/MetadataStructures.h"

namespace whiteboard
{

// Forward declarations
struct Buffer;
class CommAdapter;
struct RoutingTableEntry;
class Whiteboard;

/* Reconnect attempt timeout */
static const size_t RECONNECT_TIMEOUT_MS = 60 * 1000;

/* Maximum message sending interval */
static const size_t KEEP_ALIVE_SEND_INTERVAL_MS = 15 * 1000;

/* Keep alive timeout to disconnect
   More than multiple of KEEP_ALIVE_SEND_INTERVAL to allow little tolerance for slow connections / non-rt remote wb
 */
static const size_t KEEP_ALIVE_TIMEOUT_MS = 32 * 1000; 

/** Whiteboard communication handler */
class WhiteboardCommunication FINAL : public IExecutionContext
{
public:
    /**
    * Constructor
    *
    * @param rWhiteboard Whiteboard instance
    * @param rSerialNumber Serial number
    * @param rConfiguration Configuration
    */
    WhiteboardCommunication(Whiteboard& rWhiteboard, SuuntoSerial& rSerialNumber, const Config& rConfiguration);

    /**
    * Destructor.
    */
    virtual ~WhiteboardCommunication();

    /**
    * Gets associated Whiteboard instance
    *
    * @return Whiteboard instance
    */
    inline Whiteboard& getWhiteboard();

    /**
    * Gets status of the WhiteboardCommunication instance
    *
    * @return Status flags
    */
    StatusFlags probeStatus();

    /**
    *	Gets name of the execution context
    *
    *	@return Name of the execution context
    */
    virtual const char* getName() const OVERRIDE { return "WB_COMM"; }

    /**
    *	Gets ID of the execution context
    *
    *	@return ID of the execution context
    */
    virtual ExecutionContextId getId() const OVERRIDE { return ID_INVALID_EXECUTION_CONTEXT; }

    /**
    * Gets the used routing table.
    *
    * @return RoutingTable instance
    */
    inline RoutingTable& getRoutingTable();

#if WB_HAVE_NETWORK_INFO
    /**
    *   Gets name of communication adapter with given index.
    *
    *   @param index Index of the communication adapter.
    *   @param bufferSize Maximum number of charactes in the buffer including zero terminator
    *   @param nameBuffer Buffer where name should be written
    *   @return Result of the operation.
    */
    Result getAdapterName(size_t index, size_t bufferSize, char* nameBuffer);

    /**
    *   Gets state of communication adapter with given index.
    *
    *   @param index Index of the communication adapter.
    *   @param rAdapterState On output contains state of the adapter
    *   @return Result of the operation.
    */
    Result probeAdapterState(size_t index, WB_RES::AdapterState& rAdapterState);
#endif

private:
    /** Whiteboard accesses these functions directly */
    friend class Whiteboard;

    /**
    *	Performs asynchronous resource resolving
    *
    *	@param requestId ID of the request
    *	@param pFullPath Path of the resource
    *	@param clientId ID of the client that will receive the callback.
    *	@param timeoutMs Request timeout in milliseconds (Timeout applies only to remote requests.
    *                    If zero, request won't timeout).
    *	@return Result code of the operation and ID of the request
    */
    Result asyncGetResource(RequestId requestId, const char* pFullPath, ClientId clientId, size_t timeoutMs);

    /**
    *	Sends GET RESOURCE request to destination
    *
    *	@param destinationWhiteboardId ID of destination whiteboard
    *	@param requestId ID of the request
    *	@param pFullPath Path of the resource
    *	@param clientId ID of the client that will receive the callback.
    *	@param timeoutMs Request timeout in milliseconds (Timeout applies only to remote requests.
    *                    If zero, request won't timeout).
    *	@return Result code of the operation and ID of the request
    */
    Result dispatchResourceGetRequest(
        WhiteboardId destinationWhiteboardId, RequestId requestId, const char* pFullPath, ClientId clientId, size_t timeoutMs);

    /**
    *	Sends RELEASE RESOURCE request to destination
    *
    *	@param requestId ID of the request
    *	@param resourceId Id of the resource
    *	@param clientId ID of the client that will receive the callback.
    *	@param timeoutMs Request timeout in milliseconds (Timeout applies only to remote requests. If zero, request won't
    *timeout).
    *	@return Result code of the operation and ID of the request
    */
    Result dispatchResourceReleaseRequest(RequestId requestId, ResourceId resourceId, ClientId clientId, size_t timeoutMs);

    /**
    *	Sends resource changed notification for to client's execution context
    *
    *	@param clientId ID of the client that should receive this message
    *	@param resourceId ID of the associated resource
    *	@param rValue Current value of the resource
    *	@param rParameters Parameters, where the notificationType (update, insert, delete) is the first parameter
    *	@return Result of the operation
    */
    Result dispatchClientOnNotify(
        ClientId clientId, ResourceId resourceId, const Value& rValue, const ParameterList& rParameters);

    /**
    *	Sends results of GET RESOURCE request to client's execution context
    *
    *	@param requestId ID of the request
    *	@param resultCode Result code of the request
    *	@param clientId ID of the client that should receive this message
    *	@param resourceId ID of the associated resource
    *	@return Result of the operation
    */
    Result dispatchClientOnGetResourceResult(const Request& rRequest, Result resultCode);

    /**
    *	Sends results of RELEASE RESOURCE request to client's execution context
    *
    *	@param requestId ID of the request
    *   @param resourceId ID of the resource
    *	@param resultCode Result code of the request
    *	@param clientId ID of the client that should receive this message
    *	@return Result of the operation
    */
    Result dispatchClientOnReleaseResourceResult(const Request& rRequest, Result resultCode);

    /**
    * Sends request results to client's execution context
    *
    * @param rRequest The request
    * @param resultCode Result code of the request
    * @param rResultData Result of the request
    */
    Result dispatchClientResult(
        const Request& rRequest,
        Result resultCode,
        const Value& rResultData);

    /** Prototype for client dispatch functions */
    typedef Result(IMessageDispatcher::*DispatchClientResultFunc)(
        const Request&,
        Result,
        const Value&,
        IMessageDispatcher::IDispatcher&,
        const IMessageDispatcher::RoutingInfo*,
        bool) const;

    /**
    * Sends request results to client's execution context
    *
    * @param requestId ID of the request
    * @param resultCode Result code of the request
    * @param clientId ID of the client that should receive this message
    * @param resourceId ID of the resource
    * @param rResultData Result of the request
    * @param dispatchFunc Dispatch function to use
    * @return Result of the operation
    */
    Result dispatchClientResultFunc(
        RequestId requestId,
        Result resultCode,
        ClientId clientId,
        ResourceId resourceId,
        const Value& rResultData,
        DispatchClientResultFunc dispatchFunc);

    /**
    *	Sends request to provider's execution context.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *	@param timeoutMs Request timeout in milliseconds (Timeout applies only to remote requests. If zero,
    *                    request won't timeout).
    *	@return Result of the operation
    */
    Result dispatchProviderRequest(
        const Request& rRequest, const ParameterList& rParameters, size_t timeoutMs);

private:
    /** Dispatch helper */
    class DispatchHelper;
    friend class DispatchHelper;

    /** Prepares routing of a new message
    *
    * @param destinationWhiteboardId ID of destination whiteboard
    * @param rRoutingInfo On output contains routing information destination
    * @param rNextHopHandle On output contains handle to next hop on route to destination
    * @param rpBufferAllocator On output contains the buffer allocator to use
    * @param rpMessageDispatcher On output contains the message dispatcher that should be used with destination
    * @return Result of the operation
    */
    Result prepareMessageRouting(
        WhiteboardId destinationWhiteboardId,
        IMessageDispatcher::RoutingInfo& rRoutingInfo,
        RoutingTableEntryHandle& rNextHopHandle,
        IBufferAllocator*& rpBufferAllocator,
        const IMessageDispatcher*& rpMessageDispatcher);

    /** Dispatches a data message to the specified communication interface
    *
    * @param rNextHopHandle Handle to next hop on route to message destination
    * @param pBuffer Buffer that contains the message that should be dispatched
    * @param timeoutMs Request timeout in milliseconds (Timeout applies only to remote requests. If zero, request won't timeout).
    * @param resourceId ID of the associated resource
    * @param clientId ID of the associated resource
    * @return Return value of the operation
    */
    Result dispatch(
        const RoutingTableEntryHandle& rNextHopHandle,
        Buffer* pBuffer,
        size_t timeoutMs,
        ResourceId resourceId,
        ClientId clientId);

private:
    /** CommAdapter calls these functions */
    friend class CommAdapter;
    friend class CommAdapterMockup;

    /** Registers a communication adapter
     *
     * @param rAdapter Adapter that should be registered
     */
    void registerAdapter(CommAdapter& rAdapter);

    /** Unregisters a communication adapter
    *
    * @param rAdapter Adapter which registraton should be removed
    */
    void unregisterAdapter(CommAdapter& rAdapter);

    /** Adds new route to the routing table
    *
    * @see RoutingTable::addRoute
    */
    RoutingTableEntryHandle addRoute(
        const SuuntoSerial& rSerialNumber,
        CommAdapter* pInterface,
        const Address& rAddress,
        WbTimestamp lifeTime,
        bool buddy);

    /** Adds new or updates existing route
    *
    * @see RoutingTable::addRoute
    */
    RoutingTableEntryHandle addRoute(
        const SuuntoSerial& rSerialNumber,
        const SuuntoSerial& rNextHopSerialNumber,
        HopCount numberOfHops,
        SequenceNumber sequenceNumber,
        WbTimestamp lifeTime);

    /** Adds new or updates existing route
    *
    * @see RoutingTable::addRoute
    */
    RoutingTableEntryHandle addRoute(
        const SuuntoSerial& rSerialNumber,
        CommAdapter* pInterface,
        const Address& rAddress,
        const RoutingTableEntryHandle& rNextHop,
        HopCount numberOfHops,
        SequenceNumber sequenceNumber,
        WbTimestamp lifeTime);

    /** Removes a route from the routing table
    *
    * @see RoutingTable::addRoute
    */
    Result removeRoute(const RoutingTableEntryHandle& rHandle, const CommAdapter& rAdapter);

    /** Removes a route from the routing table
    *
    * @see RoutingTable::addRoute
    */
    Result removeRoute(const Address& rAddress, const CommAdapter& rAdapter);

    /** Removes all routes from the routing table
    *
    * @param rAdapter Adapter which routes should all be removed
    */
    void removeRoutes(const CommAdapter& rAdapter);

    /** Enables routes associated with given communication adapter that are in disabled state
    *
    * @param rAdapter Adapter which routes should be enabled
    * @return Result of the operation
    */
    Result enableRoutes(const CommAdapter& rAdapter);

    /** Disables all routes associated with given communication adapter
    *
    * @param rAdapter Adapter which routes should be disabled
    * @param gracefully A value indicating whether the routes should be closed gracefully i.e.
    *                   whether communication interface's disconnect should be called
    * @return Result of the operation
    */
    Result disableRoutes(const CommAdapter& rAdapter, bool gracefully);

    /** Called when adapter has completed Whiteboard initiated CommAdapter::connect asynchronously
    *
    * @param pAdapter Adapter that received the message
    * @param rDestination Connect destination
    * @param resultCode Result code of the operation
    */
    void onConnectCompleted(CommAdapter* pAdapter, const Address& rDestination, Result resultCode);

    /** Called when adapter has completed Whiteboard initiated CommAdapter::disconnect asynchronously
    *
    * @param pAdapter Adapter that received the message
    * @param rDestination Connect destination
    * @param resultCode Result code of the operation
    */
    void onDisconnectCompleted(CommAdapter* pAdapter, const Address& rDestination, Result resultCode);

    /** Called when adapter has received data
    *
    * @param pAdapter Adapter that received the message
    * @param rSource Adapter specific address of message sender
    * @param pBuffer Buffer that contains the message
    * @return Result of the operation
    */
    Result onDataReceived(CommAdapter* pAdapter, const Address& rSource, Buffer* pBuffer);

    /** Called when adapter has completed asynchronous send operation.
    *
    * @note Do not call this if send completed synchronously
    *
    * @param pBuffer Pointer to buffer that contained the sent message
    * @param resultCode Result of send operation
    */
    void onSendCompleted(Buffer* pBuffer, Result resultCode);

private:
    /** Routes a message from the communication adapter to destination
    *
    * @param pSourceAdapter Adapter that received the message
    * @param rSource Adapter specific address of message sender
    * @param pBuffer Buffer that contains the message
    * @return Result of the operation
    */
    Result routeMessage(CommAdapter* pSourceAdapter, const Address& rSource, Buffer* pBuffer);

    /** Handles a message from the communication adapter.
    *
    * @param pSourceAdapter Adapter that received the message
    * @param rSource Adapter specific address of message sender
    * @param pBuffer Buffer that contains the message
    * @param rMessageHeader The message header
    * @return Result of the operation
    */
    Result handleMessage(
        CommAdapter* pSourceAdapter, const Address& rSource, Buffer* pBuffer, const DataMessageHeader& rMessageHeader);

    /** Checks that message is from a valid origin and handles hello messages. Messages are 
    * valid if the connection is esablished or we are about doing so with a hello message.
    *
    * @param rOrigin Message origin. Updated to contain valid handle, if invalid handle is given.
    * @param rOriginSerial Serial number of the origin if known
    * @param pInterface Interface used with sender when message is not routed
    * @param rAddress Address or the sender when message is not routed
    * @param rSender Sender (or last router) of the message
    * @param numberOfHops Number of to the origin
    * @param rMessageHeader Message header
    * @param rResultCode On output contains result code of the operation
    * @return A value indicating whether the message was handled (successfully or in failure) and
    *        other message handling should not be continued
    */
    bool checkOriginAndHandleHello(
        RoutingTableEntryHandle& rOrigin,
        const SuuntoSerial& rOriginSerial,
        CommAdapter* pInterface,
        const Address& rAddress,
        const RoutingTableEntryHandle& rSender,
        HopCount numberOfHops,
        const DataMessageHeader& rMessageHeader,
        Result& rResultCode);

    /** Handles HELLO and HELLO ACK messages in communication thread
    *
    * @param isAcknowledgement A value indicating whether this is acknowledgement message
    * @param rMessage Buffer that contains the message
    * @param rSource Routing table entry associated with the communication source
    * @return Result of the operation
    */
    Result handleHelloMessage(bool isAcknowledgement, const Hello& rMessage, RoutingTableEntryHandle& rSource);

private:
    /** Handles a message from the communication adapter
    *
    * @param remoteWhiteboardIdInLocalScope ID of remote whiteboard that sent the message
    * @param protocolVersion Protocol version of remote whiteboard
    * @param pBuffer Buffer that contains the message
    * @param rMessageHeader The message header
    * @return Result of the operation
    */
    Result handleDataMessage(
        WhiteboardId remoteWhiteboardIdInLocalScope,
        ProtocolVersion protocolVersion,
        Buffer* pBuffer,
        const DataMessageHeader& rMessageHeader);

    /** Dispatches the message to execution context for real handling
    *
    * @param remoteWhiteboardIdInLocalScope ID of remote whiteboard that sent the message
    * @param protocolVersion Protocol version of remote whiteboard
    * @param pBuffer Buffer that contains the message
    * @param rMessageHeader The message header
    * @return Result of the operation
    */
    Result dispatchMessageToExecutionContext(
        WhiteboardId remoteWhiteboardIdInLocalScope,
        ProtocolVersion protocolVersion,
        Buffer* pBuffer,
        const DataMessageHeader& rMessageHeader,
        ExecutionContextId targetExecutionContextId);

protected:
    /**
    * Handles given message (from IExecutionContext).
    *
    * @param rEvent Event that should be handled
    */
    virtual void handle(const WbEvent& rEvent) OVERRIDE;

    /**
    * Optional processing of states between events(from IExecutionContext).
    *
    * @param eventsPending - not used
    *
    * @param eventsPending A value indicating whether more events are waiting in the queue
    * @return Options that guide further event prosessing.
    */
    virtual ExecutionContext::EventProcessorOptions eventStateProcess(bool eventsPending) OVERRIDE;

private:
    /** Wakes the state machine processor from its current sleep */
    void wakeStateMachine();

    /**
    * Processes route states and timeouts
    *
    * @param now Current timestamp
    * @return Number of milliseconds that can be slept, before route states must be checked again.
    */
    uint32 processRouteStates(WbTimestamp now);

    /**
    * Processes send request states and timeouts
    *
    * @param now Current timestamp
    * @return Number of milliseconds that can be slept, before send request states must be checked again.
    */
    uint32 processSendRequestStates(WbTimestamp now);

private:
    /**
    * Handles errors in route setup & tear down 
    *
    * @param rEntry Routing table entry of the route
    * @param errorCode Error code to log
    * @param message Textual error message to log
    */
    void handleRouteError(RoutingTableEntry& rEntry, Result errorCode, const char* message);

private:
    // Forward declarations
    struct SendRequest;

    /** Handles send requests in communication thread
     *
     * @param pRequest Request that should be processed
     */
    void handleSendEvent(SendRequest* pRequest);

    /** Sends the message described by given send request
    *
    * @param pRequest Request that should be processed
    * @return Result of the operation
    */
    Result send(SendRequest* pRequest);

    /** Handles send request completition in communication thread
    *
    * @param pBuffer Message that has been sent
    * @param resultCode Result code of the send operation
    */
    void handleSendCompletedEvent(Buffer* pBuffer, Result resultCode);

    /** Handles response received event in communication thread
    *
    * @param pBuffer Message that has been received
    * @param remoteWhiteboardIdInLocalScope ID of remote whiteboard in local scope
    * @param protocolVersion Protocol version of remote whiteboard
    */
    void handleResponseReceivedEvent(
        Buffer* pBuffer, WhiteboardId remoteWhiteboardIdInLocalScope, ProtocolVersion protocolVersion);

    /** Handles contextless request received event in communication thread
    *
    * @param pBuffer Message that has been received
    * @param remoteWhiteboardIdInLocalScope ID of remote whiteboard in local scope
    * @param protocolVersion Protocol version of remote whiteboard
    */
    void handleContextlessRequestReceivedEvent(
        Buffer* pBuffer, WhiteboardId remoteWhiteboardIdInLocalScope, ProtocolVersion protocolVersion);

    /** Returns error to sender of the data message
    *
    * @param pRequest Send request that has failed
    * @param result Result code of the operation	 */
    void returnError(const SendRequest* pRequest, Result resultCode);

    /** Prepares message buffer for sending
     *
     * @param rDestination Message destination
     * @param messageType Type of the message
     * @param payloadLength Length of the message payload
     * @param requestId Associated request ID
     * @param rDataOffset On output contains offset for the payload data
     * @param rpNextHop On output contains pointer to routing table entry of next hop
     * @return Pointer to allocated buffer
     */
    Buffer* prepareSend(
        RoutingTableEntry& rDestination,
        MessageType messageType,
        size_t payloadLength,
        RequestId requestId,
        size_t& rDataOffset,
        RoutingTableEntry*& rpNextHop);

    /** Sends a handshake message to the specified communication interface
    *
    * @param rDestination Message destination
    * @param isAck if true sends acknowledgement to the initial handshake
    * @param pGone Optional. On output contains a value indicating whether routing table entry is no longer valid
    * @return Return value of the operation
    */
    Result sendHandshake(RoutingTableEntry& rDestination, bool isAck, bool* pGone);

    /** Send a dont understand message to specified communication interface. Should be used if unknown message type is encountered 
    *
    * @param rDestination Message destination
    * @param requestId Id of the request
    * @param localClientId Id of the client that sent the message
    * @param pGone Optional. On output contains a value indicating whether routing table entry is no longer valid
    * @return Return value of the operation
    */
    Result sendDontUnderstand(
        RoutingTableEntry& rDestination, RequestId requestId, LocalClientId localClientId, bool* pGone);

    /** Sends a keep alive message to the specified communication interface
    *
    * @param rDestination Message destination
    * @param pGone Optional. On output contains a value indicating whether routing table entry is no longer valid
    * @return Return value of the operation
    */
    Result sendKeepAlive(RoutingTableEntry& rDestination, bool* pGone);

    /** Sends buffer to target whiteboard
    *
    * @param rDestination Message destination
    * @param pBuffer Buffer that contains the message
    * @param relockRoutingTableAndDereferenceAdapter Function unlocks routing table while it is doing send.
    *        CommAdapter reference count is incremented while routing table is unlocked so that it is not deallocated
    *        while it is still in use. This value indicates whether the function should relock routing table and
    *        decrement CommAdapter reference count after send.
    * @param pGone Optional. On output contains a value indicating whether routing table entry is no longer valid
    * @return Return value of the operation
    */
    Result sendBuffer(
        RoutingTableEntry& rDestination,
        Buffer* pBuffer,
        bool relockRoutingTableAndDereferenceAdapter,
        bool* pGone);

private:
    /** State of a request */
    typedef enum
    {
        /** Request has not been queued */
        SEND_REQUEST_STATE_NONE,

        /** Request has been sent to interface, but not yet completed */
        SEND_REQUEST_STATE_WAITING_FOR_COMPLETITION,

        /** Request is waiting for response. Used only for request
        * that have specified timeot */
        SEND_REQUEST_STATE_WAITING_FOR_RESPONSE,

        /** Request has been timed out, but not yet deallocated */
        SEND_REQUEST_STATE_TIMED_OUT
    } SendRequestState;

    /** Send request information */
    struct SendRequest
    {
        /** Next request in list of active requests */
        SendRequest* pNext;

        /** State of the request */
        SendRequestState state;

        /** State timeout */
        WbTimestamp stateTimeout;

        /** Communication adapter to use with the request. We could
         * dig this from nextHop, but having own pointer makes buffer
         * deallocation easier when routing table entry has been reassigned. 
         */
        CommAdapter* pNextHopAdapter;

        /** Routing table entry of next hop / destination */
        RoutingTableEntryHandle nextHop;

        /** Buffer associated with the request */
        Buffer* pBuffer;

        /** Request timeout in milliseconds. If zero request has no timeout. */
        size_t requestTimeoutMs;

        /** Type of the request */
        uint8 messageType : 7;

        /** A value indicating whether the message is request message
         * and whether error result should be sent to client if
         * send fails. */
        uint8 requestMessage : 1;

        /** Request ID */
        RequestId requestId;

        /** Resource ID */
        ResourceId resourceId;

        /** Client ID */
        ClientId clientId;
    };

    /** Removes request from currently executing requests
    *
    * @param pRequest Request that should be remove
    * @param pPrevious Previous send request
    * @return Pointer to next request for iteration
    */
    SendRequest* removeRequest(SendRequest* pRequest, SendRequest* pPrevious);

    /** Finds a request with specified ID
    *
    * @param requestId ID of the request
    * @param localClientId ID of the local client that sent the request
    * @param rpPrevious On output contains pointer to previous request for easier removal
    * @return Pointer to request or NULL if it was not found
    */
    SendRequest* findRequestById(RequestId requestId, LocalClientId localClientId, SendRequest*& rpPrevious);

private:
    /** Associated whiteboard instance (This is the singleton instance for all but unittest mockups) */
    Whiteboard& mrWhiteboard;

    /** Mutex that protects the internal data (routing table and comm adapter list) */
    WbRecursiveMutexHandle mMutexHandle;

    /** Routing table */
    RoutingTable mRoutingTable;

    /** First communication adapter */
    CommAdapter* mpFirstCommAdapter;

    /** Serial number of this whiteboard */
    SuuntoSerial& mrSerial;

    /** Event types */
    enum EventType
    {
        /** Invalid event */
        WB_COMM_EVENT_NONE,
        /** Send */
        WB_COMM_EVENT_SEND,
        /** Send has completed */
        WB_COMM_EVENT_SEND_COMPLETED,
        /** Response received */
        WB_COMM_EVENT_RESPONSE_RECEIVED,
        /** Request without execution context received */
        WB_COMM_EVENT_CONTEXTLESS_REQUEST_RECEIVED,
        /** Dummy event to wake communication thread */
        WB_COMM_EVENT_WAKE
    };

    /** Allocator for send request pool */
    BlockingFreeListPoolAllocator mSendRequestPoolAllocator;

    /** Pool of send requests */
    TypedPool<uint8, SendRequest> mSendRequestPool;

    /** First active request */
    SendRequest* mpFirstActiveRequest;

    /** Semaphore that protects receives from flooding the system */
    WbSemaphoreHandle mReceiveRequestSemaphore;

    /** Default client request timeout */
    const size_t mDefaultClientRequestTimeoutMs;

    /** Event processor settings */
    const metadata::ExecutionContext mEventProcessorSettings;

    /** The event processor.
     *
     * @note This is last in the initialization list so that it and its thread
     *       is terminated before any of the pools have been deallocated
     */
    EventProcessor mEventProcessor;
};

inline Whiteboard& WhiteboardCommunication::getWhiteboard()
{
    return mrWhiteboard;
}

inline RoutingTable& WhiteboardCommunication::getRoutingTable()
{
    return mRoutingTable;
}

} // namespace whiteboard
