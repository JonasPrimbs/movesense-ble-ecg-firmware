#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/Initialization.h"
#include "whiteboard/containers/BitmapPoolAllocator.h"
#include "whiteboard/containers/TypedPool.h"

#ifdef WB_HAVE_PATH_PARAMETERS

namespace whiteboard
{

// Forward declarations
class Value;
class WhiteboardCommunication;

/** Internal storage type of path parameters */
typedef int32 PathParameter; // Only ints supported currently

/** Value that is used to indicate invalid path parameter */
static const PathParameter INVALID_PATH_PARAMETER_VALUE = INT32_MAX;

/** Structure that stores information about clients path parameters */
class PathParameterCacheEntry
{
public:
    /** ID of client this path parameter entry belongs to */
    ClientId mClientId;

    /** ID of the resource with instance ID */
    ResourceId mResourceId;

    /** Since the parameter list does not hold the actual data, this is the data for the parameter list */
    PathParameter mPathParameterData[WB_MAX_NUMBER_OF_PATH_PARAMETERS];

    /** Constructor */
    PathParameterCacheEntry() : mClientId(), mResourceId() { reset(); }

    /** Gets resource ID of the actual resource 
     *
     * @return Resource ID of the actual resource
     */
    inline ResourceId getResourceOwnerId() const { return ResourceId(mResourceId.getConstId()); }

    /** Clears this entry as it was after construction*/
    void reset()
    {
        mClientId = ID_INVALID_CLIENT;
        mResourceId = ID_INVALID_RESOURCE;
        for (size_t i = 0; i < ELEMENTS(mPathParameterData); ++i)
        {
            mPathParameterData[i] = INVALID_PATH_PARAMETER_VALUE;
        }
    }

    /**
    Adds new path parameter for this path parameter cache entry.

    @param PathParameterIndex [in] Index of this path parameter
    @param rPathParameter [in] Path parameter to store
    @return true if success, else false
    */
    bool addPathParameter(size_t PathParameterIndex, const PathParameter& rPathParameter)
    {
        if (PathParameterIndex >= ELEMENTS(mPathParameterData))
        {
            WB_ASSERT(false); // too deep path parameter path
            return false;
        }

        mPathParameterData[PathParameterIndex] = rPathParameter;
        return true;
    }

    /** Gets number of path parameters
     *
     * @return Path parameter count
     */
    uint8 getCount() const
    {
        for (uint8 i = 0; i < ELEMENTS(mPathParameterData); ++i)
        {
            if (INVALID_PATH_PARAMETER_VALUE == mPathParameterData[i])
            {
                return i;
            }
        }
        
        return ELEMENTS(mPathParameterData);
    }
};

/**
This class is intented to store (on the provider's Whiteboard) the path parameters of a client that
is accessing such a path (e.g. /maps/0/tiles/1), so that the whiteboard can map the request to
correct provider and pass the correct path parameters with the call.
*/
class PathParameterCache
{
public:
    /** Constructor
     *
     * @param size Size of the path parameter cache 
     */
    PathParameterCache(const Config::PoolSize size);
    
    /** Destructor */
    ~PathParameterCache();

    /**
    *	Gets number of used items
    *
    *	@return Number of used items.
    */
    inline uint16 getNumberOfItems() const
    {
        return mEntries.getNumberOfItems();
    }

    /**
    * Getter for entry from the cache.
    *
    * @param clientId [in] Id if the client
    * @param rResourceId [in] Id of the resource, the instanceId is compared too
    * @return entry from the cache with the given client and resourceId. NULL if entry not found.
    */
    PathParameterCacheEntry* getEntry(ClientId clientId, const ResourceId resourceId);
    const PathParameterCacheEntry* getEntry(ClientId clientId, const ResourceId resourceId) const;

    /**
    * Getter for entry from the cache with specific path parameters.
    *
    * @param clientId [in] Id if the client
    * @param localResourceId [in] Id of the local resource (resource instanceId is NOT compared here)
    * @param PathParameters [in] List of path parameters to compare against
    * @return entry from the cache with the given client and resourceId. NULL if entry not found.
    */
    const PathParameterCacheEntry* getEntry(
        ClientId clientId,
        LocalResourceId localResourceId,
        const PathParameter PathParameters[WB_MAX_NUMBER_OF_PATH_PARAMETERS]) const;

    /**
    * Checks if this client is found in the cache for given resource
    *
    * @param clientId [in] Id if the client
    * @param rResourceId [in] Id of the resource, the instanceId is compared too
    * @return true, if the client has subscribed this path parameter resource
    */
    bool isClientInCache(ClientId clientId, const ResourceId resourceId) const;

    /// @returns new entry that has just been allocated from the linked list, null if list full
    PathParameterCacheEntry* addEntry();

    /** Marks this entry free
    * 
    * @param rEntry Entry to free
    * @return true if succesfully removed, false if entry not found
    */
    bool removeEntry(PathParameterCacheEntry& rEntry);

    /**
    * Removes entry from the list, based on the clientId & rResourceId
    *
    * @return true if succesfully removed, false if entry not found
    */
    bool removeEntry(ClientId clientId, const ResourceId resourceId);

    /// Removes all entries of the given client from the cache
    void removeEntries(ClientId clientId);

    /// Removes all entries of the given client from the cache
    void removeEntries(WhiteboardId whiteboardId);

    /// Removes all entries with the given resource id from the cache
    void removeEntries(LocalResourceId localResourceId);

    /// Empties and resets the whole cache as if it was just constructed
    void clear();

    /// Determines the next free resource instanceId for a path parameter
    ResourceInstanceId getNextResourceInstanceId(ClientId clientId, const LocalResourceId localResourceId);

    /// @returns entry with duplicate client & resourceId, otherwise NULL.
    PathParameterCacheEntry* getDuplicate(const PathParameterCacheEntry& rEntry);

    /// Finds first resource with matching resourceId and returns the number of path parameter parameters, else returns -1
    uint8 getPathParameterCount(const ResourceId resourceId) const;

    /// Enum for subscriptionRefCount method to decide whether to substract or add to reference count.
    enum SubscriptionOpType { ADD, SUBST };
    /// Adds or substracts subsciption reference count for a client to certain resource found in the path parameter cache.
    bool subscriptionRefCount(SubscriptionOpType opType, ClientId clientId, const ResourceId resourceId);

    /// Has the given entry been subscribed by any client
    bool isSubscribed(const PathParameterCacheEntry& rEntry) const;

private:
    PathParameterCache(const PathParameterCache&) DELETED;
    PathParameterCache& operator=(const PathParameterCache&) DELETED;

    /** Marks this entry free
    */
    void removeEntry(uint16 index);

private:
    /// Type of the pool that contains path parameter entries
    typedef TypedPool<uint16, PathParameterCacheEntry> EntryPool;

    /// Allocator for the pool
    BitmapPoolAllocator mPoolAllocator;

    /// Cache entry pool
    EntryPool mEntries;
};

inline bool PathParameterCache::isSubscribed(const PathParameterCacheEntry& rEntry) const
{
    return rEntry.mClientId.reserved > 0;
}

} // namespace whiteboard

#endif
