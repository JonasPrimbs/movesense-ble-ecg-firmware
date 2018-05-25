#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#ifdef WB_HAVE_DEPRECATED_BYTE_STREAM

#include "whiteboard/integration/port.h"
#include "whiteboard/Identifiers.h"


namespace whiteboard
{

struct StreamBase
{
public:
    static const uint8 STATUS_RESERVED = 1 << 0;
    static const uint8 STATUS_RESERIALIZATION = 1 << 1;

    /**
        Get re-serialization status.
        @return True if re-serializing (not the initial serialization from client's buffer).
    */
    inline bool reserializationStatus() const { return ((statusMask & STATUS_RESERIALIZATION) != 0); }

    /**
        Set re-serialization status.
        @param status True if not the first serialization of a message.
    */
    inline void setReserializationStatus(bool status) const
    {
        if (status)
        {
            statusMask |= STATUS_RESERIALIZATION;
        }
        else
        {
            statusMask &= ~STATUS_RESERIALIZATION;
        }
    }

protected:
    /// streaming status masks used by the system
    mutable uint8 statusMask;
};

#define MAX_STREAM_COMM_BLOCK (WB_MAX_MESSAGE_LENGTH - 40) // minus header + alignment waste
#define MAX_STREAM_DATA_BLOCK (MAX_STREAM_COMM_BLOCK - sizeof(ByteStream))

WB_PACK_BEGIN()
/**
    Simple byte stream type for streaming over comm routes.
    @note Length is stored in 8-bit members to avoid alignment problems during (de)serialization
*/
struct WB_PACKED ByteStream : public StreamBase
{
private:
    /** Prevent use of default constructor */
    ByteStream() DELETED;

    /** Prevent use of copy constructor */
    ByteStream(const ByteStream&) DELETED;

    /** Prevent use of assigment operator */
    const ByteStream& operator=(const ByteStream&) DELETED;

public:
    /**
        Get the size of the base type without the buffer.
        @return the base type size.
    */
    inline static uint16 baseTypeSize()
    {
#if (__APPLE__ && __MACH__ || __GNUC__)
        ByteStream* dummy = (ByteStream*)NULL;
        return static_cast<uint16>(reinterpret_cast<size_t>(dummy->data) - reinterpret_cast<size_t>(dummy));
#else
        return static_cast<uint16>(WB_OFFSETOF(ByteStream, data));
#endif
    }

    /**
        Get the size of the base type of the instance.
        @return the base type size.
    */
    inline uint16 getBaseTypeSize() const { return baseTypeSize(); }

    /**
        Allocates new ByteStream instance
        @param numberOfDataBytes (Maximum) number of bytes to allocate
        @return New ByteStream structure instance pointer
     */
    inline static ByteStream* allocate(uint32 numberOfDataBytes)
    {
        ByteStream* pResult = static_cast<ByteStream*>(WbMemAlloc(baseTypeSize() + numberOfDataBytes));
        pResult->statusMask = 0;
        pResult->setLength(numberOfDataBytes);
        pResult->setTransmitted(0);
        pResult->setPayloadSize(0);
        return pResult;
    }

    /**
        Maps a ByteStream into a "window" buffer pointer (e.g. from stack)
        @param pBuffer Pointer to the buffer
        @param bufferSize Size of the given buffer in bytes
        @param length Byte length of the actual stream
        @param transmitted Amount of bytes already transmitted
        @return New ByteStream structure instance pointer
    */
    inline static ByteStream* mapIntoBuffer(void* pBuffer, uint32 bufferSize, uint32 length, uint32 transmitted)
    {
        ByteStream* pResult = static_cast<ByteStream*>(pBuffer);
        if ((NULL == pResult) || (bufferSize < (length + baseTypeSize())))
        {
            return NULL;
        }
        pResult->statusMask = 0;
        pResult->setLength(length);
        pResult->setTransmitted(transmitted);
        pResult->setPayloadSize(0);
        return pResult;
    }

    /**
        Deallocates ByteStream instance
        @param instance ByteStream structure instance to deallocate
    */
    inline static void deallocate(ByteStream* instance) { WbMemFree(instance); }

