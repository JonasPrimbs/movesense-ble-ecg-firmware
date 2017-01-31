// Copyright (c) Suunto Oy 2014. All rights reserved.
#pragma once

#include "whiteboard/DpcFunctor.h"
#include "whiteboard/internal/Whiteboard.h"
#include "whiteboard/internal/BuildConfig.h"

// Check that we are using same build configuration
WB_HEADER_CHECK_DEFINE(WB_HAVE_EVT_STATISTICS)
WB_HEADER_CHECK_DEFINE(WB_HAVE_EVT_TRACE)

namespace WB_RES
{
    struct EventQueueInfo;
}

namespace whiteboard
{

class Dpc;
class IExecutionContext;

/** Event processor abstraction for async task execution */
class EventProcessor FINAL
{
private:
    /** Default constructor hidden */
    EventProcessor();

public:

    /** Event types */
    enum EventType
    {
        /** Invalid event */
        WB_EVENT_INVALID = 0,

        /** Data message event including a buffer allocated for the data */
        WB_EVENT_REQUEST_DATAMSG,
        WB_EVENT_RESPONSE_DATAMSG,

        /** Request events without a buffer */
        WB_EVENT_TINY_GET,
        WB_EVENT_TINY_PUT,
        WB_EVENT_TINY_POST,
        WB_EVENT_TINY_DEL,
        WB_EVENT_TINY_SUB,
        WB_EVENT_TINY_UNSUB,
        WB_EVENT_TINY_GET_RESOURCE,
        WB_EVENT_TINY_REL_RESOURCE,
        WB_EVENT_TINY_GET_RESPONSE,
        WB_EVENT_TINY_PUT_RESPONSE,
        WB_EVENT_TINY_POST_RESPONSE,
        WB_EVENT_TINY_DEL_RESPONSE,
        WB_EVENT_TINY_SUB_RESPONSE,
        WB_EVENT_TINY_UNSUB_RESPONSE,
        WB_EVENT_TINY_GET_RESOURCE_RESPONSE,
        WB_EVENT_TINY_REL_RESOURCE_RESPONSE,

        /** Client timer event */
        WB_EVENT_CLIENT_TIMER,

        /** Provider timer event */
        WB_EVENT_PROVIDER_TIMER,

        WB_EVENT_REMOTE_WB_DISCONNECTED_NOTIFICATION,
        WB_EVENT_LOCAL_CLIENT_REMOVE,
        WB_EVENT_CLIENT_ON_LOCAL_RESOURCE_UNAVAILABLE,

        WB_EVENT_ID_COUNT
    };

public:
    /**
    * Constructor
    *
    * @param rContext Reference to the execution context.
    * @param rSettings Execution context settings.
    */
    EventProcessor(
        IExecutionContext& rContext,
        const metadata::ExecutionContextSettings& rSettings);

    /** Destructor */
    ~EventProcessor();

    /**
    * Initializes the event processor
    * @return A value indicating whether event processing was successfully initialized
    */
    bool init();

    /**
    * Waits until certain event has passed. While waiting execution context's event are handled.
    *
    * @note The event must be associate with same wake up event (DPC, request, response, timer, etc)
    *       Otherwise the thread won't wake from the sleep immediately when event passes.
    * @note Calling this will increase thread's stack usage, because all events are handled on top
    *       of current stack.
    *
    * @param rEndWait Functor which return value indicates whether wait should be ended
    */
    void syncWait(IFunctor0<bool>& rEndWait);

    /**
    * Signals to wake up the thread from current sleep.
    */
    void signal();

    /**
     * Run event processor with externally created thread
     *
     * @param entryFunction Optional entry point function to execute before 
     *                      starting actual event loop
     * @return A value indicating whether the event processor was successfully run
     */
    bool runInExternalThread(IFunctor0<void>* entryFunction);

    /**
    * Terminates the execution context
    */
    void terminate();

    /**
    *  Returns the handle to the created thread.
    */
    WbThreadHandle getThreadHandle() const { return mEventThread; }

    /** Get the settings used to initialize this EventProcessor. */
    const metadata::ExecutionContextSettings& getMetadata() const { return mrSettings; }

    /**
    * Gets the maximum stack depth used in creation of this thread.
    */
    uint16 getMaxStackDepth() const;

    /** Allocates a new DPC
    *
    * @param rFunctor Functor to execute
    * @return New instance of DPC object or NULL if DPC could not be allocated
    */
    Dpc* allocateDpc(IDpcFunctor& rFunctor);

    /** Deallocates the DPC */
    void deallocateDpc(Dpc& rDpc);

