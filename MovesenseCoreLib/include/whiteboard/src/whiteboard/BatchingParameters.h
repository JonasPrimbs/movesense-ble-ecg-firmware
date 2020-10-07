#pragma once
// Copyright (c) Suunto Oy 2019. All rights reserved.

#include "integration/shared/types.h"

namespace whiteboard
{
/* Batching parameters are pass-through from Whiteboard's perspective. It is up to the providers
to support (or not) batching.
*/
struct BatchingParameters
{
    BatchingParameters() : periodMs(0u), flushed(false) {};
    BatchingParameters(const uint32 _periodMs, const bool _flushed) : periodMs(_periodMs), flushed(_flushed) {};

    /* Maximum notification flushing period allowed. 0 means no batching is allowed. Provider can send notifications
    more often if needed or ignore batching period entirely. */
    uint32 periodMs;

    /* If provider supports batching and "flushed" is enabled, it should flush the data if possible,
    e.g. device is awake. Details how this parameter is used is platform and application specific. */
    bool flushed;
};
}
