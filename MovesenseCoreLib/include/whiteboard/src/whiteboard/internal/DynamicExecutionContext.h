#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/internal/LocalExecutionContext.h"

namespace whiteboard
{

/** Execution context that is allocated and deallocated at runtime */
class DynamicExecutionContext : public LocalExecutionContext
{
public:
    /** Initializes a new instance of DynamicExecutionContext class
    *
    * @param rInfo Execution context information
    * @param rWhiteboard Associated whiteboard instance. Used only for unit test mock ups.
    */
    DynamicExecutionContext(const metadata::ExecutionContextInfo& rDescriptor,
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

} // namespace whitespace
