#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/StructurePack.h"
#include "whiteboard/comm/IBufferAllocator.h"

// Define these to ease buffer pool debugging (by default these are enabled in 
// non cpu & memory constrained builds)
#if defined(_WIN32) || defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
#define WB_EXTRA_BUFFER_DEBUG_DATA
#define WB_CLEAN_HEADERS_OF_FREED_BUFFERS
#endif

namespace whiteboard
{

struct DataMessageHeader;

/** Buffer header */
struct WB_API BufferHeader
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
struct WB_API Buffer
{
    union
    {
        /** Header */
        BufferHeader header;

        /** Align following data to qword boundary */
        WB_ALIGNED(uint64) aligner[(sizeof(BufferHeader) + (sizeof(uint64) - 1)) / sizeof(uint64)];
    };

    /** Data follows (aligned on qword boundary) */
	uint8 data[1];

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

} // namespace whiteboard
