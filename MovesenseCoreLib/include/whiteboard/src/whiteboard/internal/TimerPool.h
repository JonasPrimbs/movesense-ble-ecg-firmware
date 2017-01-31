#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "whiteboard/Identifiers.h"
#include "whiteboard/containers/BitmapPoolAllocator.h"
#include "whiteboard/containers/TypedPool.h"

namespace whiteboard
{

/** Timer is a wrapper for OS timers that is used by the whiteboard::ResourceProvider and whiteboard::ResourceClient
to start and stop timers and receive timer messages to correct whiteboard::IExecutionContext.*/
class TimerPool FINAL
{
public:
    /** Constructor.
    *
    * @param rMutex Synchronization object to use
    * @param size Size of the pool
    */
    EXPLICIT TimerPool(WbRecursiveMutexHandle& rMutex, Config::Size size);

    /** Destructor */
    ~TimerPool();

    /** Starts the timer with the given parameters. Called by the ResourceProvider and ResourceClient

    @param periodMs Period of the continuous timer to be created in milliseconds
    @param isContinous If true, continuous timer, else single shot
    @param executionContextId ID of the execution context, the subscribed lives in
    @param ownerId LocalProviderId or LocalClientId of the provider or client that wants to receive the timer notifications.
    @param isProvider Is the subscriber whiteboard::ResourceClient or whiteboard::ResourceProvider.

    @return ID of the created timer or ID_INVALID_TIMER if timer allocation & starting fails.
    */
    TimerId start(
        size_t periodMs, bool isContinuous, ExecutionContextId executionContextId, uint16 ownerId, bool isProvider);

    /** Reschedules a running continuous timer with given new period.

    @param timerId ID of the timer to reschedule
    @param periodMs Period of the continuous timer to be created in milliseconds
    @param isContinous If true, continuous timer, else single shot
    @param ownerId LocalProviderId or LocalClientId of the provider or client that wants to receive the timer notifications.
    @param isProvider Is the subscriber whiteboard::ResourceClient or whiteboard::ResourceProvider.
    @return true if succesfully rescheduled, else false
    */
    bool reschedule(
        TimerId timerId, size_t periodMs, uint16 ownerId, bool isProvider);

    /** Stops the timer with the given id.

    @param ownerId LocalProviderId or LocalClientId of the provider or client that wants to receive the timer notifications.
    @param isProvider Is the subscriber whiteboard::ResourceClient or whiteboard::ResourceProvider.
    @param timerId ID of the timer to stop
    @return false if timer was already stopped or timerId did not belong to the subscriber.
    */
    bool stop(uint16 ownerId, bool isProvider, TimerId timerId);

    /** Flags, that previous timer message for this user has been handled. This is a indication to the osTimer-thread to let the
    next timer message through - mechanism to prevent message queue flooding. If the timer is a single shot timer,
    its OS handling will be stopped here.
    
    @param ownerId LocalProviderId or LocalClientId of the provider or client that wants to receive the timer notifications.
    @param isProvider Is the subscriber whiteboard::ResourceClient or whiteboard::ResourceProvider.
    @param timerId ID of the timer to stop
    @return True if parameters specified a valid timer and timer callback should be called
    */
    bool setHandled(uint16 ownerId, bool isProvider, TimerId timerId);

    /** Clears all timers of belonging to certain user.

    @param ownerId LocalProviderId or LocalClientId of the provider or client that wants to receive the timer notifications.
    @param isProvider Is the subscriber whiteboard::ResourceClient or whiteboard::ResourceProvider.
    */
    void clearTimers(uint16 ownerId, bool isProvider);

private:
    // OS timer callback
    static void onTimer(WbTimerHandle timer);

    /** Internal helper for stopping the timer with the given id.
    * Assumes all checks have been done.
    *
    * @param timerId ID of the timer to stop
    */
    void stopChecked(TimerId timerId);

    // Deny creation and copy
    TimerPool(const TimerPool& rOrig) DELETED;
    TimerPool& operator=(const TimerPool& rRhs) DELETED;

    /// One entry in the timer pool
    struct TimerMapEntry FINAL
    {
        union Data // Little optimization to save memory...
        {
            uint32 mRaw;
            struct Parts
            {
                uint8 mIsProvider : 1;
                uint8 mIsContinuous : 1;
                uint8 mIsPending : 1;
                uint8 mReserved : 5;
                ExecutionContextId mExecutionContextId;
                uint16 mOwnerId;
            } mParts;
        } mData;

        /// Constructor
        TimerMapEntry();
    };

    /// Mutex for synchronization
    WbRecursiveMutexHandle& mrMutex;

    /// Pool allocator
    BitmapPoolAllocator mAllocator;

    /// Timer pool
    typedef TypedPool<TimerId, TimerMapEntry> EntryPool;
    EntryPool mTimers;

    /// OS timer handles. OS timers are always allocated, so we cannot apply same kind of
    /// allocation semantics as for the entry pool
    WbTimerHandle* mpTimerHandles;
};

} // whiteboard
