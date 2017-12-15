#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

// TODO: Fix this dependency
#include "whiteboard/unittest/DynamicExecutionContext.h"

#include "whiteboard/unittest/TestClient.h"
#include "whiteboard/unittest/TestProvider.h"
#include "whiteboard/unittest/TestResourceScope.h"

#if WB_UNITTEST_BUILD

namespace whiteboard
{

// Access rights
static const uint32 providerAccessRights = 0;
static const uint32 clientAccessRights = ALL_ACCESS;
const uint8 MAX_DPCS = 2;

// Provider execution context settings
const ExecutionContextInfo providerContextInfo =
    INIT_EXEC_CTX_INFO("DynProviderTestExecCtx", MAX_DPCS, 12, 8, false, WB_THREAD_PRIORITY_HIGH, 2500, providerAccessRights);
// Client execution context settings
const ExecutionContextInfo clientContextInfo =
    INIT_EXEC_CTX_INFO("DynClientTestExecCtx", 5, 6, 42, false, WB_THREAD_PRIORITY_NORMAL, 2200, clientAccessRights);

/**
*	Base class to initialize provider and client context.
*/
template <class ResourceScope = WbResTestResourceScope> class TestContextBase
{
public:
    /** Constructor */
    TestContextBase()
        : mProviderExecutionContext(providerContextInfo),
          mClientExecutionContext(clientContextInfo)
    {
    }

    /** Destructor */
    ~TestContextBase()
    {
    }

    /**
    *	Function to get client context ID.
    *	@return ID of created client context.
    */
    inline ExecutionContextId getClientContextId() const { return mClientExecutionContext.getId(); }

    /**
    *	Function to get provider context ID.
    *	@return ID of created provider context.
    */
    inline ExecutionContextId getProviderContextId() const { return mProviderExecutionContext.getId(); }

private:
    const ResourceScope mTestResourceScope;
    const DynamicExecutionContext mProviderExecutionContext;
    const DynamicExecutionContext mClientExecutionContext;
};

} // namespace whiteboard

#endif
