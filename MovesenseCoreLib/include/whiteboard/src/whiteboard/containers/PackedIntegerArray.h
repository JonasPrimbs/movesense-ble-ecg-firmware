#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#include "whiteboard/integration/port.h"

namespace whiteboard
{

/**
* Untyped implementation of an array that stores integers of given bit-width
* Array is not thread-safe. The user must ensure thread safety.
*/
class PackedIntegerArray FINAL
{
public:
    /** Constructor
    *
    * @param numberOfItems Initial number of items to allocate
    * @param bitsPerItem Number of bits per item
    */
    PackedIntegerArray(size_t numberOfItems, uint8 bitsPerItem);

    /** Destructor */
    ~PackedIntegerArray();

    /** Gets size of teh array */
    inline size_t size() const
    {
        return mNumberOfItems;
    }

    /** Gets value of an item */
    size_t operator[](size_t index) const;

    /** Sets value of an item */
    void set(size_t index, size_t value);

    /** Sets all bits of the array off */
    void setZeros();

    /** Sets all bits of the array on */
    void setOnes();

private:
    /** Prevent use of default constructor */
    PackedIntegerArray() DELETED;

    /** Prevent use of copy constructor */
    PackedIntegerArray(const PackedIntegerArray&) DELETED;

    /** Prevent use of assignment operator */
    PackedIntegerArray& operator=(const PackedIntegerArray&) DELETED;

private:
    /** Number of items */
    const size_t mNumberOfItems;

    /** Number of bits per item */
    const uint8 mBitsPerItem;

    /** Data array */
    size_t* mpData;
};

/**
* Typed implementation of an array that stores integers of given bit-width.
* Typed implementation hides all casts between source and implementation data types.
* Array is not thread-safe. The user must ensure thread safety.
*/
template <typename T>
class TypedPackedIntegerArray FINAL
{
public:
    /** Constructor
    *
    * @param numberOfItems Initial number of items to allocate
    * @param bitsPerItem Number of bits per item
    */
    inline TypedPackedIntegerArray(size_t numberOfItems, uint8 bitsPerItem)
        : mImplementation(numberOfItems, bitsPerItem)
    {
    }

    /** Destructor */
    inline ~TypedPackedIntegerArray()
    {
    }

    /** Gets size of teh array */
    inline size_t size() const
    {
        return mImplementation.size();
    }

    /** Gets value of an item */
    inline T operator[](size_t index) const
    {
        return static_cast<T>(mImplementation[index]);
    }

    /** Sets value of an item */
    inline void set(size_t index, T value)
    {
        mImplementation.set(index, static_cast<size_t>(value));
    }

    /** Sets all bits of the array off */
    inline void setZeros()
    {
        mImplementation.setZeros();
    }

    /** Sets all bits of the array on */
    inline void setOnes()
    {
        mImplementation.setOnes();
    }

private:
    /** Prevent use of default constructor */
    TypedPackedIntegerArray() DELETED;

    /** Prevent use of copy constructor */
    TypedPackedIntegerArray(const TypedPackedIntegerArray&) DELETED;

    /** Prevent use of assignment operator */
    TypedPackedIntegerArray& operator=(const TypedPackedIntegerArray&) DELETED;

private:
    /** The implementation */
    PackedIntegerArray mImplementation;
};

} // namespace whiteboard
