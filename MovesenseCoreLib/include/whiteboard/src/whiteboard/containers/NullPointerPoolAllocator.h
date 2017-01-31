#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/containers/Pool.h"

namespace whiteboard
{

/**
* Pool allocator that allocates slots that have currently NULL value
*/
class NullPointerPoolAllocator FINAL : public Pool::IAllocator
{
public:
    /** Helper typedefs for a bit cleaner code */
    typedef Pool::KeyType KeyType;

    /** Static instance. This class has no members so it can be shared
    * between all users. */
    static NullPointerPoolAllocator instance;

public:
    /** Constructor */
    NullPointerPoolAllocator()
    {
    }

    /** Destructor */
    ~NullPointerPoolAllocator()
    {
    }

    /**
    * Allocates a new entry from the pool.
    *
    * @param rPool Pool from where the item should be allocated
    * @return KeyType Id which is the index within the array or INVALID_ID if pool is full
    */
    KeyType allocate(Pool& rPool, size_t) OVERRIDE
    {
        for (KeyType i = 0; i < rPool.mNumberOfAllocatedItems; ++i)
        {
            if (*reinterpret_cast<const void**>(rPool[i]) == NULL)
            {
                return i;
            }
        }

        return Pool::INVALID_ID;
    }

    /**
    * Frees an allocated entry.
    *
    * @param rPool Pool that ows the item
    * @param id Id of the entry
    */
    void deallocate(Pool& rPool, KeyType id) OVERRIDE
    {
        *reinterpret_cast<const void**>(rPool[id]) = NULL;
    }

    /**
    * Checks whether given ID is allocated.
    *
    * @param rPool Pool that ows the item
    * @param id Id of the allocated object.
    * @return A value indicating whether the ID is allocated
    */
    bool isAllocated(const Pool& rPool, KeyType id) const OVERRIDE
    {
        return *reinterpret_cast<const void* const*>(rPool[id]) != NULL;
    }

    /**
    * Increases size of the allocator data structures
    *
    * @return A value indicating wether the allocator data structures were correctly grown
    */
    bool grow(Pool&, KeyType, KeyType) OVERRIDE
    {
        return true;
    }
};

} // namespace whiteboard
