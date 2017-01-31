#pragma once
/******************************************************************************

        Copyright (c) Suunto Oy 2015.
        All rights reserved.

******************************************************************************/

/** @file Fifo.hpp
        Fifo class template.
*/

#include "common/compiler/genType.h"

namespace nea
{

/**
        Fifo class template.

        A fixed size first-in first-out out queue.

        @param T A type with value semantics.
*/
template <typename T> class Fifo
{
public:
    /**
            Query empty status of a fifo.

            @return bool @li true - fifo is empty.
                                     @li false - fifo is not empty.
    */
    bool empty() const { return mCount == 0; }

    /**
            Query full status of a fifo.

            @return bool @li true - fifo is full.
                                     @li false - fifo is not full.
    */
    bool full() const { return mCount == mSize; }

    /**
            Query size of fifo.

            @return number of items in a fifo.
    */
    size_t size() const { return mCount; }

    /**
            Query capacity of a fifo.

            @return maximum size.
    */
    size_t max_size() const { return mSize; }

    /**
            Push an item to the end of a fifo.

            @param val A value.

            @return bool @li true - success.
                                     @li false - failure.

            @precondition !full().
    */
    bool push(const T& val);

    /**
            Pop the front item off a fifo.

            @precondition !empty().
    */
    bool pop();

    /**
            Query front item of a fifo.

            @return front item reference.

            @precondition !empty().
    */
    T& front() const { return mStorage[ix(0)]; }

    /**
            Peek an item in a fifo.

            @param index An index.

            @precondition index < size().
    */
    T& operator[](size_t index) const { return mStorage[ix(index)]; }

protected:
    Fifo(T* storage, size_t sz) : mStorage(storage), mSize(sz), mCount(0), mNext(0) {}

private:
    void incNext();
    size_t ix(size_t sz) const;

    T* mStorage;
    const size_t mSize;
    size_t mCount;
    size_t mNext;
};

template <typename T> bool Fifo<T>::push(const T& val)
{
    const bool result = mCount < mSize;
    if (result) {
        mStorage[mNext] = val;
        incNext();
        ++mCount;
    }
    return result;
}

template <typename T> bool Fifo<T>::pop()
{
    const bool result = mCount > 0;
    if (result) {
        --mCount;
    }
    return result;
}

template <typename T> void Fifo<T>::incNext()
{
    ++mNext;
    if (mNext == mSize) {
        mNext = 0;
    }
}

template <typename T> size_t Fifo<T>::ix(size_t sz) const
{
    size_t i = mNext + mSize - mCount + sz;
    return i < mSize ? i : i % mSize;
}

/**
        FifoInstance class template,

        Create a fifo instance.

        @param T A type
        @param SZ A size.
*/
template <typename T, size_t SZ> class FifoInstance : public Fifo<T>
{
public:
    FifoInstance() : Fifo<T>(mStorage, SZ) {}

private:
    T mStorage[SZ];
};

} // namespace nea {
