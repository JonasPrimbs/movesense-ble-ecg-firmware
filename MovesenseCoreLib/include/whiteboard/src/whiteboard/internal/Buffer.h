#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/StructurePack.h"
#include "whiteboard/internal/IBufferAllocator.h"
#include "whiteboard/internal/protocol/Messages.h"

// Define these to ease buffer pool debugging (by default these are enabled in 
// non cpu & memory constrained builds)
#if defined(_WIN32) || defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
#define WB_EXTRA_BUFFER_DEBUG_DATA
#define WB_CLEAN_HEADERS_OF_FREED_BUFFERS
#endif

namespace whiteboard
{

/** Buffer header */
struct BufferHeader
{
#ifdef WB_EXTRA_BUFFER_DEBUG_DATA
    /** Thread that allocated the buffer */
    const char* threadName;
#endif

    /** Allocator of the buffer */
    IBufferAllocator* pAllocator;

    /** Index of the pool */
    IBufferAllocator::PoolIndex poolIndex;

    /** Payload offset */
    uint8 payloadOffset;

    /** Amount of payload data in the buffer */
    uint16 payloadLength;
};

/** Type of communication buffers (interface for buffer instances) */
struct Buffer
{
    union
    {
        /** Header */
        BufferHeader header;

        /** Align following data to qword boundary */
        WB_ALIGNED(uint64) aligner[(sizeof(BufferHeader) + (sizeof(uint64) - 1)) / sizeof(uint64)];
    };

    /** Data follows (aligned on qword boundary) */
    union
    {
        /** Header information (mostly to ease debugging) */
        DataMessageHeader messageHeader;

        /** Data buffer */
        uint8 data[1];
    };

    /** Deallocates buffer */
    inline void free()
    {
        WB_ASSERT(header.pAllocator != NULL);
        header.pAllocator->freeBuffer(this);
    }

    /** Gets data message header
    *
    * @return Data message header
    */
    inline DataMessageHeader& getDataMessage()
    {
        return *reinterpret_cast<DataMessageHeader*>(&data[header.payloadOffset]);
    }

    /** Gets data message header
     *
     * @return Data message header
     */
    inline const DataMessageHeader& getDataMessage() const
    {
        return *reinterpret_cast<const DataMessageHeader*>(&data[header.payloadOffset]);
    }
};

WB_STATIC_VERIFY(WB_OFFSETOF(Buffer, data) % WB_MESSAGE_ALIGNMENT == 0, BufferDataIsNotCorrectlyAligned);
WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(Buffer) == WB_MESSAGE_ALIGNMENT, BufferIsNotCorrectlyAligned);

/** Communication buffer instance with actual data allocation
 * TODO: Align using alignas when we can use C++11 */
template <size_t BUFFER_LENGTH> struct BufferInstance
{
    union
    {
        /** Header */
        BufferHeader header;

        /** Align following data to qword boundary */
        WB_ALIGNED(uint64) aligner[(sizeof(BufferHeader) + (sizeof(uint64) - 1)) / sizeof(uint64)];
    };

    /** Data (align to qword boundary, so that structure members align correctly) */
    union
    {
        /** Header information (mostly to ease debugging) */
        DataMessageHeader messageHeader;

        /** Data buffer */
        uint8 data[(BUFFER_LENGTH + (WB_MESSAGE_ALIGNMENT - 1)) / WB_MESSAGE_ALIGNMENT * WB_MESSAGE_ALIGNMENT];
    };
};

// Size should be divisible by eight so that array of buffers keep their aligment
WB_STATIC_VERIFY(sizeof(BufferInstance<1>) % WB_MESSAGE_ALIGNMENT == 0, BufferSizeWillBreakAlignment);
WB_STATIC_VERIFY(WB_OFFSETOF(BufferInstance<1>, data) % WB_MESSAGE_ALIGNMENT == 0, BufferDataIsNotCorrectlyAligned);
WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(BufferInstance<1>) == WB_MESSAGE_ALIGNMENT, BufferIsNotCorrectlyAligned);

} // namespace whiteboard
