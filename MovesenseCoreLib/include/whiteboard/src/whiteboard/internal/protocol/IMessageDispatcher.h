#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/Result.h"
#include "whiteboard/Request.h"
#include "whiteboard/internal/IBufferAllocator.h"
#include "whiteboard/internal/protocol/Messages.h"
#include "whiteboard/WbVersion.h"

namespace whiteboard
{

// Forward declarations
class ParameterList;
class Value;

/** Interface for protocol specific message dispatching */
class IMessageDispatcher
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~IMessageDispatcher() {}

public:
    /** Interface for classes that do actual buffer allocation and dispatching */
    class IDispatcher
    {
    protected:
        /** Protected destructor. Interface is not used to delete objects. */
        inline ~IDispatcher() {}

    public:
        /** Gets buffer allocator instance
        *
        * @return Buffer allocator instance
        */
        virtual IBufferAllocator& getBufferAllocator() = 0;

        /** Dispatches a data message
        * Requests may fail if receiving context's request queue is full. EventProcessor detects if it is a request or response.
        * @param messageType The type of the message.
        * @param pBuffer Buffer that contains the message that should be dispatched
        * @param whiteboardIdInLocalScope Destination whiteboard ID as seen by the local whiteboard
        * @param protocol Whiteboard comm protocol version
        * @param dontBlockIfQueueFull If true returns immediately with error, if the queue is full, else blocks and waits untill space in the queue.
        * @return Result code of the dispatch operation.
        */
        virtual Result dispatch(const MessageType messageType, 
                                Buffer* pBuffer,
                                WhiteboardId whiteboardIdInLocalScope = LOCAL_WHITEBOARD_ID,
                                ProtocolVersion protocol = WB_COMM_PROTO_CURRENT_VERSION,
                                bool dontBlockIfQueueFull = false) = 0;
    };

    /** Additional information for message routing */
    struct RoutingInfo
    {
        /** Constructor
         *
         * @param _routed A value indicating whether the message is routed
         * @param _rDestination Serial number of mesage destination
         * @param _rSource Serial number of message source
         */
        RoutingInfo(bool _routed, const SuuntoSerial& _rDestination, const SuuntoSerial& _rSource)
            : routed(_routed), rDestination(_rDestination), rSource(_rSource)
        {}

        /** A value indicating whether the message is routed */
        bool routed;

        /** Serial number of mesage destination */
        const SuuntoSerial& rDestination;

        /** Serial number of message source */
        const SuuntoSerial& rSource;
    };

    /**
    *	Sends GET RESOURCE request to destination
    *
    *	@param requestId ID of the request
    *	@param pFullPath Path of the resource
    *	@param clientId ID of the client that will receive the callback.
    *	@param rDispatcher Dispatcher implementation
    *   @param pRoutingInfo Optional information about message routing
    *   @param dontBlockIfQueueFull If true returns immediately with error, if the queue is full, else blocks and waits untill space in the queue.
    *	@return Result code of the operation and ID of the request
    */
    virtual Result dispatchResourceGetRequest(
        RequestId requestId,
        const char* pFullPath,
        ClientId clientId,
        IDispatcher& rDispatcher,
        const RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const = 0;

    /**
    *	Sends RELEASE RESOURCE to destination
    *
    *	@param requestId ID of the request
    *	@param resourceId ID of the resource
    *	@param clientId ID of the client that will receive the callback.
    *	@param rDispatcher Dispatcher implementation
    *   @param pRoutingInfo Optional information about message routing
    *   @param dontBlockIfQueueFull If true returns immediately with error, if the queue is full, else blocks and waits untill space in the queue.
    *	@return Result code of the operation and ID of the request
    */
    virtual Result dispatchResourceReleaseRequest(
        RequestId requestId,
        ResourceId resourceId,
        ClientId clientId,
        IDispatcher& rDispatcher,
        const RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const = 0;

    /**
    *	Sends resource changed notification for to client's execution context
    *
    *	@param clientId ID of the client that should receive this message
    *	@param resourceId ID of the associated resource
    *	@param rValue Current value of the resource
    *	@param rParameters List of parameters
    *	@param rDispatcher Dispatcher implementation
    *   @param pRoutingInfo Optional information about message routing
    *   @param dontBlockIfQueueFull If true returns immediately with error, if the queue is full, else blocks and waits untill space in the queue.
    *	@return Result of the operation
    */
    virtual Result dispatchClientOnNotify(
        ClientId clientId,
        ResourceId resourceId,
        const Value& rValue,
        const ParameterList& rParameters,
        IDispatcher& rDispatcher,
        const RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const = 0;

    /**
    *	Sends results of a request to client's execution context
    *	(GET RESOURCE, RELEASE RESOURCE, GET, PUT, POST, DELETE, SUBSCRIBE & UNSUBSCRIBE).
    *
    *	@param rRequest The request
    *	@param resultCode Result code of the request
    *	@param rResultData Result of the request
    *	@param rDispatcher Dispatcher implementation
    *   @param pRoutingInfo Optional information about message routing
    *   @param dontBlockIfQueueFull If true returns immediately with error, if the queue is full, else blocks and waits untill space in the queue.
    *	@return Result of the operation
    */
    virtual Result dispatchClientResult(
        const Request& rRequest, 
        Result resultCode,
        const Value& rResultData,
        IMessageDispatcher::IDispatcher& rDispatcher,
        const IMessageDispatcher::RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const = 0;

    /**
    *	Sends requests to provider's execution context.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *	@param rDispatcher Dispatcher implementation
    *   @param pRoutingInfo Optional information about message routing
    *   @param dontBlockIfQueueFull If true returns immediately with error, if the queue is full, else blocks and waits untill space in the queue.
    *	@return Result of the operation
    */
    virtual Result dispatchProviderRequest(
        const Request& rRequest,
        const ParameterList& rParameters,
        IDispatcher& rDispatcher,
        const RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const = 0;

    /** Allocates buffer for a message and writes its header
    *
    *	@param rBufferAllocator Buffer allocator to use
    *   @param poolType Type of the pool to use
    *   @param messageLength Length of the message
    *	@param messageType Type of the message being sent
    *	@param requestId ID of the request
    *   @param pRoutingInfo Optional information about message routing
    *   @param rDataOffset On output contains offset where additional data should be written
    *   @return Buffer instance
    */
    static Buffer* allocateBufferAndWriteHeader(
        IBufferAllocator& rBufferAllocator,
        IBufferAllocator::PoolType poolType,
        size_t messageLength,
        MessageType messageType,
        RequestId requestId,
        const RoutingInfo* pRoutingInfo,
        size_t& rDataOffset);
};


} // namespace whiteboard
