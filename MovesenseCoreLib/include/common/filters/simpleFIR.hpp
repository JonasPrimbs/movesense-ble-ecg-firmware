#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/
/**
@file simpleFIR.hpp
*/

#include "common/compiler/genDef.h"

/**
 * GCC gives false positive 'array-out-of-bounds' warning. Disabling it.
 */
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif

/**
 * Simple FIR filter implemetation. No overflow protection or saturation support.
 * User configurable input/coeffient and accumulator data types.
 *
 */
template <typename Input, typename Coefficient, typename Accumulator, size_t NUMBER_OF_TAPS> class SimpleFIR
{
public:
    SimpleFIR(const Coefficient (&coeffs)[NUMBER_OF_TAPS]) : mCoeffs(coeffs), mIndex(0)
    {
        memset(mRingBuffer, 0, (NUMBER_OF_TAPS - 1) * sizeof(Input));
    }

    ~SimpleFIR() {}

    /**
     * Feeds a new sample into filter and returns the filter output
     *
     * @param input		Input sample
     * @returns output sample
     */
    Accumulator update(Input input)
    {
        size_t i = 0;
        Accumulator acc = input * mCoeffs[i++];

        /* from mRingBuffer start index to end of the buffer */
        while (mIndex < NUMBER_OF_TAPS - 1)
        {
            acc += mRingBuffer[mIndex++] * mCoeffs[i++];
        }

        mIndex = 0;

        /* from the end of the buffer to mRingBuffer start index */
        while (i < NUMBER_OF_TAPS)
        {
            acc += mRingBuffer[mIndex++] * mCoeffs[i++];
        }

        /* Adjust the index one step back to the oldest sample*/
        if (mIndex == 0)
        {
            mIndex = NUMBER_OF_TAPS - 2;
        }
        else
        {
            mIndex--;
        }

        /* Replace the oldest sample with the newest one (=input) */
        mRingBuffer[mIndex] = input;

        return acc;
    }

private:
    /* Filter coefficients */
    const Coefficient (&mCoeffs)[NUMBER_OF_TAPS];

    /* Ring buffer for input samples */
    Input mRingBuffer[NUMBER_OF_TAPS - 1];

    /* Pointer to the ring buffer */
    size_t mIndex;
};
