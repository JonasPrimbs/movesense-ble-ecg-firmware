#pragma once
// Copyright (c) Suunto Oy 2018. All rights reserved.

#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>

class ResourceForwardingTracker
{
    static const size_t ARRAY_SIZE = 6;
public:
    ResourceForwardingTracker(wb::ResourceClient& clientApi, wb::ResourceProvider& providerApi);

public:
    // mapping for resource forwarding, has to be done with 32 bit resource IDs because of path parameters
    struct ResourceIdMapEntry
    {
        wb::ResourceId inResource;  // track /sample/intacc/{samplerate}
        wb::ResourceId outResource; // to /meas/acc/{samplerate} mapping
        uint16_t samplerate;        // which sample rate
    };

    // allocates and initialises a map entry for inResoure + samplerate
    ResourceIdMapEntry* allocateMapEntry(const wb::ResourceId inResource, const int32_t samplerate);
    // look for a mapped entry for inResoure + samplerate
    ResourceIdMapEntry* findMapEntry(const wb::ResourceId inResource, const int32_t samplerate);
    // frees the mapping of the inResource + samplerate, returns the outResource for unsubscribing
    wb::ResourceId freeMapEntry(const wb::ResourceId inResource, const int32_t samplerate);
    // will unsubscribe on behalf of mClient if no local subscriptions are open
    void clearDisconnected();

    inline void freeMapEntry(ResourceIdMapEntry* entry)
    {
        *entry = { wb::ID_INVALID_RESOURCE, wb::ID_INVALID_RESOURCE, 0 };
    }

private:
    typedef wb::ResourceProvider::EnumerateSubscriptions<ResourceForwardingTracker>::Functor SubEnumerator;
    void onEnumerateSubscriptions(wb::ResourceId rscId, wb::ClientId clientId, const wb::ParameterList& rPathParams);

    wb::ResourceClient& mClient;
    wb::ResourceProvider& mProvider;
    ResourceIdMapEntry mResourceMap[ARRAY_SIZE]; // array for simultaneous subscription mappings

    // workaround for Whiteboard isResourceSubscribed unique path parameter ResourceId bug/feature
    wb::ResourceId notSoNiceSyncTemporaryResourceId; // "input" parameter 1
    int32_t sampleRate;      // "input" parameter 2
    wb::Result outResult;    // "output" parameter

public:
    // for resource ID path parameter uniqueness, as in client API
    wb::Result isResourceSubscribed(wb::ResourceId rscId, int32_t samplerate);
};
