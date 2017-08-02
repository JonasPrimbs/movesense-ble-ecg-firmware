#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

namespace whiteboard
{

/** Queue implementation that is single writer - multiple reader thread safe.
 * Implementation is based on ring buffer that is protected using interlocked operations.
 *
 * Implementation assumes that writes are not done simultaneously either from 
 * multiple threads or ISRs. Use mutex or disable interrupts to provide mutual
 * exclusion of multiple writes.
 *
 * Don't use very large objects, because this class makes copies.
 */
template <typename ItemType>
class WB_API InterlockedQueue
{
public:
    /** Constructor
     *
     * @param size Size of the queue
     */
    InterlockedQueue(uint32 size);

    /** Destructor */
    ~InterlockedQueue();

    /** Gets size of the queue
    *
    */
    inline size_t getSize() const;

    /** Gets number of free slots in the queue
     *
     * @note This is safe to call from the writer. When executed in ISR
     *       this will return actual free count. When executed outside the ISR
     *       readers may interrupt prosessing and the returned free count may be
     *       smaller than the actual free count.
     */
    inline size_t getFreeCount() const;

    /** Adds new element to the buffer
     *
     * @param rItemType Item to add
     * @param discardOldest A value indicating whether oldest item should be removed
     *        in case the buffer was full
     * @return A value indicating whether item was successfully added to the buffer
     *         False if the buffer was full and discardOldest was false
     */
    inline bool push(const ItemType& rItemType, bool discardOldest);

    /** Adds new element to the buffer from ISR
    *
    * @param rItemType Item to add
    * @param discardOldest A value indicating whether oldest item should be removed
    *        in case the buffer was full
    * @return A value indicating whether item was successfully added to the buffer
    *         False if the buffer was full and discardOldest was false
    */
    inline bool pushISR(const ItemType& rItemType, bool discardOldest);

    /** A value indicating whether buffer has been over flown
     *
     * @return A value indicating whether buffer has overflown and items
     *         have been discarded
     */
    inline bool hasOverflown();

    /** Gets oldest element from the buffer
     *
     * @param rItemType On output contains copy of the oldest item
     * @return A value indicating whether item could be copied.
     *        False if the buffer was empty
     */
    inline bool pop(ItemType& rItemType);

private:
    /** Number of allocated items (size+1 because we need to distinct empty and full states) */
    const uint32 mAllocatedItems;

    /** Read position */
    volatile uint32 mReadPosition;

    /** WritePosition */
    volatile uint32 mWritePosition;

    /** A value indicating whether the buffer has overflown */
    volatile uint32 mOverflown;

    /** The data */
    ItemType* mpItems;
};

template <typename ItemType>
InterlockedQueue<ItemType>::InterlockedQueue(uint32 size)
    : mAllocatedItems(size + 1), mReadPosition(0), mWritePosition(0), mOverflown(0)
{
    mpItems = NewArray<ItemType>(mAllocatedItems);
}

template <typename ItemType>
InterlockedQueue<ItemType>::~InterlockedQueue()
{
    DeleteArray<ItemType>(mpItems);
}

template <typename ItemType>
inline size_t InterlockedQueue<ItemType>::getSize() const
{
    return mAllocatedItems - 1;
}

template <typename ItemType>
inline size_t InterlockedQueue<ItemType>::getFreeCount() const
{
    uint32 writePosition = mWritePosition;
    uint32 readPosition = mReadPosition;
    if (readPosition <= writePosition)
    {
        readPosition += mAllocatedItems;
    }

    return (readPosition - writePosition) - 1;
}

template <typename ItemType>
inline bool InterlockedQueue<ItemType>::push(const ItemType& rItemType, bool discardOldest)
{
    // One or more reader threads can interrupt us any time.

    do
    {
        uint32 newWritePosition = (mWritePosition + 1) % mAllocatedItems;
        uint32 originalReadPosition = mReadPosition;
        if (originalReadPosition == newWritePosition)
        {
            mOverflown = 1;

            // Queue is full.
            if (!discardOldest)
            {
                return false;
            }

            // Move read position
            uint32 newReadPosition = (originalReadPosition + 1) % mAllocatedItems;

            if (!WbInterlockedCompareAndExchange32(
                reinterpret_cast<volatile int32*>(&mReadPosition), newReadPosition, originalReadPosition))
            {
                // Some one was faster than us. Retry.
                continue;
            }
        }

        mpItems[mWritePosition] = rItemType;
        mWritePosition = newWritePosition;
    } while (0);

    return true;
}

template <typename ItemType>
inline bool InterlockedQueue<ItemType>::pushISR(const ItemType& rItemType, bool discardOldest)
{
    // This should be running uninterrupted in ISR
    uint32 newWritePosition = (mWritePosition + 1) % mAllocatedItems;
    if (mReadPosition == newWritePosition)
    {
        mOverflown = 1;

        // Queue is full.
        if (!discardOldest)
        {
            return false;
        }

        // Move read position
        mReadPosition = (mReadPosition + 1) % mAllocatedItems;
    }

    mpItems[mWritePosition] = rItemType;
    mWritePosition = newWritePosition;
    return true;
}

template <typename ItemType> inline bool InterlockedQueue<ItemType>::hasOverflown()
{
    return WbInterlockedExchange32(reinterpret_cast<volatile int32*>(&mOverflown), 0) != 0;
}

template <typename ItemType>
inline bool InterlockedQueue<ItemType>::pop(ItemType& rItemType)
{
    // ISR and single writer can interrupt us any time.

    // Atomically move read position
    uint32 originalReadPosition, newReadPosition;
    do
    {
        originalReadPosition = mReadPosition;
        if (originalReadPosition == mWritePosition)
        {
            // Currently queue seems empty.
            return false;
        }

        // Copy value from original read position
        rItemType = mpItems[originalReadPosition];

        newReadPosition = (originalReadPosition + 1) % mAllocatedItems;
    } while (!WbInterlockedCompareAndExchange32(
        reinterpret_cast<volatile int32*>(&mReadPosition), newReadPosition, originalReadPosition));

    return true;
}

} // namespace whiteboard
