#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/Initialization.h"

WB_HEADER_CHECK_DEFINE(WB_HAVE_POOL_STATISTICS)

// Forward declarations
namespace WB_RES
{
struct PoolStats;
}

namespace whiteboard
{
/**
    Untyped pool for small arrays and dynamic allocations from them.
    Allocation and deallocation are thread safe when used with some
    blocking / atomic allocators. Access to items is thread safe if
    multiple threads do not try to access same item simultaneously.
    Dynamic pool size growing is not synchronized and pool needs
    additional level of synchronization if being used at the same time.
*/
class Pool FINAL
{
public:
    /** Type of the key */
    typedef uint16 KeyType;

    /** Value used to indicate invalid keys */
    static const KeyType INVALID_ID = static_cast<KeyType>(-1);

    /** Type of the object */
    typedef void UntypedData;

    /** Interface for allocator implementations */
    class IAllocator
    {
    public:
        /**
        * Allocates a new entry from the pool.
        *
        * @param rPool Pool from where the item should be allocated
        * @param timeoutMs Timeout for allocation in milliseconds. Only useful when using some blocking allocators. Ignored in others.
        * @return KeyType Id which is the index within the array or INVALID_ID if pool is full or timeout has expired
        */
        virtual KeyType allocate(Pool& rPool, size_t timeoutMs) = 0;

        /**
        * Frees an allocated entry.
        *
        * @param rPool Pool that ows the item
        * @param id Id of the entry to deallocate
        */
        virtual void deallocate(Pool& rPool, KeyType id) = 0;

        /**
        * Checks whether given ID is allocated.
        *
        * @param rPool Pool that ows the item
        * @param id Id of the allocated object.
        * @return A value indicating whether the ID is allocated
        */
        virtual bool isAllocated(const Pool& rPool, KeyType id) const = 0;

        /**
        * Increases size of the allocator data structures
        *
        * @param rPool Pool that ows the item
        * @param currentSize Current size of the pool
        * @param newSize New size of the pool
        * @return A value indicating wether the allocator data structures were correctly grown
        */
        virtual bool grow(Pool& rPool, KeyType currentSize, KeyType newSize) = 0;
    };

    /** Type adapter interface for pools */
    class ITypeAdapter
    {
    public:
        /** Size of an item */
        const uint16 mItemSize;

        /** Alignment of an item */
        const uint16 mItemAlignment;

        /** Constructor 
         *
         * @param itemSize Size of an item
         * @param itemAlignment Alignment of an item
         */
        ITypeAdapter(const uint16 itemSize, const uint16 itemAlignment);

        /** Performs placement new for given array of objects
        *
        * @param pArray Array that is target for placement new
        * @param numberOfItems Number of items in the array
        */
        virtual void placementNewArray(UntypedData* pArray, KeyType numberOfItems) const = 0;

        /** Performs placement destruct for given array of objects
        *
        * @param pArray Array that is target for placement destruct
        * @param numberOfItems Number of items in the array
        */
        virtual void placementDestructArray(UntypedData* pArray, KeyType numberOfItems) const = 0;

        /**
        * Assigns source object to target
        *
        * @param pSource Source object
        * @param pTarget Target object
        */
        virtual void assign(UntypedData* pSource, UntypedData* pTarget) const = 0;
    };

    /** Type adapter implementation */
    template <typename T, bool isClass>
    class TypeAdapterHelper : public ITypeAdapter
    {
    public:
        /** Constructor */
        TypeAdapterHelper()
            : ITypeAdapter(sizeof(T), WB_TYPE_ALIGNMENT(T))
        {
        }

        /** Performs placement new for given array of objects
        *
        * @param pArray Array that is target for placement new
        * @param numberOfItems Number of items in the array
        */
        void placementNewArray(UntypedData* pArray, KeyType numberOfItems) const OVERRIDE
        {
            new (pArray) T[numberOfItems];
        }

        /** Performs placement destruct for given array of objects
        *
        * @param pArray Array that is target for placement destruct
        * @param numberOfItems Number of items in the array
        */
        void placementDestructArray(UntypedData* pArray, KeyType numberOfItems) const OVERRIDE
        {
            T* pItem = static_cast<T*>(pArray);
            for (KeyType i = 0; i < numberOfItems; ++i, ++pItem)
            {
                (*pItem).~T();
            }
        }

        /**
        * Assigns source object to target
        *
        * @param pSource Source object
        * @param pTarget Target object
        */
        void assign(UntypedData* pSource, UntypedData* pTarget) const OVERRIDE
        {
            *static_cast<T*>(pTarget) = *static_cast<T*>(pSource);
        }
    };

    /** Type adapter implementation for POD types */
    template <typename T>
    class TypeAdapterHelper<T, false> : public ITypeAdapter
    {
    public:
        /** Constructor */
        TypeAdapterHelper()
            : ITypeAdapter(sizeof(T), WB_TYPE_ALIGNMENT(T))
        {
        }

