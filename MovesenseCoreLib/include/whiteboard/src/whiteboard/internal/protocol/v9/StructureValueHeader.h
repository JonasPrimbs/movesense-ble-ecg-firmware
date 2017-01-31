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

WB_PACK_BEGIN()

/** Additional structure information. Byte based data access is
* used to avoid alignment problems.
*/
struct WB_PACKED StructureValueHeader
{
    /** Least significant byte of serializationLength */
    uint8 serializationLengthLSB;

    /** Most significant byte of serializationLength */
    uint8 serializationLengthMSB : 1;

    /** Structure alignment*/
    uint8 alignment : 3;

    /** A value indicating whether data type is receiver's data type */
    uint8 isReceiverDataType : 1;

    /** A value indicating whether data type is sender's data type */
    uint8 isSenderDataType : 1;

    /** Reserved for future use */
    uint8 reserved : 2;

    /** @param alignment 1-128
    @return alignment bits to be stored to structure value header */
    inline static uint8 toAlignmentBitField(uint8 alignment)
    {
        switch (alignment)
        {
        case 1: return 0;
        case 2: return 1;
        case 4: return 2;
        case 8: return 3;
        case 16: return 4;
        case 32: return 5;
        case 64: return 6;
        case 128: return 7;
        default:
            WB_ASSERT(false);
            return 0;
        }
    }

    /** @param alignmentBits from StructureValueHeader
    @return alignment 1-128*/
    inline static uint8 fromAlignmentBitField(uint8 alignmentBits)
    {
        switch (alignmentBits)
        {
        case 0: return 1;
        case 1: return 2;
        case 2: return 4;
        case 3: return 8;
        case 4: return 16;
        case 5: return 32;
        case 6: return 64;
        case 7: return 128;
        default:
            WB_ASSERT(false);
            return 1;
        }
    }

    /**
    * Writes structure value header to given buffer, buffer must have sizeof(StructureValueHeader) worth of space!
    * 
    * @param pBuffer Pointer to buffer where the header should be written
    * @param dataSerializationLength Serialization length of the structure data (excluding this header)
    * @param alignment Structure alignment
    * @param isReceiverDataType A value indicating whether the structure is receiver's data type
    * @param isSenderDataType A value indicating whether the structure is sender's data type
    */
    static inline void writeHeader(
        void* pBuffer,
        size_t dataSerializationLength,
        uint8 alignment,
        bool isReceiverDataType,
        bool isSenderDataType)
    {
        StructureValueHeader* pHeader = reinterpret_cast<StructureValueHeader*>(pBuffer);
        WB_ASSERT(dataSerializationLength <= 0xffff);
        pHeader->serializationLengthLSB = static_cast<uint8>(dataSerializationLength & 0xff);
        pHeader->serializationLengthMSB = static_cast<uint8>(dataSerializationLength >> 8);
        pHeader->isSenderDataType = isSenderDataType ? 1 : 0;
        pHeader->isReceiverDataType = isReceiverDataType ? 1 : 0;
        pHeader->alignment = StructureValueHeader::toAlignmentBitField(alignment);
        pHeader->reserved = 0;
    }

    /** Reads header information from the buffer
    *
    * @param pBuffer Source buffer
    * @param bufferLength Length of the buffer
    * @param rDataSerializationLength On output contains serialization length of structure data (excluding this header)
    * @param rAlignment On output contains serialized structure alingment
    * @param rIsReceiverDataType On output contains a value indicating whether structure uses receiver data type
    * @param rIsSenderDataType On output contains a value indicating whether structure uses sender data type
    * @return A value indicating whether the deserialized header was valid and buffer is big enough to hold structure data
    */
    static inline bool readHeader(
        const void* pBuffer,
        size_t bufferLength,
        size_t& rDataSerializationLength,
        uint8& rAlignment,
        bool& rIsReceiverDataType,
        bool& rIsSenderDataType)
    {
        if (bufferLength < sizeof(StructureValueHeader))
        {
            return false;
        }

        const StructureValueHeader* pHeader = static_cast<const StructureValueHeader*>(pBuffer);
        rDataSerializationLength = (static_cast<size_t>(pHeader->serializationLengthLSB) |
            (static_cast<size_t>(pHeader->serializationLengthMSB) << 8));
        rAlignment = StructureValueHeader::fromAlignmentBitField(pHeader->alignment);
        rIsReceiverDataType = pHeader->isReceiverDataType != 0;
        rIsSenderDataType = pHeader->isSenderDataType != 0;
        return bufferLength >= sizeof(StructureValueHeader) + rDataSerializationLength;
    }
};

WB_PACK_END()

} // protocol_v9
} // namespace whiteboard
