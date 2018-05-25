#pragma once
// Copyright (c) Suunto Oy 2018. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/Identifiers.h"

#if !defined(NDEBUG) || defined(WB_HAVE_DEBUGLOG_IN_RELEASE)

namespace whiteboard
{

// Forward declarations
struct Buffer;
struct DataMessageHeader;

/** Writes message information to debug out
 *
 * @param tag Tag that should be printed before message information
 * @param rBuffer Message buffer
 * @param senderWhiteboardId Whiteboard ID of the message sender
 * @param receiverWhiteboardId Whiteboard ID of the message receiver
 */
void traceMessage(const char* tag, const Buffer& rBuffer, WhiteboardId senderWhiteboardId, WhiteboardId receiverWhiteboardId);

/** Writes message information to debug out
*
* @param tag Tag that should be printed before message information
* @param rMessageHeader Message header (and message data after the header)
* @param senderWhiteboardId Whiteboard ID of the message sender
* @param receiverWhiteboardId Whiteboard ID of the message receiver
*/
void traceMessage(const char* tag, const DataMessageHeader& rMessageHeader, WhiteboardId senderWhiteboardId, WhiteboardId receiverWhiteboardId);

}

#endif
