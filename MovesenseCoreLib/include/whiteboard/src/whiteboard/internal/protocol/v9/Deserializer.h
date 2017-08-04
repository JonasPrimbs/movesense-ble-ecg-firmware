#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/ParameterList.h"
#include "whiteboard/Result.h"
#include "whiteboard/Value.h"
#include "whiteboard/internal/protocol/Messages.h"
#include "whiteboard/internal/protocol/v9/Shared.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Buffer data deserializer */
class Deserializer FINAL
{
public:
    /** Constructor
    *
    * @param rBuffer Owning buffer instance
    */
    inline Deserializer(const DataMessage& rBuffer);

    /** Destructor */
    inline ~Deserializer();

    /** Skips given number of bytes
     *
     * @return A value indicating whether the operation succeeded
    */
    inline bool skip(size_t numberOfBytes);

    /** Deserializes string from the buffer
    *
    * @param length On input contains maximum length of the string. On output contains actual length of the string.
    *               Both values exclude null terminator, even though the function adds the terminator.
    * @param string On output contains deserialized string
    * @return A value indicating whether the operation succeeded
    */
    template <typename LengthType>
    inline bool deserializeString(LengthType& rLength, char* string);

    /** Deserializes request result from the buffer
    *
    * @param rResultCode On output contains result of request
    * @return A value indicating whether the operation succeeded
    */
    inline bool deserializeResult(Result& rResultCode);

    /** Deserializes resource ID from the buffer
    *
    * @param rResourceId On output contains id of the resource
    * @param  whiteboardId ID of whiteboard to set for the resource
    * @return A value indicating whether the operation succeeded
    */
    inline bool deserializeResourceId(ResourceId& rResourceId, WhiteboardId whiteboardId);

    /** Deserializes client ID from the buffer
    *
    * @param rClientId On output contains id of the client
    * @param  whiteboardId ID of whiteboard to set for the client
    * @return A value indicating whether the operation succeeded
    */
    inline bool deserializeClientId(ClientId& rClientId, WhiteboardId whiteboardId);

    /** Deserializes whiteboard value from the buffer
    *
    * @param rValue On output contains deserialized whiteboard value
    * @return A value indicating whether the operation succeeded
    */
    inline bool deserializeValue(Value& rValue);

    /** Deserializes a value from given buffer
    *
    * @param rValue On output contains deserialized whiteboard value
    * @param pBuffer Buffer where data should be read
    * @param maxLength Maximum length of the buffer
    * @param rNumberOfDeserializedBytes Number of deserialized bytes
    * @return A value indicating whether the operation succeeded
    */
    static bool deserializeValue(Value& rValue, const void* pBuffer, size_t maxLength, size_t& rNumberOfDeserializedBytes);

    /** Deserializes whiteboard parameter list parameter count from the buffer
    *
    * @param rNumberOfParameters On output contains number of parameters in parameter list
    * @return A value indicating whether the operation succeeded
    */
    bool deserializeParameterListLength(size_t& rNumberOfParameters);

    /** Deserializes whiteboard parameter list from the buffer
    *
    * @param rParameterList On output contains deserialized whiteboard parameter list
    * @return A value indicating whether the operation succeeded
    */
    bool deserializeParameterList(ParameterList& rParameterList);

    /** Checks that all data has been deserialized
     *
     * @return A value indicating whether the operation succeeded
     */
    inline bool check() const;

private:
    /** Owning buffer instance */
    const DataMessage* mpBuffer;

    /** Current deserialization position in the buffer */
    size_t mPosition;
};

Deserializer::Deserializer(const DataMessage& rBuffer) : mpBuffer(&rBuffer), mPosition(0)
{
}

Deserializer::~Deserializer()
{
}

bool Deserializer::skip(size_t numberOfBytes)
{
    if (mPosition + numberOfBytes > mpBuffer->header.messageLength)
    {
        return false;
    }

    mPosition += numberOfBytes;
    return true;
}

template <typename LengthType>
bool Deserializer::deserializeString(LengthType& rLength, char* string)
{
    if (mPosition + sizeof(LengthType) > mpBuffer->header.messageLength)
    {
        return false;
    }

    LengthType deserializedLength;
    memcpy(&deserializedLength, &mpBuffer->data[mPosition], sizeof(LengthType));
    mPosition += sizeof(LengthType);

    if ((mPosition + deserializedLength > mpBuffer->header.messageLength) ||
        (rLength < deserializedLength))
    {
        return false;
    }

    rLength = deserializedLength;

    memcpy(string, &mpBuffer->data[mPosition], deserializedLength);
    string[deserializedLength] = 0;
    mPosition += deserializedLength;
    return true;
}