    /**
        Serializes part of the byte stream into the given buffer.
        @param pWriteCursor Pointer to the serialization buffer.
        @maxLen Maximum amount of bytes to serialize.
        @return The amount of bytes serialized.
    */
    inline size_t serialize(uint8* pWriteCursor, size_t maxLen) const
    {
        bool reSerializing = reserializationStatus();

        size_t pos = transmitted();
        size_t baseSize = baseTypeSize();
        size_t toSerialize = length() - pos; // remaining stream data
        maxLen -= baseSize;

        if (reSerializing)
        {
            toSerialize = payloadSize();
        }

        if (toSerialize > maxLen)
        {
            toSerialize = maxLen; // clamp to max buffer size
        }

        if (!reSerializing) // detected in reserialization after COMM receive
        {
            // initialize base data for transmit
            setTransmitted(static_cast<uint32>(pos + toSerialize));
            setPayloadSize(static_cast<uint16>(toSerialize));
            setReserializationStatus(true);
        }
        memcpy(pWriteCursor, this, baseSize); // copy base data
        pWriteCursor += baseSize;
        if (reSerializing)
        {
            memcpy(pWriteCursor, data, toSerialize); // copy stream data
        }
        else
        {
            memcpy(pWriteCursor, data + pos, toSerialize); // copy stream data
        }
        setReserializationStatus(false); // reset local serialization status
        return baseSize + toSerialize;
    }

    /**
        Gets the number of bytes.
        @return Number of bytes
    */
    inline uint32 length() const
    {
        return static_cast<uint32>((static_cast<uint32>(numberOfBytesHSB) << 16) | (static_cast<uint32>(numberOfBytesMSB) << 8) |
                                   numberOfBytesLSB);
    }

    /**
        Sets number of bytes.
        @return numberOfBytes Number of bytes
    */
    inline void setLength(uint32 bytes)
    {
        numberOfBytesLSB = static_cast<uint8>(bytes);
        numberOfBytesMSB = static_cast<uint8>(bytes >> 8);
        numberOfBytesHSB = static_cast<uint8>(bytes >> 16);
    }

    /**
        Gets the full size of the object base type + buffer size.
        @return The full size in bytes.
    */
    inline uint32 fullSize() const // TODO: larger sizes
    {
        return (length() + baseTypeSize());
    }

    /**
        Returns the number of bytes transmitted.
        @return Number of bytes.
    */
    inline uint32 transmitted() const
    {
        return static_cast<uint32>((static_cast<uint32>(transmittedHSB) << 16) | (static_cast<uint32>(transmittedMSB) << 8) |
                                   transmittedLSB);
    }

    /**
        Sets the number of bytes transmitted (used by the WB comm system).
        @param bytes The amount of bytes transmitted.
        */
    inline void setTransmitted(uint32 bytes) const
    {
        transmittedLSB = static_cast<uint8>(bytes);
        transmittedMSB = static_cast<uint8>(bytes >> 8);
        transmittedHSB = static_cast<uint8>(bytes >> 16);
    }

    /**
        Sets the number of stream bytes contained in the payload.
        @param bytes The amount of bytes stored.
    */
    inline void setPayloadSize(uint16 bytes) const
    {
        payloadSizeLSB = static_cast<uint8>(bytes);
        payloadSizeMSB = static_cast<uint8>(bytes >> 8);
    }

    /**
        Gets the number of stream bytes contained in the payload.
        @return bytes The amount of bytes stored.
    */
    inline uint16 payloadSize() const { return static_cast<uint16>((static_cast<uint32>(payloadSizeMSB) << 8) | payloadSizeLSB); }

    /**
        Gets the total (re)serialization length.
        @return The total byte length.
    */
    inline uint32 serializationLength() const
    {
        bool reSerializing = reserializationStatus();
        if (reSerializing)
        {
            return payloadSize() + baseTypeSize();
        }
        else
        {
            uint32 size = fullSize() - transmitted();
            static const uint32 max_size = MAX_STREAM_DATA_BLOCK;
            if (size > max_size)
            {
                size = max_size; // clamp within serialization buffer
            }
            return size;
        }
    }

private:
    /** Number of bytes (low 8-bits) */
    uint8 numberOfBytesLSB;

    /** Number of bytes (middle 8-bits) */
    uint8 numberOfBytesMSB;

    /** Number of bytes (high 8-bits) */
    uint8 numberOfBytesHSB;

    /** Number of bytes transmitted (low 8-bits) */
    mutable uint8 transmittedLSB;

    /** Number of bytes transmitted (middle 8-bits) */
    mutable uint8 transmittedMSB;

    /** Number of bytes transmitted (high 8-bits) */
    mutable uint8 transmittedHSB;

    /** Number of bytes serialized in the stream package (low 8 bits) */
    mutable uint8 payloadSizeLSB;

    /** Number of bytes serialized in the stream package (high 8 bits) */
    mutable uint8 payloadSizeMSB;

public:
    /// Actual stream data
    uint8 data[1]; // the real size of the data comes from ::allocate
};

WB_PACK_END()

} // namespace whiteboard

#endif
