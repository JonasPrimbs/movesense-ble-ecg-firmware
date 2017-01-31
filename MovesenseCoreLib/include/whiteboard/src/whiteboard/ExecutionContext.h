#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/Functor.h"

namespace whiteboard
{

/** Interface for execution context related operations
 *
 * @note Avoid using these functions from unit test code that has multiple Whiteboards.
 *       These functions map only to singleton Whiteboard instance.
 */
class WB_API ExecutionContext
{
private:
    // Prevent use of default constructors and assignment operators
    ExecutionContext() DELETED;
    ExecutionContext(const ExecutionContext&) DELETED;
    ExecutionContext& operator=(const ExecutionContext&) DELETED;

public:

    /**
    * Checks whether this is the local execution thread of the given of execution context
    *
    * @param executionContextId ID of execution context to compare
    * @return true if the execution context is running in callers thread false if not
    */
    static bool isCurrentThread(ExecutionContextId executionContextId);

    /**
    * Waits until certain event has passed. While waiting execution context's event are handled.
    *
    * @note The event must be associate with same wake up event (DPC, request, response, timer, etc)
    *       Otherwise the thread won't wake from the sleep immediately when event passes.
    * @note Calling this will increase thread's stack usage, because all events are handled on top
    *       of current stack.
    *
    * @param rEndWait Functor which return value indicates whether wait should be ended
    * @return A value indicating whether the wait was successefully initiated
    */
    static bool syncWait(IFunctor0<bool>& rEndWait);

    /**
    * Signals to wake up the execution context from current sleep.
    *
    * @param executionContextId Id of the execution context to wake
    */
    static void signal(ExecutionContextId executionContextId);

    /**
    * Run execution context event handling in externally created thread
    *
    * @note This function does not return until execution context has been terminated
    *       i.e. whiteboard::shutdown has been called
    *
    * @param executionContextId ID of execution context which event loop should be run
    * @param entryFunction Optional entry point function to execute before starting
    *                      actual event loop. You can use this function to post first
    *                      requests and DPCs to this same execution context.
    * @return A value indicating whether the event loop was successfully run
    */
    static bool runInExternalThread(ExecutionContextId executionContextId, IFunctor0<void>* entryFunction);
};

} // namespace whiteboard