        /** Performs placement new for given array of objects
        *
        * @param pArray Array that is target for placement new
        * @param numberOfItems Number of items in the array
        */
        void placementNewArray(UntypedData* pArray, KeyType numberOfItems) const OVERRIDE
        {
            // We could use placement new array here, but it wouldn't zero initialize
            // pointers even though it zero initialized all other POD types
            // Zero init manually.
            memset(pArray, 0, sizeof(T) * numberOfItems);
        }

        /** Performs placement destruct for given array of objects
        *
        * @param pArray Array that is target for placement destruct
        * @param numberOfItems Number of items in the array
        */
        void placementDestructArray(UntypedData* /*pArray*/, KeyType /*numberOfItems*/) const OVERRIDE
        {
        }

        /**
        * Assigns source object to target
        *
        * @param pSource Source object
        * @param pTarget Target object
        */
        void assign(UntypedData* pSource, UntypedData* pTarget) const OVERRIDE
        {
            *static_cast<T*>(pTarget) = *static_cast<T*>(pSource);
        }
    };

    template <typename T>
    struct TypeAdapter FINAL : public TypeAdapterHelper<T, IsClassOrUnion<T>::value >
    {
    };

    /** Constructor
    *
    * @param name Name of the pool instance.
    * @param config Pool size config
    * @param rAllocator Allocator to use
    * @param rTypeAdapter Type adapter to use
    */
    Pool(const char* name, Config::PoolSize config, IAllocator& rAllocator, const ITypeAdapter& rTypeAdapter);

    /** Destructor */
    ~Pool();

    /** Clears the pool */
    void clear();

    /**
    * Try to allocate a new entry from the pool.
    *
    * @param timeoutMs Timeout for allocation in milliseconds. Only useful when using some blocking allocators. Ignored in others.
    * @return KeyType Id which is the index within the array. INVALID_ID if object could not be allocated.
    */
    KeyType allocate(size_t timeoutMs = 0);

    /**
    * Adds object to the pool
    *
    * @param value Object that should be added
    * @param timeoutMs Timeout for allocation in milliseconds. Only useful when using some blocking allocators. Ignored in others.
    * @return Key that points to the added object. INVALID_ID if object could not be added.
    */
    KeyType add(UntypedData* value, size_t timeoutMs = 0);

    /**
    * Free an allocated entry.
    *
    * @param id Id of the entry.
    */
    void deallocate(KeyType id);

    /**
    * Checks whether given ID is allocated.
    *
    * @param id Id of the allocation.
    * @return A value indicating whether the ID is allocated
    */
    inline bool isAllocated(KeyType id) const;

    /**
    * Operator to index the pool with the allocated id.
    *
    * @param id Id of the object to be accessed.
    * @return Pointer to the object
    */
    inline UntypedData* operator[](KeyType id);

    /**
    * Operator to index the pool with the allocated id.
    *
    * @param id Id of the object to be accessed.
    * @return Pointer to the object
    */
    inline const UntypedData* operator[](KeyType id) const;

    /**
    * Gets object from the registry with given key.
    *
    * @param key Key that indicates the object that should be retrieved
    * @param pValue On output contains pointer to the value
    * @return A value indicating whether the value was found and retrieved
    */
    inline bool tryGetValue(KeyType id, UntypedData*& rpValue);

    /**
    * Gets key that points to given object.
    *
    * @param pValue Pointer to object that is allocated from the pool
    * @return A key that indicates location of the the object
    */
    KeyType getKey(const UntypedData* pValue) const;

    /**
    *	Gets the name given to the pool instance.
    *
    *	@return Pointer to the name of the instance.
    */
    inline const char* getName() const;

    /**
    *	Gets number of used item
    *
    *	@return Number of used items.
    */
    inline KeyType getNumberOfItems() const;

    /**
    *	Gets maximum number of items
    *
    *	@return Maximum number of items.
    */
    inline KeyType getMaximumNumberOfItems() const;

    /** Iterator class for iterating allocated items */
    class Iterator
    {
    public:
        /** Constructor
        *
        * @param rOwner Owning Pool instance
        */
        inline Iterator(Pool& rOwner);

        /** Checks whether the iterator is still valid and there is more data to be read */
        inline bool hasMore() const;

        /** Gets the key the iterator is pointing at
        *
        * @return key the iterator is pointing at
        */
        inline KeyType getKey() const;

        /** Gets the item the iterator is pointing at
        *
        * @return Pointer to current item or NULL if iterator is invalid
        */
        inline UntypedData* operator*();

        /** Gets the item the iterator is pointing at
        *
        * @return Pointer to current item or NULL if iterator is invalid
        */
        inline const UntypedData* operator*() const;

        /** Prefix increment operator for advancing the iterator
        *
        * return Reference to this iterator
        */
        inline Iterator& operator++();

    private:
        /** Owner Pool instance */
        Pool& mrOwner;

        /** Current iteration index */
        KeyType mIndex;
    };

