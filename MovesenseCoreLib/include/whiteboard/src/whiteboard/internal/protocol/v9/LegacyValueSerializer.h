#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "whiteboard/internal/protocol/ILegacyValueSerializer.h"
#include "whiteboard/internal/protocol/v9/Serializer.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Class that implements interface for value serilization */
class LegacyValueSerializer FINAL : public ILegacyValueSerializer
{
public:
    /** Destructor */
    virtual ~LegacyValueSerializer() {}

    /** Calculates serialization length of given value
    *
    * @param rValue Value which serialization length should be calculated
    * @param bufferOffset Serialization buffer offset
    * @return Serialization length
    */
    size_t getSerializationLength(const Value& rValue, size_t bufferOffset) const OVERRIDE
    {
        return Serializer::getSerializationLength(rValue, bufferOffset);
    }

    /** Serializes value to given buffer
    *
    * @param rValue Value that should be serialized
    * @param pBuffer Buffer where data should be written
    * @param maxLength Maximum length of the buffer
    * @param isReceiverDataType Is the message in receiver format (true)
    * @return Number of bytes serialized
    */
    size_t serializeValue(const Value& rValue, void* pBuffer, size_t maxLength, bool isReceiverDataType) const OVERRIDE
    {
        return Serializer::serializeValue(rValue, pBuffer, maxLength, isReceiverDataType);
    }
};

} // namespace protocol_v9
} // namespace whiteboard
