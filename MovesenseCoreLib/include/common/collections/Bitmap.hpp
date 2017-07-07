/******************************************************************************

        Copyright (c) Suunto Oy 2016.
        All rights reserved.

******************************************************************************/
#pragma once

#include "whiteboard/integration/port.h"

#ifdef __ICCARM__
    #ifndef CMSIS_DEVICE_HEADER
    #error CMSIS_DEVICE_HEADER must be defined for interlocked operations on ARM platforms.
    #endif
    #include CMSIS_DEVICE_HEADER
#endif 

class Bitmap FINAL
{
public:
    Bitmap() : mBits(0) {}
    ~Bitmap() {}

    static const uint32 ALLBITS = UINT32_MAX;

    static uint32 flag(unsigned bit) { return (1 << bit); }

    // Sets given bits and returns the original bitmap value.
    uint32 set(uint32 mask)
    {
        uint32 oldBits = mBits;
        mBits = oldBits | mask;
        return oldBits;
    }

    // Clears given bits and returns the original bitmap value.
    uint32 clear(uint32 mask)
    {
        uint32 oldBits = mBits;
        mBits = oldBits & ~mask;
        return oldBits;
    }

    // Returns true if given bits are set, otherwise false
    bool isSet(uint32 mask)
    {
        return (mBits & mask) == mask;
    }

    // Atomically sets given bits and returns the original bitmap value.
    uint32 setAtomic(uint32 mask)
    {
        uint32 oldBits;
#ifndef __ICCARM__
        uint32 newBits;

        do {
            oldBits = mBits;
            newBits = oldBits | mask;
        } while (!WbInterlockedCompareAndExchange32(reinterpret_cast<volatile int32*>(&mBits), newBits, oldBits));
#else
        do {
            oldBits = __LDREXW(reinterpret_cast<volatile unsigned long*>(&mBits));
        } while (__STREXW(oldBits | mask, reinterpret_cast<volatile unsigned long*>(&mBits)));
#endif

        return oldBits;
    }

    // Atomically clears given bits and returns the original bitmap value.
    uint32 clearAtomic(uint32 mask)
    {
        uint32 oldBits;
#ifndef __ICCARM__
        uint32 newBits;

        do {
            oldBits = mBits;
            newBits = oldBits & ~mask;
        } while (!WbInterlockedCompareAndExchange32(reinterpret_cast<volatile int32*>(&mBits), newBits, oldBits));
#else
        do {
            oldBits = __LDREXW(reinterpret_cast<volatile unsigned long*>(&mBits));
        } while (__STREXW(oldBits & ~mask, reinterpret_cast<volatile unsigned long*>(&mBits)));
#endif

        return oldBits;
    }

    // Returns the bitmap value.
    uint32 get() const { return mBits; }

private:
    volatile uint32 mBits;
};
