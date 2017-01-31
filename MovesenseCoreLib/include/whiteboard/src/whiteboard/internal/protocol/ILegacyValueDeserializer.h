#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/Value.h"

namespace whiteboard
{

/** Interface for value deserilization */
class ILegacyValueDeserializer
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~ILegacyValueDeserializer() {}

public:
    /** Deserializes a value from given buffer
    *
    * @param pBuffer Buffer where data should be read
    * @param maxLength Maximum length of the buffer
    * @param rNumberOfDeserializedBytes Number of deserialized bytes
    * @return New value instance
    */
    virtual Value deserializeValue(const void* pBuffer, size_t maxLength, size_t& rNumberOfDeserializedBytes) const = 0;
};

} // namespace whiteboard
