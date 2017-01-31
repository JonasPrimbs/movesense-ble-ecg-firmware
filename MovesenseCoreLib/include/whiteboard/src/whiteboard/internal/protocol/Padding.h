#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"

namespace whiteboard
{

/** Calculates padding in bytes required for given offset and given alignment */
static inline uint8 padding(size_t bufferOffset, uint8 alignment)
{
    // (~bufferOffset + 1) computes the negative value, that would make up 0 when added to bufferOffset
    // & (alignment - 1) filters only the last relevant bits.
    return (~bufferOffset + 1) & (alignment - 1);
}

}
