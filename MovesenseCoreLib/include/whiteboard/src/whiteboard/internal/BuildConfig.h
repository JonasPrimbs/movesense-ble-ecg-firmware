#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2014.
    All rights reserved.
******************************************************************************/

#include <stddef.h>
#include "whiteboard/integration/shared/macros.h"

//////////////////////////////////////////////////
// FEATURES
//////////////////////////////////////////////////

#ifndef WB_HAVE_BUS_COMM
#define WB_HAVE_BUS_COMM 0
#endif

#ifndef WB_HAVE_TCP_COMM
#define WB_HAVE_TCP_COMM 0
#endif

#ifndef WB_HAVE_UDP_COMM
#define WB_HAVE_UDP_COMM 0
#endif

#ifndef WB_HAVE_SERIAL_COMM
#define WB_HAVE_SERIAL_COMM 0
#endif

#ifndef WB_HAVE_BLE_COMM
#define WB_HAVE_BLE_COMM 0
#endif

//////////////////////////////////////////////////
// LOCAL WHITEBOARD
//////////////////////////////////////////////////

/** Maximum wait for EventProcessor dispatch timeout */
#define WB_DEFAULT_DISPATCH_TIMEOUT_MS 50u

//////////////////////////////////////////////////
// WHITEBOARD COMMUNICATION
//////////////////////////////////////////////////

/** Maximum number of milliseconds the sleep clock may drift from the timestamp clock source 
 in a second. This value doesn't need to be very accurate and can be larger than needed. */
#define WB_COMM_SLEEP_MAX_CLOCK_ERROR_MS 10

/** Timeout handling resolution in milliseconds for whiteboard communication request */
#define WB_COMM_TIMEOUT_RESOLUTION_MS 100

#if (WB_COMM_SLEEP_MAX_CLOCK_ERROR_MS <= 0) || (WB_COMM_TIMEOUT_RESOLUTION_MS <= 0) || \
    (WB_COMM_SLEEP_MAX_CLOCK_ERROR_MS > WB_COMM_TIMEOUT_RESOLUTION_MS)
#error Invalid configuration: Defined communication timeout constants would break implementation.
#endif

/** Maximum number of hops to route a message */
#define WB_COMM_PROTO_TIME_TO_LIVE 16

/** Maximum number of routing table observers to attach to routing table. */
#define WB_COMM_MAX_NUMBER_OF_ROUTING_TABLE_OBSERVERS 2

/** Length of reouting table provider's notification queue */
#define WB_COMM_ROUTINGTABLEPROVIDER_NOTIFICATIONQUEUE_LENGTH 4

/** Number of buffers needed for Whiteboard communication internal messaging */
#define WB_COMM_INTERNAL_MESSAGE_BUFFERS 2

/** Size of Whiteboard communication internal messaging buffers (53 bytes for routed handshakes, 35 bytes after serial number
* changes)*/
#define WB_COMM_INTERNAL_MESSAGE_BUFFER_LENGTH 56

/** Communication buffers are always maximum length */
#define WB_COMM_MESSAGE_BUFFER_LENGTH (WB_MAX_MESSAGE_LENGTH + WB_MESSAGE_ALIGNMENT)

/** Length of maximum length (approx 512-byte) buffers */
#define WB_MAXLEN_MESSAGE_SEND_BUFFER_SIZE WB_COMM_MESSAGE_BUFFER_LENGTH

/** Maximum stack depth of whiteboard communication protocol. */
#define WB_COMM_PROTO_STACK_DEPTH 512

//////////////////////////////////////////////////
// WHITEBOARD IP ADAPTER
//////////////////////////////////////////////////

/** Select timeout of IP receiver thread */
#define WB_IP_RECEIVER_SELECT_TIMEOUT 10

/** Number of extra header bytes to allocate for IP buffers (Berkeley socket API needs no extra space.) */
#define WB_IP_BUFFER_HEADER_SPACE 0

/** Stack size of IP receiver thread */
#define WB_IP_RECEIVER_STACK_DEPTH 384

//////////////////////////////////////////////////
// WHITEBOARD BUS ADAPTER
//////////////////////////////////////////////////

#if (WB_HAVE_BUS_COMM == 1)

/** Number of extra header bytes to allocate for BUS buffers */
#ifndef WB_BUS_BUFFER_HEADER_SPACE
#error WB_BUS_BUFFER_HEADER_SPACE must be defined by build system
#endif

#else // (WB_HAVE_BUS_COMM == 1)

#undef WB_BUS_BUFFER_HEADER_SPACE
#define WB_BUS_BUFFER_HEADER_SPACE 0

#endif // (WB_HAVE_BUS_COMM == 1)

//////////////////////////////////////////////////
// WHITEBOARD SERIAL ADAPTER
//////////////////////////////////////////////////

/** Number of extra header bytes to allocate for SERIAL buffers (one byte required for message start identification from the
 * bytestream) */
#define WB_SERIAL_BUFFER_HEADER_SPACE WB_MAX(1, WB_MESSAGE_ALIGNMENT)

/** Stack size of serial receiver thread */
#define WB_SERIAL_RECEIVER_STACK_DEPTH 384

//////////////////////////////////////////////////
// WHITEBOARD BLE ADAPTER
//////////////////////////////////////////////////

/** Stack size of BLE receiver thread */
#define WB_BLE_RECEIVER_STACK_DEPTH (256 + 62)
