#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/comm/Buffer.h"
#include "whiteboard/comm/IBufferAllocator.h"

namespace whiteboard
{

// Forward declarations
class BufferPool;
class WhiteboardCommunication;

/** Buffer allocator that uses this pool */
class WB_API BufferPoolAllocator FINAL : public IBufferAllocator
{
public:
    /** Constructor 
     *
     * @param payloadOffset Number of bytes to reserve for lower level message header
     * @param pWhiteboardCommunication Optional WhiteboardCommunication instance to use. Used for unit testing.
     */
    BufferPoolAllocator(uint16 payloadOffset, WhiteboardCommunication* pWhiteboardCommunication = NULL);

    /** Destructor */
    virtual ~BufferPoolAllocator() {}

    /** Gets length of the header needed for lower level protocol and offset where whiteboard
    * protocol data can start */
    inline size_t getPayloadOffset() const OVERRIDE;

    /** Allocates buffer from the driver. Function should block
    * until a buffer is available.
    *
    * @param pool Type of the pool where the buffer should allocated from
    * @param payloadLength Number of bytes to allocate for the payload
    * @param timeoutMs Timeout in milliseconds
    * @return Pointer to allocated buffer. Allocation should always succeed.
    */
    Buffer* allocateBuffer(
        IBufferAllocator::PoolType pool, uint16 payloadLength, size_t timeoutMs = WB_INFINITE) OVERRIDE;

    /** Frees the buffer
    *
    * @param pBuffer Buffer that should be freed
    */
    void freeBuffer(Buffer* pBuffer) OVERRIDE;

#ifndef WB_UNITTEST_BUILD
    /** Template helper to retrieve pool allocator instance that is shared
    * accross application
    */
    template <size_t PAYLOAD_OFFSET>
    inline static BufferPoolAllocator& getInstance();
#endif

private:
    /** Payload offset */
    uint16 mPayloadOffset;

    /** Buffer pool instance to use */
    BufferPool& mrBufferPool;
};

inline size_t BufferPoolAllocator::getPayloadOffset() const 
{ 
    return mPayloadOffset;
}

#ifndef WB_UNITTEST_BUILD
template <size_t PAYLOAD_OFFSET>
inline BufferPoolAllocator& BufferPoolAllocator::getInstance()
{
    static BufferPoolAllocator instance(PAYLOAD_OFFSET);
    return instance;
}
#endif

} // namespace whiteboard
