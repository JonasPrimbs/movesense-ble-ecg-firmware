#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/internal/LocalExecutionContext.h"

#if WB_UNITTEST_BUILD

namespace whiteboard
{

/** Execution context information */
struct ExecutionContextInfo
{
    /** Name of the execution context */
    const char* const name;

    /** Execution context settings */
    const metadata::ExecutionContext settings;

    /** Pointer to function that is used to filter update notifications before dispatching. */
    ExecutionContext::NotificationFilterFunc* pNotificationFilter;

    /** Pointer to function that that performs custom processing between
    *  Whiteboard events handling. */
    ExecutionContext::StateProcessorFunc* pStateProcessor;
};

#define INIT_EXEC_CTX_INFO(name, numberOfDpcs, numberOfRequests, numberOfResponses, externalThread, priority, stackSize, securityMask) \
    { name, { whiteboard::metadata::INVALID_STRING, numberOfDpcs, numberOfRequests, numberOfResponses, externalThread ? 1 : 0, 0, priority, 0, stackSize, securityMask }, NULL, NULL }

/** Execution context that is allocated and deallocated at runtime */
class DynamicExecutionContext : public LocalExecutionContext
{
public:
    /** Initializes a new instance of DynamicExecutionContext class
    *
    * @param rInfo Execution context information
    * @param rWhiteboard Associated whiteboard instance. Used only for unit test mock ups.
    */
    DynamicExecutionContext(const ExecutionContextInfo& rDescriptor,
                            Whiteboard& rWhiteboard = *Whiteboard::getInstance());

    /** Destructor */
    virtual ~DynamicExecutionContext();

private:
    // Private since DynamicExecutionContext is initialized in constructor.
    virtual bool init() OVERRIDE { return LocalExecutionContext::init(); }

private:
    /** ExecutionContextMap needs direct access */
    friend class ExecutionContextMap;

    /** Next execution context in the list */
    DynamicExecutionContext* mpNext;
};

} // namespace whiteboard

#endif
