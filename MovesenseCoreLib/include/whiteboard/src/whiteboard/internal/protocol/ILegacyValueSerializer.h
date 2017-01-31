#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/Value.h"

namespace whiteboard
{

/** Interface for value serilization */
class ILegacyValueSerializer
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~ILegacyValueSerializer() {}

public:
    /** Calculates serialization length of given value
    *
    * @param rValue Value which serialization length should be calculated
    * @param bufferOffset Serialization buffer offset
    * @return Serialization length
    */
    virtual size_t getSerializationLength(const Value& rValue, size_t bufferOffset) const = 0;

    /** Serializes value to given buffer
    *
    * @param rValue Value that should be serialized
    * @param pBuffer Buffer where data should be written
    * @param maxLength Maximum length of the buffer
    * @param isReceiverDataType Is the message in receiver format (true)
    * @return Number of bytes serialized
    */
    virtual size_t serializeValue(const Value& rValue, void* pBuffer, size_t maxLength, bool isReceiverDataType) const = 0;
};

} // namespace whiteboard
