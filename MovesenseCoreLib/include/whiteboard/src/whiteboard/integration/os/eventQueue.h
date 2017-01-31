// Copyright (c) Suunto Oy 2015-2016. All rights reserved.
#pragma once

#include "whiteboard/integration/port.h"

/// Class representing one event in the event queue
class WbEvent
{
public:

    /// Default ctor - members initialized to zero/NULL
    WbEvent() : id(0), id2(0), iData2(0), iData(0), iData3(0) {}
    
    /// Ctor with event id - rest of the items initialized to zero
    WbEvent(uint8 _id) : id(_id), id2(0), iData2(0), iData(0), iData3(0) {}
    
    /// Ctor with event id and three data items
    WbEvent(uint8 _id, uint32 d1, uint16 d2 = 0, uint16 d3 = 0) : id(_id), iData2(d2), iData(d1), iData3(d3)
    {
    }

    /// Ctor with event id and three data items, where d1 is a pointer to user data
    WbEvent(uint8 _id, void* d1, uint16 d2 = 0, uint16 d3 = 0) : id(_id), id2(0), iData2(d2), pData(d1), iData3(d3)
    {
    }
    
    /// Ctor with event id, sub event id and three data items
    WbEvent(uint8 _id, uint8 _id2, uint32 d1, uint16 d2 = 0, uint16 d3 = 0) : id(_id), id2(_id2), iData2(d2), iData(d1), iData3(d3)
    {
    }
    
    /// Ctor with event id, sub event id and three data items, where d1 is a pointer to user data
    WbEvent(uint8 _id, uint8 _id2, void* d1, uint16 d2 = 0, uint16 d3 = 0) : id(_id), id2(_id2), iData2(d2), pData(d1), iData3(d3)
    {
    }

    /// Id of the event
    uint8 id;
    
    /// Subid of the event
    uint8 id2;
    uint16 iData2;

    /// 1st payload of the event
    union
    {
        uint32 iData;
        void* pData;
    };

    /// 2nd payload of the event
    uint16 iData3;

    /// 3rd payload of the event
    uint16 iData4;
};

struct WbEventQueue; // Forward declaration hides the implementation
typedef WbEventQueue* WbEventQueueHandle;
#define WB_INVALID_EVENTQUEUE NULL

/** Creates and event queue.

@param name Name of the event queue.
@param maxEvents Maximun number of event objects the queue can hold.

@return handle to the created event queue or WB_INVALID_EVENTQUEUE if failed.

*/
WB_API WbEventQueueHandle WbEventQueueCreate(const char* name, size_t maxEvents);

/** Deletes event queue.

@param eventQueueHandle handle to the event queue to delete.
*/
WB_API void WbEventQueueDelete(WbEventQueueHandle eventQueueHandle);

/** Post new event to the event queue from interrupt.

@param eventQueueHandle handle to the event queue to post the event.
@param rEvent Event to be posted to the event queue (physical copy of the event is taken).

@return true if post was succesfull, else false.
*/
WB_API bool WbEventQueuePostFromISR(WbEventQueueHandle eventQueueHandle, const WbEvent& rEvent);

/** Post new event to the event queue. Blocks if the queue is full.

Note: If event is to be posted from interrupt use WbEventQueuePostFromISR.

@param eventQueueHandle handle to the event queue to post the event.
@param rEvent Event to be posted to the event queue (physical copy of the event is taken).
@param timeoutInMs If the event queue is full wait posting for the timeoutInMs milliseconds.

@return true if post was succesfull, else false.
*/
WB_API bool WbEventQueuePost(WbEventQueueHandle eventQueueHandle, const WbEvent& rEvent, size_t timeoutInMs);

/** Test if the event queue has events pending.

@param eventQueueHandle event queue to test.
*/
WB_API bool WbEventQueueHasEvents(WbEventQueueHandle eventQueueHandle);

/** Reads the oldest event from the event queue.

@param eventQueueHandle event queue to read event from.
@param rEvent reference to event to read data from the queue.
@param timeoutInMs Timeout to wait reading if the event queue is empty.

@return true if the read succeeded, else false.
*/
WB_API bool WbEventQueueTryGetEvent(WbEventQueueHandle eventQueueHandle, WbEvent& rEvent, size_t timeoutInMs);
