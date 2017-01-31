#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/containers/Pool.h"

namespace whiteboard
{

/**
Pool allocator that uses a free list to keep track of allocated/free items.
*/
class FreeListPoolAllocator FINAL : public Pool::IAllocator
{
public:
    /** Helper typedefs for a bit cleaner code */
    typedef Pool::KeyType KeyType;

    /** Constructor */
    FreeListPoolAllocator()
        : mpFreeNodes(NULL), mFirstFree(Pool::INVALID_ID)
    {
    }

    /** Destructor */
    ~FreeListPoolAllocator()
    {
        delete[] mpFreeNodes;
    }

    /**
    * Allocates a new entry from the pool.
    *
    * @return KeyType Id which is the index within the array or INVALID_ID if pool is full
    */
    KeyType allocate(Pool&, size_t) OVERRIDE
    {
        if (mFirstFree == Pool::INVALID_ID)
        {
            return mFirstFree;
        }

        KeyType id = mFirstFree;
        mFirstFree = mpFreeNodes[id];
#ifndef NDEBUG
        // Clear allocated to make debugging easier
        mpFreeNodes[id] = Pool::INVALID_ID;
#endif
        return id;
    }

    /**
    * Frees an allocated entry.
    *
    * @param id Id of the entry
    */
    void deallocate(Pool&, KeyType id) OVERRIDE
    {
        WB_DEBUG_ASSERT(mpFreeNodes[id] == Pool::INVALID_ID);
        mpFreeNodes[id] = mFirstFree;
        mFirstFree = id;
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

        // Allocate new array
        KeyType* pTemp = new KeyType[newSize];
        if (pTemp == NULL)
        {
            return false;
        }

        // Copy existing
        if (mpFreeNodes != NULL)
        {
            memcpy(pTemp, mpFreeNodes, currentSize * sizeof(KeyType));
            delete[] mpFreeNodes;
        }

        mpFreeNodes = pTemp;

        while (currentSize < newSize)
        {
            mpFreeNodes[currentSize] = mFirstFree;
            mFirstFree = currentSize++;
        }

        return true;
    }

private:
    /** Free list array */
    KeyType* mpFreeNodes;

    /** Index of first free node */
    KeyType mFirstFree;
};

} // namespace whiteboard
