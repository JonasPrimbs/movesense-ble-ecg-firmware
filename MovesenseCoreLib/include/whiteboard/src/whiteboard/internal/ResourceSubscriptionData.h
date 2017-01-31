#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"

namespace whiteboard
{

// Forward declarations
struct Subscription;

/** Resource subscription data */
struct ResourceSubscriptionData
{
    /** Current resource provider */
    LocalProviderId providerId;

    /** Id of first subscription */
    LocalSubscriptionId firstSubscriptionId;

    /** Helper to remove subscriptions from this resource */
    void unlinkSubscription(Subscription* pPrevious, Subscription* pUnlinkThis);
};

} // namespace whiteboard
