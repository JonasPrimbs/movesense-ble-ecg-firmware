#pragma once
// Copyright (c) Suunto Oy 2019. All rights reserved.

#include "integration/shared/types.h"

namespace whiteboard
{
/* Batching parameters are pass-through from Whiteboard's perspective. It is up to the providers
to support (or not) batching. Default parameters are non-batched and real-time.

-------------------------------------------------------------------------------------------------
periodMs | onlyLatestNotification | Behavior
-------------------------------------------------------------------------------------------------
    0    |          false         | Each notification is transferred to client ASAP, even if it
         |                        | means the subsystem client runs in has to be woken up.
-------------------------------------------------------------------------------------------------
   >0    |          false         | Notifications are collected into a batch and transferred to
         |                        | client at appropriate time (batch fills up or something else
         |                        | triggers flushing it).
-------------------------------------------------------------------------------------------------
    0    |          true          | Latest notification is stored, and transferred to client when
         |                        | client is ready to process it (subsystem in which the client
         |                        | is running is awake). This means client might not receive
         |                        | every notification.
-------------------------------------------------------------------------------------------------
   >0    |          true          | Reserved for application specific usage. Treat unsupported
         |                        | periodMs value as zero.
-------------------------------------------------------------------------------------------------
*/
struct BatchingParameters
{
    BatchingParameters(uint32 _periodMs = 0u, bool _onlyLatestNotification = false) :
        periodMs(_periodMs), onlyLatestNotification(_onlyLatestNotification) {}

    /* Maximum notification flushing period allowed. 0 means no batching is allowed. Provider can send notifications
    more often if needed or ignore batching period entirely. */
    uint32 periodMs;

    /* True if client is interested of only the latest notification. */
    bool onlyLatestNotification;
};
}
