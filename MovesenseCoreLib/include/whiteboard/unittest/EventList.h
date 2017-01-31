#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

namespace whiteboard
{

/// Waitable event list to track for example multiple provider and client interactions on many contexts
class EventList FINAL
{
public:
    struct Event
    {
        const char* loggerName;
        const char* event;

        Event() : loggerName(NULL), event(0) {}
        Event(const char* name, const char* type) : loggerName(name), event(type) {}
        // Default assignment operator ok
    };

    Event* mEventList;
    volatile size_t mSize;
    const size_t mMaxSize;
    WbMutexHandle mMutex;

    EventList(size_t maxSize) : mSize(0), mMaxSize(maxSize), mMutex(WbMutexCreate())
    {
        mEventList = new Event[maxSize];
    }

    ~EventList()
    {
        WbMutexDelete(mMutex);
    }

    void add(const Event& rEvent)
    {
        ASSERT_TRUE(mSize < mMaxSize);
        WbMutexLock(mMutex);
        mEventList[mSize++] = rEvent;
        WbMutexUnlock(mMutex);
    }

    bool waitUntilNrEvents(size_t eventCount)
    {
        for (size_t i = 0; i < 20000; ++i)
        {
            if (mSize == eventCount)
            {
                return true;
            }
            
            WbThreadSleepMs(1);
        }
        
        return false;
    }
};

} // namespace whiteboard
