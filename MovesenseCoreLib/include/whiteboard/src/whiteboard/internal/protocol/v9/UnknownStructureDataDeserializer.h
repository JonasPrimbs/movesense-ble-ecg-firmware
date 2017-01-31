#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/internal/protocol/Padding.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Class that handles data deserialization for unknown structures */
class UnknownStructureDataDeserializer
{
public:
    /** Constructor
    *
    * @param pBuffer Owning buffer instance
    * @param bufferLength Length of the buffer
    * @param padding before the structure in the buffer to align it properly
    */
    EXPLICIT UnknownStructureDataDeserializer(void* pBuffer, uint32 bufferLength, uint8 padding);

    /** Destructor */
    inline ~UnknownStructureDataDeserializer() {}

    /** Deserializes scalar value from the buffer
    *
    * @param offset Deserialization offset
    * @param rData On output contains the deserialized data
    * @return A value indicating whether the data was successfully deserialized
    */
    inline bool take(uint32& rOffset, bool& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, int8& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, int16& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, int32& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, int64& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, uint8& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, uint16& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, uint32& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, uint64& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, float& rData) { return deserialize(rOffset, rData); }
    inline bool take(uint32& rOffset, double& rData) { return deserialize(rOffset, rData); }

    inline bool take(uint32& rOffset, const char*& rData)
    {
        rData = reinterpret_cast<const char*>(static_cast<uint8*>(mBuffer) + rOffset);

        // Scan until end of string or end of buffer
        const char* pData = rData;
        while (true)
        {
            if (rOffset++ >= mBufferLength)
            {
                return false;
            }

            if (*pData++ == '\0')
            {
                return true;
            }
        }
    }

    /** Calculates new offset for given alignment
    *
    * @param rOffset Deserialization offset
    * @param alignment Data alignment
    * @return A value indicating whether the offset remained in valid
    *         data range
    */
    inline bool align(uint32& rOffset, uint8 alignment)
    {
        // Aling in respect to the current buffer alignment
        size_t bufferOffset = reinterpret_cast<size_t>(mBuffer) + rOffset;
        bufferOffset += padding(bufferOffset, alignment);
        rOffset = static_cast<uint32>(bufferOffset - reinterpret_cast<size_t>(mBuffer));
        return rOffset < mBufferLength;
    }

    /** @returns padding in bytes between the structure header and the structure in the current deserialization buffer
    to align the pointers within the structure properly */
    inline uint8 getPadding() const { return mPadding; };

private:
    /** Deserializes scalar value from the buffer
    *
    * @param rData On output contains the deserialized data
    * @param alignment Data alignment
    * @return A value indicating whether the data was successfully deserialized
    */
    template <typename Type> inline bool deserialize(uint32& rOffset, Type& rData)
    {
        uint8 alignment = sizeof(Type);
#ifdef WB_STRUCT_PACKING
        if (alignment > WB_STRUCT_PACKING)
        {
            alignment = WB_STRUCT_PACKING;
        }
#endif

        uint32 dataOffset = rOffset;
        if (!align(dataOffset, alignment))
        {
            return false;
        }

        uint32 newOffset = dataOffset + sizeof(Type);
        if (newOffset > mBufferLength)
        {
            return false;
        }

        rData = *reinterpret_cast<const Type*>(static_cast<uint8*>(mBuffer) + dataOffset);
        rOffset = newOffset;
        return true;
    }

private:
    /** The buffer */
    void* mBuffer;

    /** Length of the buffer */
    uint32 mBufferLength;

    /** Padding between the structure header and the structure in the current buffer to align the structure properly */
    uint8 mPadding;
};

} // protocol_v9
} // namespace whiteboard
