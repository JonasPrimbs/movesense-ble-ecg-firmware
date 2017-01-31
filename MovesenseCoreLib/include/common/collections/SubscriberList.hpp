/******************************************************************************

        Copyright (c) Suunto Oy 2016.
        All rights reserved.

******************************************************************************/
#pragma once

#include <whiteboard/Identifiers.h>

namespace nea
{

class SubscriberListBase
{
protected:
    struct SubscriberListEntry_t {
        whiteboard::ClientId clientId;
        whiteboard::ResourceId resourceId;
    };

protected:
    SubscriberListBase(SubscriberListEntry_t* subscribers, size_t count);

public:
    static const size_t INVALID_LOCATION = static_cast<size_t>(-1);

    /**
     * Test whether entry idx has subscriber
     */
    inline bool hasSubscriber(size_t idx) const
    {
        return mSubscribers[idx].clientId != whiteboard::ID_INVALID_CLIENT;
    }

    /**
     * Add subscriber, return slot where subscriber was added or
     * INVALID_LOCATION if full. If subscriber is already present, the
     * previous location is returned.
     */
    size_t add(whiteboard::ClientId clientId, whiteboard::ResourceId resourceId);
    /**
     * Remove subscriber, return slot where subscriber was or
     * INVALID_LOCATION if not found.
     */
    size_t remove(whiteboard::ClientId clientId, whiteboard::ResourceId resourceId);

private:
    const size_t mCount;

private:
    SubscriberListEntry_t* const mSubscribers;
};

template <typename ParamT>
class SubscriberListWithParam : public SubscriberListBase
{
private:
    ParamT* const mParams;

protected:
    inline SubscriberListWithParam(SubscriberListEntry_t* subscribers, ParamT* params, size_t count)
        : SubscriberListBase(subscribers, count),
          mParams(params) {}

public:
    /**
     * Add subscriber, return slot where subscriber was added or
     * INVALID_LOCATION if full. If subscriber is already present,
     * update the parameter value.
     */
    size_t add(whiteboard::ClientId clientId, whiteboard::ResourceId resourceId,
               ParamT param)
    {
        size_t loc = SubscriberListBase::add(clientId, resourceId);
        if (loc != INVALID_LOCATION)
            mParams[loc] = param;
        return loc;
    }
};

template <typename ParamT, size_t _count>
class SubscriberList : public SubscriberListWithParam<ParamT>
{
public:
    static const size_t count = _count;

    inline SubscriberList()
        : SubscriberListWithParam<ParamT>(mSubscriberArray, mParamArray, count) {}

    inline void setParam(size_t idx, ParamT value)
    {
        mParamArray[idx] = value;
    }

    inline ParamT getParam(size_t idx) const
    {
        return mParamArray[idx];
    }

private:
    SubscriberListBase::SubscriberListEntry_t mSubscriberArray[count];
    ParamT mParamArray[count];
};
}
