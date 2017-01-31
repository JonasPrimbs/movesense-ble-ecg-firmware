#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/internal/protocol/IMessageHandler.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Class that implements message handling */
class MessageHandler FINAL : public IMessageHandler
{
public:
    /** Destructor */
    virtual ~MessageHandler() {}

    /** Peeks ID of target from the message
    *
    * @param rMessageHeader The message
    * @param rTargetExecutionContextId On output contains ID of the target execution context
    * @param pTargetResourceId On output contains ID of the target resource
    * @param pTargetClientId On output contains ID of the target client
    * @return A value indicating whether the target was successfully parsed
    */
    bool peekTarget(
        const DataMessageHeader& rMessageHeader, 
        ExecutionContextId& rTargetExecutionContextId, 
        LocalResourceId* pTargetResourceId,
        LocalClientId* pTargetClientId) const OVERRIDE;

    /** Peeks ID of source from the message
    *
    * @param rMessageHeader The message
    * @param rSourceExecutionContextId On output contains ID of the source execution context
    * @param pSourceResourceId On output contains ID of the source resource
    * @param pSourceClientId On output contains ID of the source client
    * @return A value indicating whether the source was successfully parsed
    */
    bool peekSource(
        const DataMessageHeader& rMessageHeader,
        ExecutionContextId& rSourceExecutionContextId,
        LocalResourceId* pSourceResourceId,
        LocalClientId* pSourceClientId) const OVERRIDE;

    /** Handles message in current execution context
    *
    * @param rHandler Handler of deserialized message
    * @param remoteWhiteboardIdInLocalScope ID of remote Whiteboard as seen by us
    * @param rMessageHeader Header of the message
    * @return A value indicating whether the message was successfully deserialized
    */
    bool handleMessage(IHandler& rHandler, WhiteboardId remoteWhiteboardIdInLocalScope, const DataMessageHeader& rMessageHeader) const OVERRIDE;

private:
    /** Handles GET RESOURCE request in current execution context
    *
    * @param rHandler Handler of deserialized message
    * @param remoteWhiteboardIdInLocalScope ID of remote Whiteboard as seen by us
    * @param rMessageHeader Header of the message
    * @return A value indicating whether the message was successfully deserialized
    */
    bool handleResourceGetRequest(IHandler& rHandler, WhiteboardId remoteWhiteboardIdInLocalScope, const DataMessageHeader& rMessageHeader) const;

    /** Handles RELEASE RESOURCE request in current execution context
    *
    * @param rHandler Handler of deserialized message
    * @param remoteWhiteboardIdInLocalScope ID of remote Whiteboard as seen by us
    * @param rMessageHeader Header of the message
    * @return A value indicating whether the message was successfully deserialized
    */
    bool handleResourceReleaseRequest(IHandler& rHandler, WhiteboardId remoteWhiteboardIdInLocalScope, const DataMessageHeader& rMessageHeader) const;

    /** Handles notify message in client's execution context
     *
     * @param rHandler Handler of deserialized message
     * @param remoteWhiteboardIdInLocalScope ID of remote Whiteboard as seen by us
     * @param rMessageHeader Header of the message
     * @return A value indicating whether the message was successfully deserialized
     */
    bool handleClientOnNotify(IHandler& rHandler, WhiteboardId remoteWhiteboardIdInLocalScope, const DataMessageHeader& rMessageHeader) const;

    /** Handles results of client requests
    *  in client's execution context
    *
    * @param rHandler Handler of deserialized message
    * @param remoteWhiteboardIdInLocalScope ID of remote Whiteboard as seen by us
    * @param rMessageHeader Header of the message
    * @return A value indicating whether the message was successfully deserialized
    */
    bool handleClientResult(IHandler& rHandler, WhiteboardId remoteWhiteboardIdInLocalScope, const DataMessageHeader& rMessageHeader) const;

    /** Handles client requests in provider's execution context
    *
    * @param rHandler Handler of deserialized message
    * @param remoteWhiteboardIdInLocalScope ID of remote Whiteboard as seen by us
    * @param rMessageHeader Header of the message
    * @return A value indicating whether the message was successfully deserialized
    */
    bool handleProviderRequest(IHandler& rHandler, WhiteboardId remoteWhiteboardIdInLocalScope, const DataMessageHeader& rMessageHeader) const;
};

} // namespace protocol_v9
} // namespace whitespace
