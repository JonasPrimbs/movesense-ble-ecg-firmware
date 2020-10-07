// Copyright (c) Suunto Oy 2018. All rights reserved.

#include "ResourceForwardingTracker.h"
#include "app-resources/resources.h"

ResourceForwardingTracker::ResourceForwardingTracker(wb::ResourceClient& clientApi, wb::ResourceProvider& providerApi):
    mClient(clientApi),
    mProvider(providerApi),
    mResourceMap{ { wb::ID_INVALID_RESOURCE, wb::ID_INVALID_RESOURCE, 0 },
                  { wb::ID_INVALID_RESOURCE, wb::ID_INVALID_RESOURCE, 0 },
                  { wb::ID_INVALID_RESOURCE, wb::ID_INVALID_RESOURCE, 0 },
                  { wb::ID_INVALID_RESOURCE, wb::ID_INVALID_RESOURCE, 0 },
                  { wb::ID_INVALID_RESOURCE, wb::ID_INVALID_RESOURCE, 0 },
                  { wb::ID_INVALID_RESOURCE, wb::ID_INVALID_RESOURCE, 0 }, },
    notSoNiceSyncTemporaryResourceId(wb::ID_INVALID_RESOURCE),
    sampleRate(0),
    outResult(wb::HTTP_CODE_INVALID)
{
}

ResourceForwardingTracker::ResourceIdMapEntry* ResourceForwardingTracker::allocateMapEntry(const wb::ResourceId inResource,
                                                                                           const int32_t samplerate)
{
    for (size_t i = 0; i < ELEMENTS(mResourceMap); i++)
    {
        if (mResourceMap[i].inResource == wb::ID_INVALID_RESOURCE)
        {
            // store ID, unfortunately not unique by path paramter :(, thus we also need to track the sample rates separately
            return &(mResourceMap[i] = { inResource, 0, static_cast<uint16_t>(samplerate) });
        }
    }
    return NULL;
}

ResourceForwardingTracker::ResourceIdMapEntry* ResourceForwardingTracker::findMapEntry(const wb::ResourceId inResource,
                                                                                       const int32_t samplerate)
{
    for (size_t i = 0; i < ELEMENTS(mResourceMap); i++)
    {
        if ((mResourceMap[i].inResource == inResource) && (mResourceMap[i].samplerate == samplerate))
        {
            return &mResourceMap[i];
        }
    }
    return NULL;
}

wb::ResourceId ResourceForwardingTracker::freeMapEntry(const wb::ResourceId inResource, const int32_t samplerate)
{
    ResourceIdMapEntry* pEntry = findMapEntry(inResource, samplerate);
    if (NULL != pEntry)
    {
        const wb::ResourceId outResource = pEntry->outResource;
        freeMapEntry(pEntry);
        return outResource;
    }
    return wb::ID_INVALID_RESOURCE;
}

void ResourceForwardingTracker::clearDisconnected()
{
    for (size_t i = 0; i < ELEMENTS(mResourceMap); i++)
    {
        if (mResourceMap[i].outResource != wb::ID_INVALID_RESOURCE)
        {
            if (wb::HTTP_CODE_NO_CONTENT == isResourceSubscribed(mResourceMap[i].inResource, mResourceMap[i].samplerate))
            {
                wb::Result r = mClient.asyncUnsubscribe(mResourceMap[i].outResource, wb::ResourceClient::AsyncRequestOptions::Empty);
                if (false == wb::RETURN_OK(r))
                {
                    DEBUGLOG("asyncUnsubscribe error: %u", r);
                }
                freeMapEntry(&mResourceMap[i]);
            }
        }
    }
}

void ResourceForwardingTracker::onEnumerateSubscriptions(wb::ResourceId rscId,
                                                         wb::ClientId clientId,
                                                         const wb::ParameterList& rPathParams)
{
    if (rscId == notSoNiceSyncTemporaryResourceId) // hopefully the resourceId uniqueness will be fixed soon
    {
        if (WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE::SUBSCRIBE::ParameterListRef(rPathParams).getSampleRate() == sampleRate)
        {
            outResult = wb::HTTP_CODE_OK;   // still have subscriptions for the resource & samplerate
        }
    }
}

wb::Result ResourceForwardingTracker::isResourceSubscribed(wb::ResourceId rscId, int32_t samplerate)
{
    notSoNiceSyncTemporaryResourceId = rscId; // hopefully the resourceId uniqueness will be fixed soon
    sampleRate = samplerate;
    outResult = wb::HTTP_CODE_NO_CONTENT; // reset result to "no subscriptions" state
    SubEnumerator enumFunctor(this, &ResourceForwardingTracker::onEnumerateSubscriptions);
    mProvider.enumerateSubscriptions(rscId, enumFunctor);

    return outResult; // HTTP_CODE_OK == subscribed, HTTP_CODE_NO_CONTENT == not subscribed
}
