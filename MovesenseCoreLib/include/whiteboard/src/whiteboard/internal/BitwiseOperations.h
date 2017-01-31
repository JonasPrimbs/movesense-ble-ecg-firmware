#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"

namespace whiteboard
{

/**
    Find the first set bit of the given mask.
    @param mask Bit mask to search
    @return Bit number between 0-31 (32 means not found i.e mask was zero)
*/
uint8 msb32(uint32 mask);

/** Interface class for bit array operations */
class BitArray
{
public:
    /** A value that indicates that bit was not found */
    static const size_t NOT_FOUND = static_cast<size_t>(-1);

    /**
    * Find first set bit in an array.
    * @param theArray Reference to the array itself
    * @param arraySize The amount of elemets to check
    * @return The number/index of the first set bit or NOT_FOUND if bit was not found.
    */
    static size_t findFirstSetBit(const uint32 theArray[], const size_t arraySize);
};


}
