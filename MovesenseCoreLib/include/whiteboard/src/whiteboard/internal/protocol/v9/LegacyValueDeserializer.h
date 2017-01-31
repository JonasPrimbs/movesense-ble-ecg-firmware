#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/internal/protocol/ILegacyValueDeserializer.h"
#include "whiteboard/internal/protocol/v9/Deserializer.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Class that implements interface for value deserilization */
class LegacyValueDeserializer FINAL : public ILegacyValueDeserializer
{
public:
    /** Destructor */
    virtual ~LegacyValueDeserializer() {}

    /** Deserializes a value from given buffer
    *
    * @param pBuffer Buffer where data should be read
    * @param maxLength Maximum length of the buffer
    * @param rNumberOfDeserializedBytes Number of deserialized bytes
    * @return New value instance
    */
    Value deserializeValue(const void* pBuffer, size_t maxLength, size_t& rNumberOfDeserializedBytes) const OVERRIDE
    {
        Value result;
        if (!Deserializer::deserializeValue(result, pBuffer, maxLength, rNumberOfDeserializedBytes))
        {
            WB_ASSERT(false);
        }

        return result;
    }
};

} // namespace protocol_v9
} // namespace whiteboard
