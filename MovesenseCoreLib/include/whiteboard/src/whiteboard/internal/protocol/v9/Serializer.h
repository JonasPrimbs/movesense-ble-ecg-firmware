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

/** Buffer data Serializer */
class Serializer FINAL
{
private:
    /** Prevent use of default constructor */
    inline Serializer() DELETED; 

public:

    /** Constructor
    *
    * @param pBuffer Pointer to owning buffer instance
    * @param isReceiverDataType Is the serializer serializing internal or external messages
    */
    inline Serializer(DataMessage* pBuffer, bool isReceiverDataType);

    /** Destructor */
    inline ~Serializer();

    /** Skips given number of bytes
    */
    inline void skip(size_t numberOfBytes);

    /** Serializes string to the buffer
    *
    * @param length Length of the string
    * @param string String to serialize
    */
    template <typename LengthType> inline void serializeString(LengthType length, const char* string);

    /** Serializes request result to the buffer
    *
    * @param resultCode Result of request
    */
    inline void serializeResult(Result resultCode);

    /** Serializes resource ID to the buffer
    *
    * @param resourceId Id of the resource
    */
    inline void serializeResourceId(ResourceId resourceId);

    /** Serializes client ID to the buffer
    *
    * @param clientId Id of the client
    */
    inline void serializeClientId(ClientId clientId);

    /** Calculates serialization length of given value
    *
    * @param rValue Value which serialization length should be calculated
    * @param bufferOffset Serialization buffer offset
    * @return Serialization length
    */
    static size_t getSerializationLength(const Value& rValue, size_t bufferOffset);
            
    /** Serializes whiteboard value to the buffer
    *
    * @param rValue Value that should be serialized
    */
    inline void serializeValue(const Value& rValue);

    /** Serializes value to given buffer
    *
    * @param rValue Value that should be serialized
    * @param pBuffer Buffer where data should be written
    * @param maxLength Maximum length of the buffer
    * @param isReceiverDataType Is the message in receiver format (true)
    * @return Number of bytes serialized
    */
    static size_t serializeValue(const Value& rValue, void* pBuffer, size_t maxLength, bool isReceiverDataType);

    /** Calculates serialization length of given parameter list
    *
    * @param rParameterList Parameter list which serialization length should be calculated
    * @param bufferOffset Serialization buffer offset
    * @return Serialization length
    */
    static size_t getSerializationLength(const ParameterList& rParameterList, size_t bufferOffset);

    /** Serializes whiteboard parameter list to the buffer
    *
    * @param rParameterList Parameter list that should be serialized
    */
    void serializeParameterList(const ParameterList& rParameterList);

    /** Checks that all data has been serialized */
    inline void check() const;

    /** Boolean operator to indicate the validity of the serializer instance */
    inline operator bool() const { return NULL != mpBuffer; }

private:
    /** Owning buffer instance */
    DataMessage* mpBuffer;

    /** Current Serialization position in the buffer */
    size_t mPosition;

    /** Is message to be serialized internal or external */
    bool mIsInternal;
};

Serializer::Serializer(DataMessage* pBuffer, bool isInternal)
    : mpBuffer(pBuffer), mPosition(0), mIsInternal(isInternal)
{
}

Serializer::~Serializer()
{
}

void Serializer::skip(size_t numberOfBytes)
{
    WB_ASSERT(mPosition + numberOfBytes <= mpBuffer->header.messageLength);
    mPosition += numberOfBytes;
}

template <typename LengthType> void Serializer::serializeString(LengthType length, const char* string)
{
    WB_ASSERT(mPosition + sizeof(LengthType) + length <= mpBuffer->header.messageLength);
    memcpy(&mpBuffer->data[mPosition], &length, sizeof(LengthType));
    mPosition += sizeof(LengthType);
    memcpy(&mpBuffer->data[mPosition], string, length);
    mPosition += length;
}

void Serializer::serializeResult(Result resultCode)
{
    WB_ASSERT(mPosition + sizeof(uint16) <= mpBuffer->header.messageLength);
    uint16 binaryValue = static_cast<uint16>(resultCode);
    memcpy(&mpBuffer->data[mPosition], &binaryValue, sizeof(uint16));
    mPosition += sizeof(uint16);
}

void Serializer::serializeResourceId(ResourceId resourceId)
{
    // Can't really verify that resource instance and execution context are located in same byte. Do next best thing.
    WB_STATIC_VERIFY(sizeof(resourceId.localResourceId) == 2, ResourceIdIsNotWhatExpected_1);
    WB_STATIC_VERIFY(WB_MAX_EXECUTION_CONTEXTS == 15, ResourceIdIsNotWhatExpected_2);
    WB_STATIC_VERIFY(WB_MAX_RESOURCE_INSTANCES == 15, ResourceIdIsNotWhatExpected_3);

    ResourceIdSerializationBuffer binaryValue = {
        static_cast<uint8>(static_cast<uint8>(resourceId.executionContextId) | static_cast<uint8>(resourceId.instanceId << 4)),
        static_cast<uint8>(resourceId.localResourceId >> 8),
        static_cast<uint8>(resourceId.localResourceId & 0xff)};

    WB_ASSERT(mPosition + sizeof(binaryValue) <= mpBuffer->header.messageLength);
    memcpy(&mpBuffer->data[mPosition], binaryValue, sizeof(binaryValue));
    mPosition += sizeof(binaryValue);
}

void Serializer::serializeClientId(ClientId clientId)
{
    // Can't really verify structure of client ID. Do next best thing.
    WB_STATIC_VERIFY(sizeof(clientId.localClientId) == 2, ClientIdIsNotWhatExpected_1);
    WB_STATIC_VERIFY(WB_MAX_EXECUTION_CONTEXTS == 15, ClientIdIsNotWhatExpected_2);

    ClientIdSerializationBuffer binaryValue = {clientId.executionContextId,
                                               static_cast<uint8>(clientId.localClientId >> 8),
                                               static_cast<uint8>(clientId.localClientId & 0xff)};

    WB_ASSERT(mPosition + sizeof(binaryValue) <= mpBuffer->header.messageLength);
    memcpy(&mpBuffer->data[mPosition], binaryValue, sizeof(binaryValue));
    mPosition += sizeof(binaryValue);
}

void Serializer::serializeValue(const Value& rValue)
{
    size_t serializationLength = serializeValue(
        rValue, &mpBuffer->data[mPosition], mpBuffer->header.messageLength - mPosition, mIsInternal);
    mPosition += serializationLength;
}

void Serializer::check() const
{
    WB_ASSERT(mPosition == mpBuffer->header.messageLength);
}

} // protocol_v9
} // namespace whiteboard
