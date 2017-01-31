#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/containers/TypedPool.h"
#include "whiteboard/containers/BitmapPoolAllocator.h"

namespace whiteboard
{

// Forward declarations
struct ByteStream;

/** Data type for storing ongoing stream information */
struct StreamData
{
    StreamData()
        : firstRequestId(ID_INVALID_REQUEST),
          requestId(ID_INVALID_REQUEST),
          timeoutMs(0),
        pStream(NULL)
    {
    }

    RequestId firstRequestId; // actual client's stream request ID
    RequestId requestId;      // incrementing streaming block request ID
    size_t timeoutMs;
    const ByteStream* pStream;
};

class StreamPool
{
public:
    /** Constructor */
    StreamPool(const Config::PoolSize size)
        : mAllocator(),
          mEntries("Streams", size, mAllocator)
    {
        WB_STATIC_VERIFY(Pool::INVALID_ID == ID_INVALID_STREAM, InvalidStreamIdMismatch);
    }

    /**
    * Try to allocate a new entry from the pool.
    *
    * @param timeoutMs Timeout for allocation in milliseconds. Only useful when using some blocking allocators. Ignored in others.
    * @return KeyType Id which is the index within the array. INVALID_ID if object could not be allocated.
    */
    inline StreamHandle allocate(size_t timeoutMs = 0)
    {
        return mEntries.allocate(timeoutMs);
    }

    /**
    * Free an allocated entry.
    *
    * @param id Id of the entry.
    */
    inline void deallocate(StreamHandle id)
    {
        mEntries.deallocate(id);
    }

    /**
    * Operator to index the pool with the allocated id.
    *
    * @param id Id of the object to be accessed.
    * @return Pointer to the object
    */
    inline StreamData& operator[](StreamHandle id)
    {
        return mEntries[id];
    }

private:
    /** Allocator */
    BitmapPoolAllocator mAllocator;

    /** Pool implementation */
    typedef TypedPool<StreamHandle, StreamData> Pool;
    Pool mEntries;
};

} // namespace whiteboard
