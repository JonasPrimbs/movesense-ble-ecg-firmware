#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/ResourceClient.h"

#if WB_UNITTEST_BUILD

namespace whiteboard
{

// Forward declarations
class WhiteboardMockup;

/// Wait until client has got a result.
/// @return true if result was received, false if not.
template <class TClient> bool WaitOneResult(TClient& client)
{
    for (size_t i = 0; i < 2000; ++i)
    {
        if (client.hasResultBeenReceived())
            return true;

        WbThreadSleepMs(1);
    }
    return false;
}

/// Wait until client has got a notification.
/// @return true if result was received, false if not.
template <class TClient> bool WaitUntilNotification(TClient& client, size_t notificationCount)
{
    for (size_t i = 0; i < 2000; ++i)
    {
        if (client.getNotificationCount() >= notificationCount)
            return true;

        WbThreadSleepMs(1);
    }
    return false;
}

template <class TProvider> bool WaitUntilRegistered(TProvider& provider)
{
    for (size_t i = 0; i < 2000; ++i)
    {
        if (provider.isRegistered())
            return true;

        WbThreadSleepMs(1);
    }
    return false;
}

/** Waits until execution context queue has been processed. Note that queue might still
 * have messages when this call exits. This function only guarantees that there are no messages
 * that have been sent before this function is called. 
 */
void WaitUntilExecutionContextQueueIsProcessed(ExecutionContextId executionContextId);

/** Helper value to indicate that TestClientWithDefaults should use resource ID that was resolved with previous
 * callGetResource or callAsyncGetResourceAndWait
 */
extern const ResourceId CURRENT_RESOURCE;

/** Base class for test clients */
class TestClientBase : public ResourceClient
{
public:
    /**
    *	Constructor
    *	@param pName Name of the resource.
    *	@param executionContextId Providers execution context.
    *   @param rWhiteboard Whiteboard instance to use
    */
    TestClientBase(
        const char* pName,
        ExecutionContextId executionContextId,
        Whiteboard& rWhiteboard);

    /**
    *	Constructor
    *	@param pName Name of the resource.
    *	@param executionContextId Providers execution context.
    *   @param rWhiteboard Whiteboard instance to use
    */
    TestClientBase(
        const char* pName,
        WhiteboardMockup& rWhiteboard);

    /** Desctructor */
    virtual ~TestClientBase();

protected:
    /** Gets the associated whiteboard instance
    *
    * @return Whiteboard instance
    */
    virtual Whiteboard& getWhiteboard() OVERRIDE FINAL
    {
        return mrWhiteboard;
    }

private:
    /** Because ResourceClient constructor binds to whiteboard before virtual table has been fully
    *  initialized, we need to fix the binding to our whiteboard */
    void fixBinding();

    /** ResourceProvider's destructor's unbinding will fail if we have not restored
    *   original binding */
    void unfixBinding();

protected:
    Whiteboard& mrWhiteboard;
    bool mBindingFixed;
};

} // namespace whiteboard

#endif
