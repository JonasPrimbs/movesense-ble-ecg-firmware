#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/containers/BlockingFreeListPoolAllocator.h"
#include "whiteboard/containers/TypedPool.h"
#include "whiteboard/comm/Buffer.h"
#include "whiteboard/comm/IBufferAllocator.h"
#include "whiteboard/internal/BuildConfig.h"
#include "whiteboard/internal/Status.h"
#include "whiteboard/internal/Whiteboard.h"

// Check that we are using same build configuration
WB_HEADER_CHECK_DEFINE(WB_COMM_INTERNAL_MESSAGE_BUFFERS)
//WB_HEADER_CHECK_DEFINE(WB_COMM_MESSAGE_RECEIVE_BUFFERS)

namespace whiteboard
{

/** Communication buffer instance with actual data allocation
* TODO: Align using alignas when we can use C++11 */
template <size_t BUFFER_LENGTH> struct BufferInstance
{
    union
    {
        /** Header */
        BufferHeader header;

        /** Align following data to qword boundary */
        WB_ALIGNED(uint64) aligner[(sizeof(BufferHeader) + (sizeof(uint64) - 1)) / sizeof(uint64)];
    };

    /** Data (align to qword boundary, so that structure members align correctly) */
    uint8 data[(BUFFER_LENGTH + (WB_MESSAGE_ALIGNMENT - 1)) / WB_MESSAGE_ALIGNMENT * WB_MESSAGE_ALIGNMENT];
};

// Size should be divisible by eight so that array of buffers keep their aligment
WB_STATIC_VERIFY(sizeof(BufferInstance<1>) % WB_MESSAGE_ALIGNMENT == 0, BufferSizeWillBreakAlignment);
WB_STATIC_VERIFY(WB_OFFSETOF(BufferInstance<1>, data) % WB_MESSAGE_ALIGNMENT == 0, BufferDataIsNotCorrectlyAligned);
WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(BufferInstance<1>) == WB_MESSAGE_ALIGNMENT, BufferIsNotCorrectlyAligned);

/**
* Pool of typed data.
*/
class BufferInstancePool FINAL
{
public:
    /** Type of the key */
    typedef uint16 KeyType;

    /** Value used to indicate invalid keys */
    static const KeyType INVALID_ID = static_cast<KeyType>(-1);

    /** Type of the object */
    typedef Buffer DataType;

    /** Constructor
    *
    * @param name Name of the pool instance.
    * @param config Pool size config
    * @param rAllocator Allocator to use
    */
    inline BufferInstancePool(const char* name, Config::BufferPoolGroupConfig config)
        : mAllocator(),
          mTypeAdapter(config.bufferSize),
          mImplementation(name, config.numberOfBuffers, mAllocator, mTypeAdapter)
    {
    }

    /** Destructor */
    inline ~BufferInstancePool()
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
        return mImplementation.allocate(timeoutMs);
    }

    /**
    * Free an allocated entry.
    *
    * @param id Id of the entry.
    */
    inline void deallocate(KeyType id)
    {
        mImplementation.deallocate(id);
    }

    /**
    * Operator to index the pool with the allocated id.
    *
    * @param id Id of the object to be accessed.
    * @return Pointer to the object
    */
    inline DataType& operator[](KeyType id)
    {
        return *static_cast<DataType*>(mImplementation[id]);
    }

    /**
    * Operator to index the pool with the allocated id.
    *
    * @param id Id of the object to be accessed.
    * @return Pointer to the object
    */
    inline const DataType& operator[](KeyType id) const
    {
        return *static_cast<const DataType*>(mImplementation[id]);
    }

    /**
    * Gets key that points to given object.
    *
    * @param pValue Pointer to object that is allocated from the pool
    * @return A key that indicates location of the the object
    */
    inline KeyType getKey(const DataType& rValue) const
    {
        return mImplementation.getKey(&rValue);
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
        return mImplementation.getNumberOfItems();
    }

    /**
    *	Gets maximum number of items
    *
    *	@return Maximum number of items.
    */
    inline KeyType getMaximumNumberOfItems() const
    {
        return mImplementation.getMaximumNumberOfItems();
    }

    /**
    *	Gets size of the buffers
    *
    *	@return Size of the buffers.
    */
    inline Config::Size getBufferSize() const
    {
        return mTypeAdapter.getBufferSize();
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
            return mImplementation.getKey();
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
    /** Prevents use of default constructor */
    BufferInstancePool() DELETED;
    /** Prevents use of copy constructor */
    BufferInstancePool(const BufferInstancePool&) DELETED;
    /** Prevents use of copy operator */
    const BufferInstancePool& operator=(const BufferInstancePool&) DELETED;

    /** Type adapter implementation */
    class TypeAdapter : public Pool::ITypeAdapter
    {
    public:
        /** Helper to aling buffer size up to next alingment boundary */
        static inline uint16 alingUp(Config::Size bufferSize)
        {
            return (bufferSize + WB_TYPE_ALIGNMENT(Buffer) - 1) / WB_TYPE_ALIGNMENT(Buffer) * WB_TYPE_ALIGNMENT(Buffer);
        }

        /** Constructor
         *
         * @param bufferSize Size of a buffer
         */
        TypeAdapter(Config::Size bufferSize)
            : Pool::ITypeAdapter(WB_OFFSETOF(Buffer, data) + alingUp(bufferSize), WB_TYPE_ALIGNMENT(Buffer))
        {
            WB_DEBUG_ASSERT(mItemSize % WB_TYPE_ALIGNMENT(Buffer) == 0);
            WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(Buffer) == WB_MESSAGE_ALIGNMENT, BufferAlignmentIsNotWhatExpected);
        }

        /** Performs placement new for given array of objects
        *
        * @param pArray Array that is target for placement new
        * @param numberOfItems Number of items in the array
        */
        void placementNewArray(Pool::UntypedData* pArray, KeyType numberOfItems) const OVERRIDE
        {
        }

        /** Performs placement destruct for given array of objects
        *
        * @param pArray Array that is target for placement destruct
        * @param numberOfItems Number of items in the array
        */
        void placementDestructArray(Pool::UntypedData* pArray, KeyType numberOfItems) const OVERRIDE
        {
        }

        /**
        * Assigns source object to target
        *
        * @param pSource Source object
        * @param pTarget Target object
        */
        void assign(void* pSource, void* pTarget) const OVERRIDE
        {
            memcpy(pTarget, pSource, mItemSize);
        }

        /**
        *	Gets size of the buffers
        *
        *	@return Size of the buffers.
        */
        inline Config::Size getBufferSize() const
        {
            return mItemSize - WB_OFFSETOF(Buffer, data);
        }
    };

    /** Allocator */
    BlockingFreeListPoolAllocator mAllocator;
    /** Type adapter */
    TypeAdapter mTypeAdapter;
    /** Actual untyped pool implementation */
    Pool mImplementation;
};

