#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/Functor.h"

namespace whiteboard
{

// Forward declarations
class Whiteboard;

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
    * Gets execution context ID for given execution context name
    *
    * @param executionContextName Name of the execution context
    * @return ID of the execution context or ID_INVALID_EXECUTION_CONTEXT if the execution context was not found
    */
    static ExecutionContextId tryFindExecutionContextByName(
        const char* executionContextName);

    /**
    * Gets execution context ID for given execution context name
    *
    * @param executionContextName Name of the execution context
    * @param rWhiteboard Whiteboard instance to use
    * @return ID of the execution context or ID_INVALID_EXECUTION_CONTEXT if the execution context was not found
    */
    static ExecutionContextId tryFindExecutionContextByName(
        const char* executionContextName, const Whiteboard& rWhiteboard);

    /**
    * Checks whether this is the local execution thread of the given of execution context
    *
    * @param executionContextId ID of execution context to compare
    * @return true if the execution context is running in callers thread false if not
    */
    static bool isCurrentThread(ExecutionContextId executionContextId);

    /**
    * Checks whether this is the local execution thread of the given of execution context
    *
    * @param executionContextId ID of execution context to compare
    * @param rWhiteboard Whiteboard instance to use
    * @return true if the execution context is running in callers thread false if not
    */
    static bool isCurrentThread(ExecutionContextId executionContextId, const Whiteboard& rWhiteboard);

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

    /** Option that guide how events should be processed */
    struct WB_API EventProcessorOptions
    {
        /** Constructor */
        inline EventProcessorOptions(uint32 _timeout, bool _doEventProcessing)
            : timeout(WB_MIN(_timeout, MAXIMUM_TIMEOUT)), doEventProcessing(_doEventProcessing ? 1 : 0)
        {
        }

        /** Maximum number of milliseconds to wait for new DPCs and events before calling eventStateProcess
        * again. Use DEFAULT_TIMEOUT to use system default.
        *
        * @note This value specifies maximum only when there is no events. No maximum can be given for how long
        *       single event is processed.
        */
        uint32 timeout : 31;

        /** A flag that indicates that events can be processed for the duration of given timeout.
        *
        * @note DPCs are always processed even if this flag is cleared
        */
        uint32 doEventProcessing : 1;

        /** Maximum timeout value */
        static const uint32 MAXIMUM_TIMEOUT = WB_MIN((1u << 31) - 1, WB_INFINITE) - 1;
    };

    /** Function prototype for worker functions that perform custom processing between
    *  Whiteboard events handling.
    *
    * @param bool : eventsPending; A value indicating whether more events are waiting in the queue
    * @return Options that guide further event prosessing.
    *
    * @see ResourceSubtreeRegistration class is used to install this hook function.
    */
    typedef IFunctor1<EventProcessorOptions, bool> StateProcessorFunc;

    /** Function prototype for filtering incoming update notifications before local message dispatching.
    * Note that this function can be called simultaneously from multiple threads
    *
    * @param clientId Client id associated with the update
    * @param resourceId Resource id associated with the update
    * @param rValue Current value of the resource
    * @param rParameters Notification parameters
    * @return If true, notification is not queued. 
    *
    * @see ResourceSubtreeRegistration class is used to install this hook function.
    */
    typedef IFunctor4<bool, whiteboard::ClientId, whiteboard::ResourceId, const whiteboard::Value&, const whiteboard::ParameterList&>
        NotificationFilterFunc;
};

} // namespace whiteboard
