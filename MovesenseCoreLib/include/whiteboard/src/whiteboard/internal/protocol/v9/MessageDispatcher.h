#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/internal/protocol/IMessageDispatcher.h"
#include "whiteboard/internal/protocol/v9/Serializer.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Class that builds messages for dispatching */
class MessageDispatcher FINAL : public IMessageDispatcher
{
public:
    /** Destructor */
    virtual ~MessageDispatcher() {}

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
    Result dispatchResourceGetRequest(
        RequestId requestId,
        const char* pFullPath,
        ClientId clientId,
        IDispatcher& rDispatcher,
        const RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const OVERRIDE;

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
    Result dispatchResourceReleaseRequest(
        RequestId requestId,
        ResourceId resourceId,
        ClientId clientId,
        IDispatcher& rDispatcher,
        const RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const OVERRIDE;

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
    Result dispatchClientOnNotify(
        ClientId clientId,
        ResourceId resourceId,
        const Value& rValue,
        const ParameterList& rParameters,
        IDispatcher& rDispatcher,
        const RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const OVERRIDE;

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
    Result dispatchClientResult(
        const Request& rRequest,
        Result resultCode,
        const Value& rResultData,
        IMessageDispatcher::IDispatcher& rDispatcher,
        const IMessageDispatcher::RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const OVERRIDE;

    /**
    *	Sends requests to provider's execution context.
    *	(GET, PUT, POST, DELETE, SUBSCRIBE & UNSUBSCRIBE).
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *	@param rDispatcher Dispatcher implementation
    *   @param pRoutingInfo Optional information about message routing
    *   @param dontBlockIfQueueFull If true returns immediately with error, if the queue is full, else blocks and waits untill space in the queue.
    *	@return Result of the operation
    */
    Result dispatchProviderRequest(
        const Request& rRequest,
        const ParameterList& rParameters,
        IDispatcher& rDispatcher,
        const RoutingInfo* pRoutingInfo,
        bool dontBlockIfQueueFull) const OVERRIDE;

private:
    /** Makes all preparations for serialization ie. allocates buffer for
    *   a message, writes its header and initializes Serializer instance
    *
    *	@param rDispatcher Dispatcher implementation
    *   @param messageLength Length of the message
    *	@param messageType Type of the data message being sent
    *	@param requestId ID of the request
    *   @param pRoutingInfo Optional information about message routing
    *   @param rpBuffer On output contains pointer to the buffer
    *   @return Message serializer instance NOTE: copy of NULL-REFERENCE if rpBuffer == NULL
    */
    static Serializer prepareSerialization(
        MessageDispatcher::IDispatcher& rDispatcher,
        size_t messageLength,
        MessageType messageType,
        RequestId requestId,
        const MessageDispatcher::RoutingInfo* pRoutingInfo,
        Buffer*& rpBuffer);
};

} // protocol_v9
} // namespace whiteboard