    /** Queues DPC execution. If DPC was already queued, this function does nothing.
    *
    * @param rDpc DPC to queue
    * @param isIsr A value indicating whether this function is executed by interrupt service routine
    */
    void queueDpc(Dpc& rDpc, bool isIsr);

    /**
    *	Checks whether the given thread is the event processor thread
    *
    *	@param threadHandle thread handle in question
    *	@return true if it is the same thread
    *			false if not
    */
    inline bool isThisThread(WbThreadHandle threadHandle) const { return threadHandle == mEventThread; }

    /** Dispatches a new request event to the event queue
    *
    * @param rEvent Event that should be dispatched
    * @param timeout Max time to wait for free event queue slot if queue full.
    * @return Return value of the operation
    */
    Result dispatch(const WbEvent& rEvent, size_t timeout = WB_DEFAULT_DISPATCH_TIMEOUT_MS);

    /** @return true if the event processor has any pending request or response events */
    bool hasEvents() const
    {
        return (mEventQueue && WbEventQueueHasEvents(mEventQueue));
    }

private:
    /**
    * Thread call back function for handling events.
    *
    * @param pUserData Used as pointer to EventProcessor class instance.
    */
    static void threadRunnerCB(void* pUserData);

    /**
    * Thread loop for handling events and DPCs.
    */
    void threadFunction();

    /**
    * Checks whether event processor termination has been requested
    *
    * @return A value indicating whether event processor should terminate
    */
    bool isTerminationRequested() const;

    /** Release dynamically allocated queues etc. */
    void releaseAllocations();

private:
    /** Reference to the parent context that handles the events */
    IExecutionContext& mrContext;

    /** The execution context settings. */
    const metadata::ExecutionContextSettings& mrSettings;

    /** Handle of the thread */
    volatile WbThreadHandle mEventThread;

    /** Thread notification bit set */
    WbThreadNotificationHandle mThreadNotification;

    /** Event queue of the execution context */
    WbEventQueueHandle mEventQueue;

    /** Array of DPCs */
    Dpc* mDpcs;

    /** Thread state signals */
    enum
    {
        THREAD_STARTING,
        THREAD_RUNNING,
        THREAD_TERMINATING,
        THREAD_SELF_TERMINATING,
        THREAD_TERMINATED
    };

    /** Current thread state */
    volatile uint8 mThreadState;

    /** Type of dispatch */
    enum DispatchType
    {
        DISPATCH_TYPE_REQUEST,
        DISPATCH_TYPE_RESPONSE,

        DISPATCH_TYPE_COUNT,
        DISPATCH_TYPE_INVALID = DISPATCH_TYPE_COUNT,
    };

    /** Lookup table for dispatch types */
    static const DispatchType dispatchTypeLookUp[WB_EVENT_ID_COUNT];

    /** Semaphores for counting request / response events and waiting for posting opportunity */
    WbSemaphoreHandle mSlotsAvailable[DISPATCH_TYPE_COUNT];

#ifdef WB_HAVE_EVT_STATISTICS
public:
    /** Copies all metrics data to the given target info array.
    * @param infoArray Pointer to the target array for the data.
    * @param maxCount Max amount of elements in the target array.
    * @param infoType Requested info type (requests, responses, timer events)
    * @return Amount of elements copied into the array-
    */
    static size_t copyEventQueueInfos(WB_RES::EventQueueInfo* infoArray, size_t maxCount, uint8 infoType);

private:
    class StatsMembers;
    StatsMembers* mStats;
#endif

#ifdef WB_HAVE_EVT_TRACE
private:
    /** Writes current state of each event processor using system events */
    static void emitStatesAsSystemEvents();

public:

    /** Trace data structure */
    struct TraceData
    {
        /** Trace event types */
        enum
        {
            TYPE_NONE,
            TYPE_WAIT,
            TYPE_DPC,
            TYPE_EVENT,
            TYPE_PROCESS,
        };

        /** Type of trace event */
        uint8 traceType;

        /** Trace event specific data */
        union
        {
            /** Unused data */
            uint8 extraData;

            /** Index of executed DPC. Valid when traceType == TYPE_DPC. */
            uint8 dpcIndex;

            /** Event ID. Valid when traceType == TYPE_EVENT. */
            uint8 eventId;
        };

        /** Timestamp of last trace event (16 least significant bits) */
        uint16 timestamp;

        /** Additional trace data */
        uint32 data;
    };
    
    /** Trace of current event processor operation */
    volatile TraceData mTraceData;
#endif
};

} //namespace whiteboard
