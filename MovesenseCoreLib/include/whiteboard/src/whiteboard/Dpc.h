#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/Functor.h"
#include "whiteboard/Identifiers.h"

namespace whiteboard
{

typedef whiteboard::IFunctor0<void> IDpcFunctor;

// Forward declarations
class EventProcessor;
class DpcFunctor;
class Whiteboard;

/** Deferred procedure call */
class WB_API Dpc FINAL
{
public:
    /** Allocates a new DPC from given execution context
    *
    * @param executionContextId Id of execution context that should execute the DPC
    * @param rFunctor The DPC function to execute
    * @return New instance of DPC object or NULL if DPC could not be allocated
    */
    static Dpc* allocate(const ExecutionContextId executionContextId, IDpcFunctor& rFunctor);

    /** Allocates a new DPC from given execution context of given Whiteboard instance. Used only in unit tests.
    *
    * @param executionContextId Id of execution context that should execute the DPC
    * @param rFunctor The DPC function to execute
    * @param rWhiteboard Whiteboard instance to use
    * @return New instance of DPC object or NULL if DPC could not be allocated
    */
    static Dpc* allocate(const ExecutionContextId executionContextId, IDpcFunctor& rFunctor, const Whiteboard& rWhiteboard);

    /** Deallocates the DPC */
    void deallocate();

    /** Queues DPC execution. If DPC was already queued, this function does nothing.
     *
     * @param isIsr A value indicating whether this function is executed by interrupt service routine
     */
    void queue(bool isIsr);

    /**
    *	Checks whether the given thread is the event processor thread
    *
    *	@param threadHandle thread handle in question
    *	@return true if it is the same thread
    *			false if not
    */
    bool isThisThread(WbThreadHandle threadHandle) const;

private:
    /** Event processor will execute DPCs */
    friend class EventProcessor;
    friend class DpcFunctor;

    /** Prevents use of default constructor */
    Dpc() : mpEventProcessor(NULL), mpDpcFunction(NULL), mDelayed(false)
    {
    }

    /** Prevents direct destruction of DPC object */
    ~Dpc() { }

    /**
    Get the pointer to the assgined DPC functor.
    @return Pointer to the functor.
    */
    IDpcFunctor* functorPtr() const { return mpDpcFunction; }

private:
    /** The associated event processor */
    EventProcessor* mpEventProcessor;

    /** The DPC function to execute */
    IDpcFunctor* mpDpcFunction;

    /** In delayed mode the Dpc functor will be called only after event processing */
    bool mDelayed;
};

} // namespace whiteboard