/** Shared pool of communication buffers. If communication
* adapter does not have internal buffers, it should use
* this to allocate buffers for communication subsystem. */
class BufferPool
{
public:
    /** Constructor
     *
     * @param configuration to use
     */
    BufferPool(const Config& rConfiguration);

    /** Destructor */
    ~BufferPool();

    /**
    * Gets status of the BufferPool
    *
    * @return Status flags
    */
    StatusFlags probeStatus();

    /** Allocates a new buffer from buffer pool. Blocks calling
    * thread until a buffer is available.
    *
    * @param pAllocator Buffer allocator instance
    * @param pool Type of the pool where the buffer should allocated from
    * @param payloadOffset Number of bytes to allocate for communication protocol headers
    * @param payloadLength Number of bytes to allocate for the payload
    * @param timeoutMs Timeout in milliseconds
    * @return Pointer to communication buffer or NULL if timeout elapsed before
    * buffer could be allocated
    */
    Buffer* allocate(IBufferAllocator* pAllocator,
                     IBufferAllocator::PoolType pool,
                     uint16 payloadOffset,
                     uint16 payloadLength,
                     size_t timeoutMs);

    /** Frees buffer back to buffer pool. No blocking.
    *
    * @param pBuffer Pointer to communication buffer
    */
    void free(Buffer* pBuffer);

private:
/* Nextgen memory requirements
 *
 * mSmallSendPool:  16 + (12 + 32) * 64 = 2832
 * mMediumSendPool:  16 + (12 + 64) * 24 = 1840
 * mLargeSendPool: 16 + (12 + 128) * 7 = 996
 * mMaxLengthSendPool: 16 + (12 + 520) * 2 = 1080
 * mCommunicationInternalPool: 16 + (12 + 56) * 2 = 136
 *      (max internal messages eg. handshakes)
 * mCommunicationReceivePool: 16 + (12 + 520) * (2 + 3) = 2128
 *      (max receives 2, BLE, Serial(USB), Bus(UART/SPI))
 * -----------------------------------------------------------
 * TOTAL: 9012 bytes
 */
    enum PoolIndex
    {
        SMALL,
        MEDIUM,
        LARGE,
        MAXIMUM,
        COMM_INTERNAL,
        COMM_RECEIVE,

        NUMBER_OF_POOLS
    };

    /** Helper to retrieve pool with given index 
     *
     * @param index Index of the pool
     * @return Pointer to the pool
     */
    inline BufferInstancePool* getPool(uint8 index)
    {
        return reinterpret_cast<BufferInstancePool*>(&mPools[index * sizeof(BufferInstancePool)]);
    }

    union
    {
        /** Helper to align following array correctly */
        TypeOfSizeAndAlignment<WB_TYPE_ALIGNMENT(BufferInstancePool)> aligner;

        /** Memory reservation for pools */
        uint8 mPools[NUMBER_OF_POOLS * sizeof(BufferInstancePool)];
    };
};

} // namespace whiteboard
