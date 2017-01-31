#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/internal/protocol/WhiteboardVersion.h"

namespace whiteboard
{

/** Protocol message types, do not change order to preserve backwards compatibility! */
enum MessageType
{
    /*0*/ WB_MSG_INVALID = 0,

    /*1*/ WB_DATAMSG_CLIENT_ON_NOTIFY,
    /*2*/ WB_DATAMSG_CLIENT_ON_GET_RESOURCE_RESULT,
    /*3*/ WB_DATAMSG_CLIENT_ON_RELEASE_RESOURCE_RESULT,
    /*4*/ NOT_USED_1,
    /*5*/ WB_DATAMSG_CLIENT_ON_GET_RESULT,
    /*6*/ NOT_USED_2,
    /*7*/ WB_DATAMSG_CLIENT_ON_PUT_RESULT,
    /*8*/ WB_DATAMSG_CLIENT_ON_SUBSCRIBE_RESULT,
    /*9*/ WB_DATAMSG_CLIENT_ON_UNSUBSCRIBE_RESULT,

    /*10*/ WB_DATAMSG_RESOURCE_GET_REQUEST,
    /*11*/ WB_DATAMSG_RESOURCE_RELEASE_REQUEST,
    /*12*/ NOT_USED_3,
    /*13*/ WB_DATAMSG_PROVIDER_GET_REQUEST,
    /*14*/ WB_DATAMSG_PROVIDER_PUT_REQUEST,
    /*15*/ NOT_USED_4,
    /*16*/ WB_DATAMSG_PROVIDER_SUBSCRIBE_REQUEST,
    /*17*/ WB_DATAMSG_PROVIDER_UNSUBSCRIBE_REQUEST,

    /*18*/ WB_COMMMSG_HELLO,
    /*19*/ WB_COMMMSG_HELLO_ACK,
    /*20*/ WB_COMMMSG_KEEP_ALIVE,

    /** Following was used by initial timer implementation, but was not ever transferred over the network */
    /*21*/ NOT_USED_5,

    /*22*/ WB_DATAMSG_PROVIDER_POST_REQUEST,
    /*23*/ WB_DATAMSG_PROVIDER_DELETE_REQUEST,
    /*24*/ WB_DATAMSG_CLIENT_ON_POST_RESULT,
    /*25*/ WB_DATAMSG_CLIENT_ON_DELETE_RESULT,

    /*26*/ WB_DATAMSG_DONT_UNDERSTAND,
    WB_DATAMSG_LAST_MESSAGE = WB_DATAMSG_DONT_UNDERSTAND // In the future the last message could be a message that comes after WB_DONT_UNDERSTAND
};

#define WHITEBOARD_DATA_MESSAGE_MAGIC 0xA5

WB_PACK_BEGIN()

/** Message header for data messages */
struct WB_PACKED DataMessageHeader
{
    /** All whiteboard messages starts with this magic byte; 1010 0101; 0xA5  */
    uint8 magic;

    /** Type of the message */
    uint8 messageType : 7;

    /** A value indicating whether the message is routed or direct */
    uint8 routed : 1;

    //TODO: For PROTOCOL v10 change this to 
    // uint16 messageLength : 10
    // uint16 protocol : 2 (control protocol [handshakes], request protocol [request, response, notify], routing protocol, reserved)
    // uint16 reserved : 2
    // uint16 protocolFlags : 2 (for request protocol 0/1 = response/request, 0/2 = contextless) 
    // - Maybe also renumber messageTypes and use mapping in v9 to match new implementation
    // - Maybe reserve some bits from messageType for future use
    // TODO: Reduce control protocol dependencies from WhiteboardCommunication

    /** Length of the message.*/
    uint16 messageLength;

    union
    {
        /** ID of the request. This is valid only for certain messages */
        RequestId requestId;

