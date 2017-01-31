/******************************************************************************

        Copyright (c) Suunto Oy 2016.
        All rights reserved.

******************************************************************************/
#pragma once

template <unsigned SZ, typename T>
class RingBuff
{
public:
    RingBuff()
        : mTail(0), mHead(0) {}

    bool empty() const
    {
        return (mTail == mHead);
    }

    void reset()
    {
        mTail = 0;
        mHead = 0;
    }

    bool put(T data)
    {
        unsigned oldTail = mTail;
        unsigned tail = (oldTail + 1) % SZ;

        if (tail == mHead) {
            return false;
        }

        mRing[oldTail] = data;
        mTail = tail;

        return true;
    }

    bool put2x(T data1, T data2)
    {
        unsigned head = mHead;
        unsigned tail = mTail;

        if (((tail >= head) ? (SZ - (tail - head) - 1) : (head - tail) - 1) < 2) {
            return false;
        }

        mRing[tail] = data1;
        tail = (tail + 1) % SZ;
        mRing[tail] = data2;
        mTail = (tail + 1) % SZ;

        return true;
    }

    bool get(T& data)
    {
        unsigned oldHead = mHead;

        if (mTail == oldHead) {
            return false;
        }

        data = mRing[oldHead];
        mHead = (oldHead + 1) % SZ;

        return true;
    }

    bool peek(T& data)
    {
        unsigned head = mHead;

        if (mTail == head) {
            return false;
        }

        data = mRing[head];

        return true;
    }

private:
    unsigned mTail;
    unsigned mHead;

    T mRing[SZ];
};

template <unsigned SZ, typename T>
class RingBuffWm
{
public:
    RingBuffWm()
        : mTail(0), mHead(0), mWmFull(0), mWmNotFull(0), mFull(false), mpfnWm(NULL) {}

    void confWatermarks(unsigned wmFull, unsigned wmNotFull, void (*pfnWm)(bool))
    {
        mWmFull = wmFull;
        mWmNotFull = wmNotFull;
        mpfnWm = pfnWm;
    }

    bool empty() const
    {
        return (mTail == mHead);
    }

    void reset(bool full)
    {
        mTail = 0;
        mHead = 0;

        if (mpfnWm) {
            mFull = false;
            mpfnWm(full);
        }
    }

    bool put(T data)
    {
        unsigned head = mHead;
        unsigned oldTail = mTail;
        unsigned tail = (oldTail + 1) % SZ;

        if (tail == head) {
            return false;
        }

        mRing[oldTail] = data;
        mTail = tail;

        if (mpfnWm && !mFull) {
            unsigned free = ((tail >= head) ? (SZ - (tail - head) - 1) : (head - tail) - 1);

            if (free <= mWmFull) {
                mpfnWm(true);
                mFull = true;
            }
        }

        return true;
    }

    bool get(T& data)
    {
        unsigned oldHead = mHead;
        unsigned tail = mTail;

        if (tail == oldHead) {
            return false;
        }

        unsigned head = (oldHead + 1) % SZ;
        data = mRing[oldHead];
        mHead = head;

        if (mpfnWm && mFull) {
            unsigned free = ((tail >= head) ? (SZ - (tail - head) - 1) : (head - tail) - 1);

            if (free >= mWmNotFull) {
                mFull = false;
                mpfnWm(false);
            }
        }

        return true;
    }

private:
    unsigned mTail;
    unsigned mHead;

    unsigned mWmFull;
    unsigned mWmNotFull;
    bool mFull;
    void (*mpfnWm)(bool full);

    T mRing[SZ];
};
