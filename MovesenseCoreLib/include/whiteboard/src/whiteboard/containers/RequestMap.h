#pragma once
// Copyright (c) Suunto Oy 2014. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/Request.h"

namespace whiteboard
{

/** Base class for request map */
class RequestMapBase
{
public:
    /**
    * Constructor
    * 
    * @param requestCount Number of request in the map
    */
    RequestMapBase(size_t requestCount);

    /** Indicates if the map is empty.
    *
    *   @return True if map is empty, false otherwise.
    */
    inline bool isEmpty() const { return mRequestCount == 0; };

    /** Indicates if map is full.
    *
    *   @return True if map is full, false otherwise.
    */
    inline bool isFull() const { return mRequestCount == mMaximumRequestCount; };

protected:
    /** Helper to find the first free slot.
    *
    *   @param pKeys Array of keys to search
    *   @param key The requestId used as a key.
    *   @param rFreeSlot The empty location found in the map.
    *   @return Returns true if a free slot was found, false if given key is invalid or already exists in the map.
    */
    bool findFreeSlot(const RequestId* pKeys, const RequestId key, size_t& rFreeSlot);

    /** Helper to find the slot that contais a specific key.
    *
    *   @param pKeys Array of keys to search
    *   @param key The requestId used as a key.
    *   @param rIndex On output contains index of the key in the map.
    *   @return Returns true if the key was found, false if given key is invalid or does not exist in the map.
    */
    bool findKey(const RequestId* pKeys, const RequestId key, size_t& rIndex);

protected:
    /** Current number of requests */
    size_t mRequestCount;

    /** Maximum number of requests */
    size_t mMaximumRequestCount;
};

template <size_t MAP_SIZE, typename ExtraDataType> class RequestMap : public RequestMapBase
{
public:
    /**
    * Default constructor
    */
    RequestMap();

    /** Gets the values from the map by finding the matching key value.
    *   This will also free the slot to be usable by other Request.
    *
    *   @param key The request ID that is to be looked up.
    *   @param rRequest Stores the found request to the passed parameter.
    *   @return true if the request specified with key could be found, false otherwise or if invalid request key was given.
    */
    bool get(const RequestId key, Request& rRequest, ExtraDataType& rExtraData);

    /** Stores the passed request parameters in the next available slot.
    *
    *   @param key The request ID that acts as a key for the map.
    *   @param rRequest The desired Request entry to be mapped to the given key.
    *   @return true if a new request could be added, false if given key already exists or is invalid.
    */
    bool put(const RequestId key, const Request& rRequest, const ExtraDataType& rExtraData);

private:
    /** Array of keys. */
    RequestId mKeys[MAP_SIZE];

    /** Array of requests */
    Request mRequests[MAP_SIZE];

    /** Array of extra data */
    ExtraDataType mExtraData[MAP_SIZE];
};

/** Special case for request maps that do not store extra information */
template <size_t MAP_SIZE> class RequestMap<MAP_SIZE, void> : public RequestMapBase
{
public:
    /**
    * Default constructor
    */
    RequestMap();

    /** Gets the values from the map by finding the matching key value.
    *   This will also free the slot to be usable by other Request.
    *
    *   @param key The request ID that is to be looked up.
    *   @param rRequest Stores the found request to the passed parameter.
    *   @return true if the request specified with key could be found, false otherwise or if invalid request key was given.
    */
    bool get(const RequestId key, Request& rRequest);

    /** Stores the passed request parameters in the next available slot.
    *
    *   @param key The request ID that acts as a key for the map.
    *   @param rRequest The desired Request entry to be mapped to the given key.
    *   @return true if a new request could be added, false if given key already exists or is invalid.
    */
    bool put(const RequestId key, const Request& rRequest);

private:
    /** Array of keys. */
    RequestId mKeys[MAP_SIZE];

    /** Array of requests */
    Request mRequests[MAP_SIZE];
};

template <size_t MAP_SIZE, typename ExtraDataType>
RequestMap<MAP_SIZE, ExtraDataType>::RequestMap() :
    RequestMapBase(MAP_SIZE), mRequests()
{
    for (size_t i = 0; i < MAP_SIZE; ++i)
    {
        mKeys[i] = ID_INVALID_REQUEST;
    }
}

template <size_t MAP_SIZE, typename ExtraDataType>
bool RequestMap<MAP_SIZE, ExtraDataType>::put(
    const RequestId key, const Request& rRequest, const ExtraDataType& rExtraData)
{
    size_t freeSlot = 0;
    if (!findFreeSlot(mKeys, key, freeSlot))
    {
        return false;
    }

    mKeys[freeSlot] = key;
    mRequests[freeSlot] = rRequest;
    mExtraData[freeSlot] = rExtraData;
    mRequestCount++;
    return true;
}

template <size_t MAP_SIZE, typename ExtraDataType>
bool RequestMap<MAP_SIZE, ExtraDataType>::get(const RequestId key, Request& rRequest, ExtraDataType& rExtraData)
{
    size_t validSlot = 0;
    if (!findKey(mKeys, key, validSlot))
    {
        return false;
    }

    mKeys[validSlot] = ID_INVALID_REQUEST;
    rRequest = mRequests[validSlot];
    rExtraData = mExtraData[validSlot];
    mRequestCount--;
    return true;
}

template <size_t MAP_SIZE>
RequestMap<MAP_SIZE, void>::RequestMap() :
    RequestMapBase(MAP_SIZE), mRequests()
{
    for (size_t i = 0; i < MAP_SIZE; ++i)
    {
        mKeys[i] = ID_INVALID_REQUEST;
    }
}

template <size_t MAP_SIZE>
bool RequestMap<MAP_SIZE, void>::put(
    const RequestId key, const Request& rRequest)
{
    size_t freeSlot = 0;
    if (!findFreeSlot(mKeys, key, freeSlot))
    {
        return false;
    }

    mKeys[freeSlot] = key;
    mRequests[freeSlot] = rRequest;
    mRequestCount++;
    return true;
}

template <size_t MAP_SIZE>
bool RequestMap<MAP_SIZE, void>::get(const RequestId key, Request& rRequest)
{
    size_t validSlot = 0;
    if (!findKey(mKeys, key, validSlot))
    {
        return false;
    }

    mKeys[validSlot] = ID_INVALID_REQUEST;
    rRequest = mRequests[validSlot];
    mRequestCount--;
    return true;
}

} // namespace whiteboard