bool Deserializer::deserializeResult(Result& rResultCode)
{
    if (mPosition + sizeof(uint16) > mpBuffer->header.messageLength)
    {
        return false;
    }

    uint16 binaryValue;
    memcpy(&binaryValue, &mpBuffer->data[mPosition], sizeof(uint16));
    rResultCode = static_cast<Result>(binaryValue);
    mPosition += sizeof(uint16);
    return true;
}

bool Deserializer::deserializeResourceId(ResourceId& rResourceId, WhiteboardId whiteboardId)
{
    // Can't really verify that resource instance and execution context are located in same byte. Do next best thing.
    WB_STATIC_VERIFY(sizeof(rResourceId.localResourceId) == 2, ResourceIdIsNotWhatExpected_1);
    WB_STATIC_VERIFY(WB_MAX_EXECUTION_CONTEXTS == 15, ResourceIdIsNotWhatExpected_2);
    WB_STATIC_VERIFY(WB_MAX_RESOURCE_INSTANCES == 15, ResourceIdIsNotWhatExpected_3);

    ResourceIdSerializationBuffer binaryValue;
    if (mPosition + sizeof(binaryValue) > mpBuffer->header.messageLength)
    {
        return false;
    }

    memcpy(binaryValue, &mpBuffer->data[mPosition], sizeof(binaryValue));
    mPosition += sizeof(binaryValue);

    // Check for invalid IDs
    const LocalResourceId localResourceId = (static_cast<LocalResourceId>(binaryValue[1]) << 8) | binaryValue[2];
    if (localResourceId == ID_INVALID_LOCAL_RESOURCE)
    {
        rResourceId = ID_INVALID_RESOURCE;
    }
    else
    {
        rResourceId = ResourceId(
            static_cast<ExecutionContextId>(binaryValue[0] & 0xF),
            whiteboardId,
            localResourceId,
            static_cast<ResourceInstanceId>(binaryValue[0] >> 4));
    }

    return true;
}

bool Deserializer::deserializeClientId(ClientId& rClientId, WhiteboardId whiteboardId)
{
    // Can't really verify structure of client ID. Do next best thing.
    WB_STATIC_VERIFY(sizeof(rClientId.localClientId) == 2, ClientIdIsNotWhatExpected_1);
    WB_STATIC_VERIFY(WB_MAX_EXECUTION_CONTEXTS == 15, ClientIdIsNotWhatExpected_2);

    ClientIdSerializationBuffer binaryValue;
    if (mPosition + sizeof(binaryValue) > mpBuffer->header.messageLength)
    {
        return false;
    }

    memcpy(binaryValue, &mpBuffer->data[mPosition], sizeof(binaryValue));
    mPosition += sizeof(binaryValue);

    // Check for invalid IDs
    const LocalClientId localClientId = (static_cast<LocalClientId>(binaryValue[1]) << 8) | binaryValue[2];
    if (localClientId == ID_INVALID_LOCAL_CLIENT)
    {
        rClientId = ID_INVALID_CLIENT;
    }
    else
    {
        rClientId = ClientId(
            static_cast<ExecutionContextId>(binaryValue[0] & 0xF),
            whiteboardId,
            localClientId);

        rClientId.nonCriticalSubscription = binaryValue[0] >> 7;
        if (whiteboardId == LOCAL_WHITEBOARD_ID)
        {
            rClientId.typeChecked = binaryValue[0] >> 6;
        }
    }

    return true;
}

bool Deserializer::deserializeValue(Value& rValue)
{
    size_t numberOfDeserializedBytes;
    if (!deserializeValue(rValue, &mpBuffer->data[mPosition], mpBuffer->header.messageLength - mPosition, numberOfDeserializedBytes))
    {
        return false;
    }

    mPosition += numberOfDeserializedBytes;
    return true;
}

bool Deserializer::check() const
{
    return mPosition == mpBuffer->header.messageLength;
}

} // protocol_v9
} // namespace whiteboard
