#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/containers/Pool.h"

namespace whiteboard
{

/**
* Pool of typed data.
* The user must ensure thread safety.
*
* @tparam KEY_TYPE Integer type key for indexing the pool
* @tparam DATA_TYPE Type of the items in the pool
*/
template <typename KEY_TYPE, typename DATA_TYPE>
class TypedPool FINAL : public IDynamicallyAllocatable
{
public:
    /** Type of the key */
    typedef KEY_TYPE KeyType;

    /** Value used to indicate invalid keys */
    static const KeyType INVALID_ID = static_cast<KeyType>(-1);

    /** Type of the object */
    typedef DATA_TYPE DataType;

    /** Constructor
    *
    * @param name Name of the pool instance.
    * @param config Pool size config
    * @param rAllocator Allocator to use
    */
    inline TypedPool(const char* name, Config::PoolSize config, Pool::IAllocator& rAllocator)
        : mTypeAdapter(), mImplementation(name, config, rAllocator, mTypeAdapter)
    {
    }

    /** Destructor */
    inline ~TypedPool()
    {
    }

    /** Clears the pool */
    inline void clear()
    {
        mImplementation.clear();
    }

    /**
    * Try to allocate a new entry from the pool.
    *
    * @param timeoutMs Timeout for allocation in milliseconds. Only useful when using some blocking allocators. Ignored in others.
    * @return KeyType Id which is the index within the array. INVALID_ID if object could not be allocated.
    */
    inline KeyType allocate(size_t timeoutMs = 0)
    {
        return static_cast<KeyType>(mImplementation.allocate(timeoutMs));
    }

    /**
    * Adds object to the pool. Variant for pointers where it is inconvenient to require
    * reference to a pointer here.
    *
    * @param rValue Object that should be added
    * @param timeoutMs Timeout for allocation in milliseconds. Only useful when using some blocking allocators. Ignored in others.
    * @return Key that points to the added object. INVALID_ID if object could not be added.
    */
    inline KeyType add(typename ByRef<DataType>::type rValue, size_t timeoutMs = 0)
    {
        return mImplementation.add(&rValue, timeoutMs);
    }

    /**
    * Free an allocated entry.
    *
    * @param id Id of the entry.
    */
    inline void deallocate(KeyType id)
    {
        mImplementation.deallocate(static_cast<Pool::KeyType>(id));
    }

    /**
    * Checks whether given ID is allocated.
    *
    * @param id Id of the allocation.
    * @return A value indicating whether the ID is allocated
    */
    inline bool isAllocated(KeyType id) const
    {
        return mImplementation.isAllocated(static_cast<Pool::KeyType>(id));
    }

    /**
    * Operator to index the pool with the allocated id.
    *
    * @param id Id of the object to be accessed.
    * @return Pointer to the object
    */
    inline DataType& operator[](KeyType id)
    {
        return *static_cast<DataType*>(mImplementation[static_cast<Pool::KeyType>(id)]);
    }

    /**
    * Operator to index the pool with the allocated id.
    *
    * @param id Id of the object to be accessed.
    * @return Pointer to the object
    */
    inline const DataType& operator[](KeyType id) const
    {
        return *static_cast<const DataType*>(mImplementation[static_cast<Pool::KeyType>(id)]);
    }

    /**
    * Gets object from the registry with given key.
    *
    * @param key Key that indicates the object that should be retrieved
    * @param pValue On output contains pointer to the value
    * @return A value indicating whether the value was found and retrieved
    */
    inline bool tryGetValue(KeyType id, DataType& rValue)
    {
        Pool::UntypedData* pUntypedValue;
        if (!mImplementation.tryGetValue(static_cast<Pool::KeyType>(id), pUntypedValue))
        {
            return false;
        }

        rValue = *static_cast<const DataType*>(pUntypedValue);
        return true;
    }

    /**
    * Gets key that points to given object.
    *
    * @param pValue Pointer to object that is allocated from the pool
    * @return A key that indicates location of the the object
    */
    inline KeyType getKey(const DataType& rValue) const
    {
        return static_cast<KeyType>(mImplementation.getKey(&rValue));
    }

    /**
    *	Gets the name given to the pool instance.
    *
    *	@return Pointer to the name of the instance.
    */
    inline const char* getName() const
    {
        return mImplementation.getName();
    }

    /**
    *	Gets number of used items
    *
    *	@return Number of used items.
    */
    inline KeyType getNumberOfItems() const
    {
        return static_cast<KeyType>(mImplementation.getNumberOfItems());
    }

    /**
    *	Gets maximum number of items
    *
    *	@return Maximum number of items.
    */
    inline KeyType getMaximumNumberOfItems() const
    {
        return static_cast<KeyType>(mImplementation.getMaximumNumberOfItems());
    }

    /** Iterator class for iterating allocated items */
    class Iterator
    {
    public:
        /** Constructor
        *
        * @param rImplementation Untyped iterator implementation
        */
        inline Iterator(const Pool::Iterator& rImplementation)
            : mImplementation(rImplementation)
        {
        }

        /** Checks whether the iterator is still valid and there is more data to be read */
        inline bool hasMore() const
        {
            return mImplementation.hasMore();
        }

        /** Gets the key the iterator is pointing at
        *
        * @return key the iterator is pointing at
        */
        inline KeyType getKey() const
        {
            return static_cast<KeyType>(mImplementation.getKey());
        }

        /** Gets the item the iterator is pointing at
        *
        * @return Pointer to current item or NULL if iterator is invalid
        */
        inline DataType& operator*()
        {
            return *static_cast<DataType*>(*mImplementation);
        }

        /** Gets the item the iterator is pointing at
        *
        * @return Pointer to currentitem or NULL if iterator is invalid
        */
        inline const DataType& operator*() const
        {
            return *static_cast<const DataType*>(*mImplementation);
        }

        /** Prefix increment operator for advancing the iterator
        *
        * return Reference to this iterator
        */
        inline Iterator& operator++()
        {
            ++mImplementation;
            return *this;
        }

    private:
        /** Actual untyped implementation */
        Pool::Iterator mImplementation;
    };

    /** Gets new iterator instance
    *
    * @return New iterator that points to first allocated item
    */
    inline Iterator begin()
    {
        return Iterator(mImplementation.begin());
    }

    /** Gets new iterator instance
    *
    * @return New iterator that points to first allocated item
    */
    inline const Iterator begin() const
    {
        return Iterator(mImplementation.begin());
    }

private:
    /** Type adapter */
    Pool::TypeAdapter<DataType> mTypeAdapter;
    /** Actual untyped pool implementation */
    Pool mImplementation;
};

} // namespace whiteboard
