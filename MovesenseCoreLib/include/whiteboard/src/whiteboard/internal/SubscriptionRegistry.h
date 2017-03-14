#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/containers/BitmapPoolAllocator.h"
#include "whiteboard/containers/TypedPool.h"

namespace whiteboard
{

/** Structure that stores information about subscribed clients */
struct Subscription
{
    /** Constructor */
    Subscription()
    {
        // No default zero initialization
    }

    /** Next subscription in the chain */
    LocalSubscriptionId next;

    /** Following are fields from the ClientId structure,
     * but to save space we make our own copy, because ClientId forces 32-alignment
     * that we don't want here.
     */
    struct
    {
        /** Reserved for future use
        *
        * @note: PathParameterCache uses one bit of this internally as subscription ref-counting for the same path variable,
        *        that implementation needs to be changed if bits here are taken into use
        */
        uint8 reserved : 4;

        /** ID of the execution context */
        ExecutionContextId executionContextId : 4;

        /** ID of the whiteboard instance */
        WhiteboardId whiteboardId;

        /** Client ID of the resource in that whiteboard instance */
        LocalClientId localClientId;

        /** Assignment operator
         *
         * @param rClientId ClientId that should be used to initialize this structure
         */
        inline void operator=(const ClientId& rClientId)
        {
            reserved = rClientId.reserved;
            executionContextId = rClientId.executionContextId;
            whiteboardId = rClientId.whiteboardId;
            localClientId = rClientId.localClientId;
        }

        /** Cast operator. Converts this structure back to ClientId 
         *
         * @return ClientId structure
         */
        inline operator ClientId() const
        {
            return ClientId(executionContextId, whiteboardId, localClientId);
        }

        /** Checks whether this structure equals given client ID
         *
         * @param rClientId ClientId for comparison
         * @return A value indicating whether this structure is equal to given client ID
         */
        inline bool operator==(const ClientId& rClientId) const
        {
            return (localClientId == rClientId.localClientId) && (whiteboardId == rClientId.whiteboardId);
        }

        /** Checks whether this structure is not equal to given client ID
        *
        * @param rClientId ClientId for comparison
        * @return A value indicating whether this structure is not equal to given client ID
        */
        inline bool operator!=(const ClientId& rClientId) const
        {
            return !(*this == rClientId);
        }
    } clientId;
};

/** Subscription registry contrainer
*
*/
class SubscriptionRegistry
{
public:
    /** Constructor
    *
    * @param size Size of the registry
    */
    SubscriptionRegistry(const Config::PoolSize size);

    /** Destructor */
    ~SubscriptionRegistry();

    /** Gets size of the registry
     *
     * @return Maximum number of subscriptions
     */
    inline LocalSubscriptionId getMaximumNumberOfItems() const;

    /**
    * Gets existing subscription
    *
    * @param subscriptionId ID of the subscription
    * @return Pointer to subscription or NULL if subscription was not valid
    */
    inline Subscription* getSubscription(LocalSubscriptionId subscriptionId);

    /**
    * Gets existing subscription
    *
    * @param subscriptionId ID of the subscription
    * @return Pointer to subscription or NULL if subscription was not valid
    */
    inline const Subscription* getSubscription(LocalSubscriptionId subscriptionId) const;

    /**
    * Gets ID of given subscription
    *
    * @param pSubscription Subscription which ID should be returned
    * @return ID of the subscription
    */
    inline LocalSubscriptionId getId(const Subscription* pSubscription) const;

    /**
    * Allocates new client subscription
    *
    * @param clientId Id of the client that owns the subscription
    * @return New subscription instance
    */
    Subscription* allocateSubscription(ClientId clientId);

    /**
    * Frees client subscription
    *
    * @param pSubscription Subscription that should be freed
    */
    void freeSubscription(Subscription* pSubscription);

    /**
    * Frees all client subscriptions of remote whiteboard
    * 
    * @param remoteWhiteboardId ID of remote whiteboard
    */
    void freeSubscriptions(WhiteboardId remoteWhiteboardId);

private:
    /** Allocator for subscription pool */
    BitmapPoolAllocator mAllocator;

    /** Pool of subscriptions */
    typedef TypedPool<LocalSubscriptionId, Subscription> SubscriptionPool;
    SubscriptionPool mSubscriptions;
};

inline LocalSubscriptionId SubscriptionRegistry::getMaximumNumberOfItems() const
{
    return mSubscriptions.getMaximumNumberOfItems();
}

inline Subscription* SubscriptionRegistry::getSubscription(LocalSubscriptionId subscriptionId)
{
    if (subscriptionId == ID_INVALID_LOCAL_SUBSCRIPTION)
    {
        return NULL;
    }

    Subscription* pResult = &mSubscriptions[subscriptionId];
    WB_DEBUG_ASSERT(pResult->clientId != ID_INVALID_CLIENT);
    return pResult;
}

inline const Subscription* SubscriptionRegistry::getSubscription(LocalSubscriptionId subscriptionId) const
{
    Subscription* pResult = const_cast<SubscriptionRegistry*>(this)->getSubscription(subscriptionId);
    return pResult;
}

inline LocalSubscriptionId SubscriptionRegistry::getId(const Subscription* pSubscription) const
{
    return mSubscriptions.getKey(*pSubscription);
}

} // namespace whiteboard
