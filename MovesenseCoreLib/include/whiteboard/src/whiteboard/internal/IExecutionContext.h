#pragma once
// Copyright (c) Suunto Oy 2014. All rights reserved.

#include "whiteboard/ExecutionContext.h"
#include "whiteboard/Identifiers.h"

namespace whiteboard
{

// Forward declarations
class EventProcessor;

/** Interface for execution contexts that abstracts message passing */
class IExecutionContext
{
public:
    /** Destructor */
    virtual ~IExecutionContext() {}

    /**
    * Gets name of the execution context
    *
    * @return Name of the execution context
    */
    virtual const char* getName() const = 0;

    /**
    * Gets ID of the execution context
    *
    * @return ID of the execution context
    */
    virtual ExecutionContextId getId() const = 0;

protected:
    /** Event processor calls these functions */
    friend class EventProcessor;

    /**
        Handler for all incoming events.
        @param rEvent reference to the event.
    */
    virtual void handle(const WbEvent& rEvent) = 0;

    /**
    * Optional processing of states between events(from IExecutionContext).
    *
    * @param eventsPending A value indicating whether more events are waiting in the queue
    * @return Options that guide further event prosessing.
    */
    virtual ExecutionContext::EventProcessorOptions eventStateProcess(bool eventsPending) = 0;
};

} // namespace whiteboard