    /** Gets new iterator instance
    *
    * @return New iterator that points to first allocated item
    */
    inline Iterator begin();

    /** Gets new iterator instance
    *
    * @return New iterator that points to first allocated item
    */
    inline const Iterator begin() const;

#ifdef WB_HAVE_POOL_STATISTICS
    /** Gets pool statistics 
    *
    * @param pStats Array that will receive pool statistics
    * @param maxNumberOfPools Maximum number of pools which statistics should be retrieved
    * @param startIndex Pools are in fixed order index onwards to start getting stats
    * @param totalNumberOfPools [out] Number of pools in total
    * @return Number of pools which data were retrieved
    */
    static size_t getStatistics(WB_RES::PoolStats* pStats, size_t maxNumberOfPools, size_t startIndex, size_t& totalNumberOfPools);

#ifdef WB_HAVE_CRASH_DUMP
    /** Log all pool statistics as system event errors for the crash dump */
    static void crashDump();
#endif // #ifdef WB_HAVE_CRASH_DUMP
#endif // #ifdef WB_HAVE_POOL_STATISTICS

private:
    /**
    * Increases size of the pool
    *
    * @param newSize New size of the pool
    * @return A value indicating wether the pool was correctly grown
    */
    bool grow(KeyType newSize);

private:
    /** Allocators have direct access to these */
    friend class NullPointerPoolAllocator;
    friend class BitmapPoolAllocator;
    friend class FreeListPoolAllocator;
    friend class BlockingFreeListPoolAllocator;

    /** Name of the instantiated pool, for statistics etc. */
    const char* mpName;
    /** Allocator which has the purpose of tracking which elements are free/allocated */
    IAllocator& mrAllocator;
    /** Type adapter implementation */
    const ITypeAdapter& mrTypeAdapter;
    /** Pool size config */
    const Config::PoolSize mConfig;
    /** Number of items currently allocated */
    KeyType mNumberOfAllocatedItems;
    /** The array */
    UntypedData* mpArray;
    /** Number of items currently used */
    volatile uint32 mNumberOfItems;

#ifdef WB_HAVE_POOL_STATISTICS
    /** First instantiated pool */
    static Pool* spFirst;
    /** Next pool in linked list */
    Pool* mpNext;
    /** Number of allocated items */
    uint32 mAllocations;
    /** High water mark */
    uint32 mHighWaterMark;
    /** Cumulative wait time (allocations) */
    uint32 mBlockTimeMs;
    /** Max single wait time (allocations) */
    uint32 mMaxBlockTimeMs;
    /** Number of times the pool has been empty */
    uint32 mEmptyCount;
#endif
};

inline bool Pool::isAllocated(KeyType id) const
{
    WB_DEBUG_ASSERT(id < mNumberOfAllocatedItems);
    return mrAllocator.isAllocated(*this, id);
}

inline Pool::UntypedData* Pool::operator[](KeyType id)
{
    return reinterpret_cast<UntypedData*>(reinterpret_cast<size_t>(mpArray) + id * mrTypeAdapter.mItemSize);
}

inline const Pool::UntypedData* Pool::operator[](KeyType id) const
{
    return reinterpret_cast<const UntypedData*>(reinterpret_cast<size_t>(mpArray) + id * mrTypeAdapter.mItemSize);
}

bool Pool::tryGetValue(KeyType id, UntypedData*& rpValue)
{
    if (isAllocated(id))
    {
        rpValue = (*this)[id];
        return true;
    }

    return false;
}

const char* Pool::getName() const
{
    return mpName;
}

Pool::KeyType Pool::getNumberOfItems() const
{
    return static_cast<Pool::KeyType>(mNumberOfItems);
}

Pool::KeyType Pool::getMaximumNumberOfItems() const
{
    return mConfig.maximumSize;
}

Pool::Iterator Pool::begin()
{
    return Iterator(*this);
}

const Pool::Iterator Pool::begin() const
{
    return Iterator(const_cast<Pool&>(*this));
}

Pool::Iterator::Iterator(Pool& rOwner)
    : mrOwner(rOwner), mIndex(static_cast<KeyType>(-1))
{
    // Move to first item
    ++(*this);
}

bool Pool::Iterator::hasMore() const
{
    return mIndex != mrOwner.mNumberOfAllocatedItems;
}

Pool::KeyType Pool::Iterator::getKey() const
{
    return mIndex;
}

Pool::UntypedData* Pool::Iterator::operator*()
{
    return mrOwner[mIndex];
}

const Pool::UntypedData* Pool::Iterator::operator*() const
{
    return mrOwner[mIndex];
}

Pool::Iterator& Pool::Iterator::operator++()
{
    if (mIndex != mrOwner.mNumberOfAllocatedItems)
    {
        while (++mIndex != mrOwner.mNumberOfAllocatedItems)
        {
            if (mrOwner.isAllocated(mIndex))
            {
                break;
            }
        }
    }

    return *this;
}

} // namespace whiteboard