        /** Reserved for future use */
        uint16 reserved;
    };
};

WB_STATIC_VERIFY(sizeof(DataMessageHeader) == 6, SizeOfMessageHeaderIsNotSixBytes);

/** Checks if the message is a contextless request message
*
* @param rMessageHeader Message header
* @return A value indicating whether this is a request message
*/
inline bool isContextlessRequestMessage(const DataMessageHeader& rMessageHeader)
{
    switch (rMessageHeader.messageType)
    {
    case WB_DATAMSG_RESOURCE_GET_REQUEST:
    case WB_DATAMSG_RESOURCE_RELEASE_REQUEST:
        return true;
    default:
        return false;
    }
}

/** Checks if the message is a request message
*
* @param messageType Type of the message
* @return A value indicating whether this is a request message
*/
inline bool isRequestMessage(const DataMessageHeader& rMessageHeader)
{
    switch (rMessageHeader.messageType)
    {
    case WB_DATAMSG_PROVIDER_GET_REQUEST:
    case WB_DATAMSG_PROVIDER_PUT_REQUEST:
    case WB_DATAMSG_PROVIDER_SUBSCRIBE_REQUEST:
    case WB_DATAMSG_PROVIDER_UNSUBSCRIBE_REQUEST:
    case WB_DATAMSG_PROVIDER_POST_REQUEST:
    case WB_DATAMSG_PROVIDER_DELETE_REQUEST:
        return true;
    default:
        return false;
    }
}

/** Checks if the message is a result message
*
* @param rMessageHeader Message header
* @return A value indicating whether this is a result message
*/
inline bool isResultMessage(const DataMessageHeader& rMessageHeader)
{
    switch (rMessageHeader.messageType)
    {
    case WB_DATAMSG_CLIENT_ON_GET_RESOURCE_RESULT:
    case WB_DATAMSG_CLIENT_ON_RELEASE_RESOURCE_RESULT:
    case WB_DATAMSG_CLIENT_ON_GET_RESULT:
    case WB_DATAMSG_CLIENT_ON_PUT_RESULT:
    case WB_DATAMSG_CLIENT_ON_SUBSCRIBE_RESULT:
    case WB_DATAMSG_CLIENT_ON_UNSUBSCRIBE_RESULT:
    case WB_DATAMSG_CLIENT_ON_POST_RESULT:
    case WB_DATAMSG_CLIENT_ON_DELETE_RESULT:
    case WB_DATAMSG_DONT_UNDERSTAND:
        return true;
    default:
        return false;
    }
}

/** Wrapper structure for generic data message handling */
struct WB_PACKED DataMessage
{
    /** Message header */
    DataMessageHeader header;

    /** Data */
    uint8 data[1];
};

// Type for hop counting
typedef uint8 HopCount;

// Type for sequence numbering route discovery messages
typedef uint8 SequenceNumber;

/** Message routing header */
struct WB_PACKED DataMessageRoutingHeader
{
    /** Destination address of the message */
    SuuntoSerial destination;

    /** Return address of the message */
    SuuntoSerial returnAddress;

    /** Current number of hops */
    HopCount numberOfHops;
};

WB_STATIC_VERIFY(sizeof(DataMessageRoutingHeader) == 2 * sizeof(SuuntoSerial) + sizeof(HopCount), SizeOfDataMessageRoutingHeaderIsNotExpected);

/** Hello message payload */
struct WB_PACKED Hello
{
    /** Protocol version */
    ProtocolVersion protocolVersion;

    /** Minimum supported protocol version */
    ProtocolVersion minProtocolVersion;

    /** Receivers whiteboard ID on senders side */
    WhiteboardId receiverWhiteboardId;

    /** Serial number of whiteboard */
    SuuntoSerial serialNumber;

    /** Whiteboard version number */
    WhiteboardVersion whiteboardVersion;

    /** A value indicating whether keep alives should not be sent for the route */
    uint8 noKeepAlives : 1;

    /** A value indicating whether receiver can not reconnect sender after communication breakage */
    uint8 notReconnectable : 1;

    /** A value indicating whether data on this communication link should be encrypted */
    uint8 encryptData : 1;

    /** Reserved for future use, initialized with 0 */
    uint8 reserved : 5;

    /** Reserved for future use, initialized with 0 */
    uint8 reserved2[8];
};

WB_STATIC_VERIFY(sizeof(Hello) == 3 + WB_PACKED_SERIAL_NUMBER_SIZE + 8 + 9, SizeOfHelloMessageIsNotExpected);

/** Response to unknown whiteboard message */
typedef struct WB_PACKED
{
    /** Result code of the message*/
    uint16 resultCode;

    /** ID of the client that sent the message or ID_INVALID_LOCAL_CLIENT
     * when message is not a request protocol message. */
    LocalClientId localClientId;

    /** Reserved for future use */
    uint8 reserved[2];
} DontUnderstand;

WB_STATIC_VERIFY(sizeof(DontUnderstand) == 2 + 2 + 2, SizeOfDontUnderstandMessageIsNotExpected);

/** Gets length of the header
*
* @param rMessageHeader Header which length should be calculated
* @return Length of the header in bytes
*/
inline static size_t getHeaderLength(bool routed)
{
    size_t headerLength = sizeof(DataMessageHeader);
    if (routed)
    {
        headerLength += sizeof(DataMessageRoutingHeader);
    }

    return headerLength;
}

WB_PACK_END()


} // namespace whiteboard
