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

    /** ID of subscribed client */
    ClientId clientId;
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
#if 0
    /**
    * Custom free list allocator to minimize memory usage
    */
    class SubcriptionFreeListPoolAllocator FINAL : public Pool::IAllocator
    {
    public:
        /** Helper typedefs for a bit cleaner code */
        typedef Pool::KeyType KeyType;

        /** Constructor */
        inline SubcriptionFreeListPoolAllocator()
        {
            mFirstFreeSubscription = 0;
        }

        /**
        * Allocates a new entry from the pool.
        *
        * @param rPool Pool from where the item should be allocated
        * @return KeyType Id which is the index within the array or INVALID_ID if pool is full
        */
        KeyType allocate(Pool& rPool, size_t) OVERRIDE
        {
            // Do we have free subscriptions
            if (mFirstFreeSubscription == ID_INVALID_LOCAL_SUBSCRIPTION)
            {
                return ID_INVALID_LOCAL_SUBSCRIPTION;
            }

            // Get next from the free list
            KeyType subscriptionId = mFirstFreeSubscription;
            mFirstFreeSubscription = static_cast<Subscription*>(rPool[subscriptionId])->next;
            return subscriptionId;
        }

        /**
        * Frees an allocated entry.
        *
        * @param rPool Pool that ows the item
        * @param id Id of the entry
        */
        void deallocate(Pool& rPool, KeyType id) OVERRIDE
        {
            // Move subscription to start of free list
            Subscription& rSubscription = *static_cast<Subscription*>(rPool[id]);
            rSubscription.clientId = ID_INVALID_CLIENT;
            rSubscription.next = mFirstFreeSubscription;
            mFirstFreeSubscription = id;
        }

        /**
        * Checks whether given ID is allocated.
        *
        * @param rPool Pool that ows the item
        * @param id Id of the allocated object.
        * @return A value indicating whether the ID is allocated
        */
        bool isAllocated(const Pool& rPool, KeyType id) const OVERRIDE
        {
            // This is needed only for debug time asserts, which is good since
            // here we assume that client id is set by someone else as soon as item is allocated
            const Subscription& rSubscription = *static_cast<const Subscription*>(rPool[id]);
            return rSubscription.clientId != ID_INVALID_CLIENT;
        }

        /**
        * Increases size of the allocator data structures
        *
        * @param rPool Pool that ows the item
        * @param currentSize Current size of the pool
        * @param newSize New size of the pool
        * @return A value indicating wether the allocator data structures were correctly grown
        */
        bool grow(Pool& /*rPool*/, KeyType /*currentSize*/, KeyType /*newSize*/) OVERRIDE
        {
            for (KeyType i = currentSize; i < newSize; ++i)
            {
                deallocate(rPool, i);
            }

            return true;
        }

    private:
        /** First free subscription. */
        LocalSubscriptionId mFirstFreeSubscription;
    };

    /** Allocator for subscription pool */
    SubcriptionFreeListPoolAllocator mAllocator;
#endif
    /** Allocator for subscription pool */
    BitmapPoolAllocator mAllocator;

    /** Pool of subscriptions */
    typedef TypedPool<LocalSubscriptionId, Subscription> SubscriptionPool;
    SubscriptionPool mSubscriptions;
};

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
