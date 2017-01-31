// Copyright (c) Suunto Oy 2016. All rights reserved.
#pragma once

#include "whiteboard/ResourceProvider.h"
#include "wb-resources/resources.h"

WB_HEADER_CHECK_DEFINE(WB_HAVE_SUBSCRIPTIONS_INFO)

#ifdef WB_HAVE_SUBSCRIPTIONS_INFO
namespace whiteboard
{
namespace services
{

/** Provider for resource subscriptions ínformation */
class SubscriptionsInfoProvider FINAL : private ResourceProvider
{
public:
    SubscriptionsInfoProvider();
    virtual ~SubscriptionsInfoProvider();

    void initSubmodule();
    void deinitSubmodule();

    /** Maximum amount of subscriptions infos returned in single request */
    static const uint8 NUM_SUBSCRIBER_INFOS_PER_REQUEST = 2;

private:

    /** Clears the data copied here for the previous request */
    void resetRequest();

    /** @see whiteboard::ResourceProvider::onGetRequest */
    virtual void onGetRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

    /** Handles the get request (runs on the context of the requested resource); enumerates all subscriptions for the resource
        and copies the subscriber info to internal data structures to be returned in handleResponse. 
        @see whiteboard::services::SubscriptionsInfoProvider::handleResponse */
    void processRequest();
    
    /** Callback that Whiteboard calls when enumerating subscriptions for a resource. @see whiteboard::Whiteboard::enumerateSubscriptions */
    void onEnumerateSubscriptions(ResourceId resourceId, ClientId clientId, const ParameterList& rPathParameters);
    
    /** Returns result of the original getRequest (runs on the context of the SubscriptionsInfoProvider); Returns result to the original requester.  */
    void handleResponse();

    /**
    The following subscription data needs to be copied here because the subscription enumeration might occur in another context 
    and not in the context this provider is on. 
    */

    /// Request to be responded once the subscription enumeration is finnished.
    Request mRequest;
    
    /// ResourceId for the resource of which subscribers needs to be found.
    ResourceId mRequestedResourceId;
    
    /// Array of SubscriptionInfos to be returned in the response.
    WB_RES::SubscriptionInfo mSubscriptions[NUM_SUBSCRIBER_INFOS_PER_REQUEST];
    
    /// If multiple subscribers exist, this index tells where to continue forming the response.
    uint8 mStartIndex;
    
    /// How many subscribers in the currently filled response.
    uint8 mSubscriberCount;
    
    /// How many subscribers in total for the resource.
    uint8 mTotalSubscriberCount;
    
    /// Memory allocation for copy of the path parameters of each subscriber
    int32 mSubscriberPathParameters[NUM_SUBSCRIBER_INFOS_PER_REQUEST][WB_MAX_NUMBER_OF_PATH_PARAMETERS];
    
    /// Result code of the subscriptions enumeration.
    Result mResult;
};

} // namespace services
} // namespace whiteboard
#endif
