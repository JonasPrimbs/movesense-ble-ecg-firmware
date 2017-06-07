#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/integration/port.h"

namespace whiteboard
{

// Forward declarations
struct Buffer;

/** Interface that enables adapter specific buffer allocation.
 * Separate interface from CommAdapter because we need to be able
 * to deallocate buffers even after adapter has been removed
 */
class WB_API IBufferAllocator
{
public:
    /** Destructor */
    virtual ~IBufferAllocator() {}

    /** Pool. We have distinct pools for
     * send, receive and internal messages so that
     * they won't starve each other.
     */
    typedef enum
    {
        /** One of send message pools (used both by local and by communication messages). */
        SEND,

        /** One of internal communication message pools. */
        COMMUNICATION_INTERNAL,

        /** One of communication receive message pools. */
        COMMUNICATION_RECEIVE,
    } PoolType;

    /** Index of the pool. Note that actual allocator can have multiple pools
     * for single type and this index won't match the enumeration above */
    typedef uint8 PoolIndex;

    /** Gets length of the header needed for lower level protocol and offset where whiteboard
     * protocol data can start */
    virtual size_t getPayloadOffset() const = 0;

    /** Allocates buffer from the driver. Function should block
    * until a buffer is available.
    *
    * @param pool Type of the pool where the buffer should be allocated from
    * @param payloadLength Number of bytes to allocate for the payload
    * @param timeoutMs Timeout in milliseconds
    * @return Pointer to allocated buffer. Allocation should always succeed.
    */
    virtual Buffer* allocateBuffer(PoolType pool, uint16 payloadLength, size_t timeoutMs = static_cast<size_t>(-1)) = 0;

    /** Frees buffer that was allocated from the bus driver
    *
    * @param pBuffer Buffer that should be freed
    */
    virtual void freeBuffer(Buffer* pBuffer) = 0;
};

} // namespace whiteboard
