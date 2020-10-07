#pragma once
// Copyright (c) Suunto Oy 2014. All rights reserved.

/** Maximum length of Suunto whiteboard serial number */
#define WB_MAX_SERIAL_NUMBER_LENGTH 16

/** Minimum length of Suunto whiteboard serial number */
#define WB_MIN_SERIAL_NUMBER_LENGTH 1

/** Size of packed serial number in bytes == sizeof(SuuntoSerial) */
#define WB_PACKED_SERIAL_NUMBER_SIZE 12

/** Maximum length of resource path */
#define WB_MAX_RESOURCE_PATH_LEN 255

/** Maximum number of parameters in a single operation. */
#define WB_MAX_NUMBER_OF_PARAMETERS 8

/** Maximum number of path parameters in single resource path */
#define WB_MAX_NUMBER_OF_PATH_PARAMETERS 4

/** Maximum number of responses in single operation. */
#define WB_MAX_NUMBER_OF_RESPONSES 32

/** Maximum number of structure properties. */
#define WB_MAX_NUMBER_OF_STRUCTURE_PROPERTIES 64

/** Maximum depth of structures. */
#define WB_MAX_STRUCTURE_DEPTH 4

/** Maximum size of message payload */
#define WB_MAX_MESSAGE_PAYLOAD_LENGTH 466 // was 470, optional 4 bytes used for 32 bit timestamp in the datamessage serialisation

/** Minimum length of whiteboard messages */
#define WB_MIN_MESSAGE_LENGTH 6

/** Maximum length of Whiteboard messages. Changing this will break backwards compatibility. This might be ok, if it is otherwise
 * guaranteed that larger messages are not used with "older" devices. If you plan to increase this, see also StructureValueHeader
 * that defines that maximum size of single structure is 511 bytes (there are some extra bits to increase this).
 */
#define WB_MAX_MESSAGE_LENGTH 512

/** Message alignment */
#define WB_MESSAGE_ALIGNMENT 8

#if (WB_MESSAGE_ALIGNMENT % 8 != 0) || (WB_MESSAGE_ALIGNMENT <= 0)
#error WB_MESSAGE_ALIGNMENT must be multiple of eight (alignment of int64 and double)
#endif

/** Maximum address length is defined to hold IPv4 addresses (sockaddr_in structure) */
#define WB_COMM_MAX_ADDRESS_LENGTH 16

/** Unit test helpers require custom buffer pools always.
 */
#ifdef WB_UNITTEST_BUILD
#undef WB_HAVE_CUSTOM_BUFFER_POOLS
#define WB_HAVE_CUSTOM_BUFFER_POOLS 1
#endif
