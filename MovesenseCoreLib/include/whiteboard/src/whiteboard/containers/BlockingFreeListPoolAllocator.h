#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/containers/Pool.h"

namespace whiteboard
{

/**
Pool allocator that uses an atomic free list to keep track of allocated/free items.
Allocator is also atomic and Allocation also implement
*/
class BlockingFreeListPoolAllocator FINAL : public Pool::IAllocator
{
public:
    /** Helper typedefs for a bit cleaner code */
    typedef Pool::KeyType KeyType;

    /** Constructor */
    BlockingFreeListPoolAllocator()
        : mSemaphoreHandle(WB_INVALID_SEMAPHORE), mpFreeNodes(NULL), mFirstFree(Pool::INVALID_ID)
    {
    }

    /** Destructor */
    ~BlockingFreeListPoolAllocator()
    {
        if (mSemaphoreHandle != WB_INVALID_SEMAPHORE)
        {
            WbSemaphoreDelete(mSemaphoreHandle);
        }

        WbMemFree(mpFreeNodes);
    }

    /**
    * Allocates a new entry from the pool.
    *
    * @param timeoutMs Timeout for allocation in milliseconds. Only useful when using some blocking allocators. Ignored in others.
    * @return KeyType Id which is the index within the array or INVALID_ID if pool is full
    */
    KeyType allocate(Pool&, size_t timeoutMs) OVERRIDE
    {
        // Wait until there are free objects
        if (!WbSemaphoreTryWait(mSemaphoreHandle, timeoutMs))
        {
            return Pool::INVALID_ID;
        }

        // Atomically pop free node from linked list
        KeyType originalFirstFree;
        do
        {
            originalFirstFree = static_cast<KeyType>(mFirstFree);
            WB_ASSERT(originalFirstFree != Pool::INVALID_ID);
        } while (!WbInterlockedCompareAndExchange32(&mFirstFree, mpFreeNodes[originalFirstFree], originalFirstFree));

#ifndef NDEBUG
        // Clear allocated to make debugging easier
        mpFreeNodes[originalFirstFree] = Pool::INVALID_ID;
#endif
        return originalFirstFree;
    }

    /**
    * Frees an allocated entry.
    *
    * @param id Id of the entry
    */
    void deallocate(Pool&, KeyType id) OVERRIDE
    {
        WB_DEBUG_ASSERT(mpFreeNodes[id] == Pool::INVALID_ID);

        // Atomically push free node to linked list
        KeyType originalFirstFree;
        do
        {
            originalFirstFree = static_cast<KeyType>(mFirstFree);
            mpFreeNodes[id] = originalFirstFree;
        } while (!WbInterlockedCompareAndExchange32(&mFirstFree, id, originalFirstFree));

        // Notify that a new buffer is available
        WbSemaphoreRelease(mSemaphoreHandle);
    }

    /**
    * Checks whether given ID is allocated.
    *
    * @param id Id of the allocated object.
    * @return A value indicating whether the ID is allocated
    */
    bool isAllocated(const Pool&, KeyType id) const OVERRIDE
    {
        // This is slow, but should be used only in debug builds so we don't care.
        KeyType current = mFirstFree;
        while (current != Pool::INVALID_ID)
        {
            if (current == id)
            {
                return false;
            }

            current = mpFreeNodes[current];
        }

        return true;
    }

    /**
    * Increases size of the allocator data structures
    *
    * @param currentSize Current size of the pool
    * @param newSize New size of the pool
    * @return A value indicating wether the allocator data structures were correctly grown
    */
    bool grow(Pool&, KeyType currentSize, KeyType newSize) OVERRIDE
    {
        WB_DEBUG_ASSERT(newSize > currentSize);

        // Dynamic grow not supported
        if (mSemaphoreHandle != WB_INVALID_SEMAPHORE)
        {
            return false;
        }

        // Initialize semaphore that is used to block when pool is empty
        mSemaphoreHandle = WbSemaphoreCreate(newSize, newSize);
        WB_ASSERT(mSemaphoreHandle);

        // Allocate new array
        mpFreeNodes = reinterpret_cast<KeyType*>(WbMemAlloc(sizeof(KeyType)*newSize));
        if (mpFreeNodes == NULL)
        {
            return false;
        }

        while (currentSize < newSize)
        {
            mpFreeNodes[currentSize] = mFirstFree;
            mFirstFree = currentSize++;
        }

        return true;
    }

private:
    /** Semaphore that protects the available buffers */
    WbSemaphoreHandle mSemaphoreHandle;

    /** Free list array */
    KeyType* mpFreeNodes;

    /** Index of first free node. We could use KeyType(uint16), but we would need
     * another set of interlocked operations. */
    volatile int32 mFirstFree;
};

} // namespace whiteboard
