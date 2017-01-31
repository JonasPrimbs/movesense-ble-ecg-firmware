#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/containers/Pool.h"
#include "whiteboard/internal/BitwiseOperations.h"

namespace whiteboard
{

/**
Pool allocator that uses a bit array to mark any pool item as allocated/free.
*/
class BitmapPoolAllocator FINAL : public Pool::IAllocator
{
public:
    /** Helper typedefs for a bit cleaner code */
    typedef Pool::KeyType KeyType;

    /** Constructor */
    BitmapPoolAllocator()
        : mpFree(NULL)
    {
    }

    /** Destructor */
    ~BitmapPoolAllocator()
    {
        if (mpFree)
        {
            WbMemFree(mpFree);
        }
    }

    /**
    * Allocates a new entry from the pool.
    *
    * @param rPool Pool from where the item should be allocated
    * @return KeyType Id which is the index within the array or INVALID_ID if pool is full
    */
    KeyType allocate(Pool& rPool, size_t) OVERRIDE
    {
        const KeyType numberOfBlocks = (rPool.mNumberOfAllocatedItems + 31) / 32;
        size_t bitId = BitArray::findFirstSetBit(mpFree, numberOfBlocks);
        if (bitId == static_cast<size_t>(-1))
        {
            return Pool::INVALID_ID;
        }

        KeyType id = static_cast<KeyType>(bitId);

        KeyType index = id >> 5;        // index in the word table
        uint32 mask = 1 << (id & 31);   // bit inside the word (reverse order)
        WB_DEBUG_ASSERT(0 != (mpFree[index] & mask));    // must be free (bit is set)
        mpFree[index] &= ~mask;         // clear bit to mark it as allocated

        return id;
    }

    /**
    * Frees an allocated entry.
    *
    * @param id Id of the entry
    */
    void deallocate(Pool&, KeyType id) OVERRIDE
    {
        KeyType index = id >> 5; // index in the word table
        uint32 mask = 1 << (id & 31);   // bit inside the word (reverse order)
        WB_DEBUG_ASSERT(0 == (mpFree[index] & mask));    // must be reserved (bit is not set)
        mpFree[index] |= mask;   // set bit to mark it as free
    }

    /**
    * Checks whether given ID is allocated.
    *
    * @param id Id of the allocated object.
    * @return A value indicating whether the ID is allocated
    */
    bool isAllocated(const Pool&, KeyType id) const OVERRIDE
    {
        return 0 == (mpFree[id >> 5] & (1 << (id & 31)));
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
        const uint32 allBits = static_cast<uint32>(-1);

        KeyType currentBlocks = (currentSize + 31) / 32;
        KeyType newBlocks = (newSize + 31) / 32;

        if (newBlocks > currentBlocks)
        {
            // Allocate new array
            uint32* pTemp = static_cast<uint32*>(WbMemAlloc(newBlocks * sizeof(uint32)));
            if (pTemp == NULL)
            {
                return false;
            }

            // Copy existing
            if (mpFree != NULL)
            {
                memcpy(pTemp, mpFree, currentBlocks * sizeof(uint32));
                WbMemFree(mpFree);
            }

            mpFree = pTemp;
            memset(&mpFree[currentBlocks], 0xFFu, (newBlocks - currentBlocks) * sizeof(uint32));    // set all bits to indicate free entries

            const KeyType lastIndex = newSize - 1;
            const uint32 mask = allBits >> (31 - (lastIndex & 31));
            mpFree[newBlocks - 1] &= mask;   // mask out unneeded bits from the last word

            newSize = currentBlocks * 32;
        }

        {
            KeyType newItemsInMiddleBlock = newSize - currentSize;
            if (newItemsInMiddleBlock > 0)
            {
                KeyType oldItemsInTheBlock = currentSize & 31;
                const uint32 mask = allBits >> (32 - newItemsInMiddleBlock) << oldItemsInTheBlock;
                mpFree[currentBlocks - 1] |= mask;  // sets bits to mark them as free
            }
        }

        return true;
    }

private:
    /** Bitmap array. Item is free if bit is set. */
    uint32* mpFree;
};

} // namespace whiteboard
