#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

******************************************************************************/

#include "buildconfig/whiteboard/project.h"

// SS2 specific overrides of whiteboard settings 
//////////////////////////////////////////////////
// LOCAL WHITEBOARD
//////////////////////////////////////////////////

// TODO: Implement different allocator for PC version, where is no need to preallocate memory and limit these
/** Maximum number of resource clients allocated from local whiteboard */
#define WB_MAX_NUMBER_OF_CLIENTS 20

/** Maximum number of resource providers allocated from local whiteboard */
#define WB_MAX_NUMBER_OF_PROVIDERS 30

/** Maximum number of client subscriptions (both local and remote clients */
#define WB_MAX_NUMBER_OF_SUBSCRIPTIONS 100

/** Maximum number of dynamic resources allocated from local whiteboard
*
* @note This is intentionally kept low. Before just increasing
* this number, try to think alternative ways to implement the
* functionality.
*/
#define WB_MAX_NUMBER_OF_DYNAMIC_RESOURCES 10

/** Maximum number of entries in the stream pool */
#define WB_MAX_NUMBER_OF_SIMULTANEOUS_PUT_STREAMS 2

/** Maximum number of simultaneous whiteboard software timers */
#define WB_MAX_NUMBER_OF_TIMERS 24

//////////////////////////////////////////////////
// WHITEBOARD COMMUNICATION
//////////////////////////////////////////////////

#define WB_DEFAULT_DISPATCH_TIMEOUT_MS 1000

/** Maximum number of simultaneous send requests */
#define WB_COMM_PROTO_MAX_NUMBER_OF_SEND_REQUESTS 2

/** Maximum number of simultaneously queued receive requests */
#define WB_COMM_PROTO_MAX_NUMBER_OF_RECEIVE_REQUESTS 2

/** Maximum number of routing table entries */
#define WB_COMM_PROTO_MAX_NUMBER_OF_ROUTING_TABLE_ENTRIES 4

/** Length of reouting table provider's notification queue */
#define WB_COMM_ROUTINGTABLEPROVIDER_NOTIFICATIONQUEUE_LENGTH 4


// WB passes stack depth directly to freertos, so convert from bytes to machine
// words
#define WB_SERIAL_RECEIVER_STACK_DEPTH ((BUILD_WB_SERIAL_STACK_BYTES+3) / 4)

/** Number of small size buffers */
#define WB_SMALL_MESSAGE_SEND_BUFFERS 8

/** Number of medium size buffers */
#define WB_MEDIUM_MESSAGE_SEND_BUFFERS 8

/** Number of large size buffers */
#define WB_LARGE_MESSAGE_SEND_BUFFERS 4

/** Number of maximum length (approx 512-byte) buffers */
#define WB_MAXLEN_MESSAGE_SEND_BUFFERS 1

/** Number of buffers needed for Whiteboard communication internal messaging */
#define WB_COMM_INTERNAL_MESSAGE_BUFFERS 2
