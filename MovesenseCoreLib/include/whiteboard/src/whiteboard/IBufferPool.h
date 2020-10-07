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
class WB_API IBufferPool
{
public:
    /** Destructor */
    virtual ~IBufferPool() {}

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

    /** Allocates a new buffer from buffer pool. Blocks calling
    * thread until a buffer is available.
    *
    * @param pool Type of the pool where the buffer should allocated from
    * @param payloadOffset Number of bytes to allocate for communication protocol headers
    * @param payloadLength Number of bytes to allocate for the payload
    * @param timeoutMs Timeout in milliseconds
    * @return Pointer to communication buffer or NULL if timeout elapsed before
    * buffer could be allocated
    */
    virtual Buffer* allocate(
        PoolType pool,
        uint16 payloadOffset,
        uint16 payloadLength,
        size_t timeoutMs = WB_INFINITE) = 0;

    /** Frees buffer back to buffer pool.
    *
    * @param pBuffer Pointer to communication buffer
    */
    virtual void free(Buffer* pBuffer) = 0;
};

} // namespace whiteboard
