#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"

namespace whiteboard
{

// TODO: Remove this when all of internal code has been moved to internal namespace
namespace internal { struct ResourceSubscriptionData;  }
using internal::ResourceSubscriptionData;

// Forward declarations
struct Subscription;

namespace internal {

class ResourceSubtree;

/** Resource subscription data */
struct ResourceSubscriptionData
{
    /** Constructor */
    ResourceSubscriptionData()
    {
        // No default zero initialization
    }

    /** ResourceSubtree instance that owns this data */
    ResourceSubtree* pOwner;

    /** Index of the resource in the subtree */
    size_t index;

    /** Current resource provider */
    LocalProviderId providerId;

    /** Id of first subscription */
    LocalSubscriptionId firstSubscriptionId;

    /** Helper to remove subscriptions from this resource */
    void unlinkSubscription(Subscription* pPrevious, Subscription* pUnlinkThis);

    /** Writes modified data back to storage */
    void update();
};

} // namespace internal
} // namespace whiteboard
