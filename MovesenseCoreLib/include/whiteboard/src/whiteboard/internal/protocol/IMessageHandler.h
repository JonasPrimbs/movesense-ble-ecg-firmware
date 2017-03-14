#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "whiteboard/Identifiers.h"
#include "whiteboard/Result.h"
#include "whiteboard/Request.h"
#include "whiteboard/internal/protocol/Messages.h"

namespace whiteboard
{

// Forward declarations
class ParameterList;
class Value;

/** Class that implements message handling */
class IMessageHandler
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~IMessageHandler() {}

public:
    /** Interface for message handler */
    class IHandler
    {
    protected:
        /** Protected destructor. Interface is not used to delete objects. */
        inline ~IHandler() {}

    public:
        /*********************
        * For Resource interface
        *********************/

        /**
        *	Handles GET RESOURCE request in caller's execution context.
        *
        *   @param rRequest Request information
        *	@param pFullPath Path of the resource to query
        */
        virtual void handleResourceGetRequest(const Request& rRequest, const char* pFullPath) = 0;

        /**
        *	Handles RELEASE RESOURCE request in caller's execution context.
        *
        *   @param rRequest Request information
        */
        virtual void handleResourceReleaseRequest(const Request& rRequest) = 0;

        /*********************
        * For Provider interface
        *********************/

        /**
        *	Handles request in provider's execution context.
        *
        *   @param rRequest Request information
        *	@param rParameters List of parameters for the request
        */
        virtual void handleProviderRequest(
            const Request& rRequest, const ParameterList& rParameters) = 0;

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
        virtual void handleClientOnNotify(
            ClientId clientId, ResourceId resourceId, const Value& rValue, const ParameterList& rParameters) = 0;

        /**
        *	Callback for asynchronous requests
        *
        *   @param rRequest Request information
        *	@param resultCode Result code of the request
        *	@param rResultData Successful result contains the request result
        */
        virtual void handleClientResult(
            const Request& rRequest, Result resultCode, const Value& rResultData) = 0;
    };

    /** Peeks ID of target from the message
    *
    * @param rMessageHeader The message
    * @param rTargetExecutionContextId On output contains ID of the target execution context
    * @param pTargetResourceId On output contains ID of the target resource
    * @param pTargetClientId On output contains ID of the target client
    * @return A value indicating whether the target was successfully parsed
    */
    virtual bool peekTarget(
        const DataMessageHeader& rMessageHeader,
        ExecutionContextId& rTargetExecutionContextId,
        LocalResourceId* pTargetResourceId,
        LocalClientId* pTargetClientId) const = 0;

    /** Peeks ID of source from the message
    *
    * @param rMessageHeader The message
    * @param rSourceExecutionContextId On output contains ID of the source execution context
    * @param pSourceResourceId On output contains ID of the source resource
    * @param pSourceClientId On output contains ID of the source client
    * @return A value indicating whether the source was successfully parsed
    */
    virtual bool peekSource(
        const DataMessageHeader& rMessageHeader,
        ExecutionContextId& rSourceExecutionContextId,
        LocalResourceId* pSourceResourceId,
        LocalClientId* pSourceClientId) const = 0;

    /** Handles message in current execution context
    *
    * @param rHandler Handler of deserialized message
    * @param remoteWhiteboardIdInLocalScope ID of remote Whiteboard as seen by us
    * @param rMessageHeader Header of the message
    * @return A value indicating whether the message was successfully deserialized
    */
    virtual bool handleMessage(
        IHandler& rHandler, WhiteboardId remoteWhiteboardIdInLocalScope, const DataMessageHeader& rMessageHeader) const = 0;
};

} // namespace whiteboard
