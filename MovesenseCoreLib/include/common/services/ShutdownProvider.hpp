#pragma once
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/ResourceClient.h>

#include "platform/bsp/systemevent.h"

namespace nea
{

/**
 * Provide a controlled and safe shutdown service to the system.
 */
class ShutdownProvider FINAL : private whiteboard::ResourceProvider, protected whiteboard::ResourceClient
{
public:
    ShutdownProvider();
    virtual ~ShutdownProvider();

    // called from the launchable module
    bool wbRegister();
    // no unregister since we are the last active prior to pulling the rug from under

private:
    /** Callback for timer notifications.
     *
     * @param timerId ID of the timer that has expired.
     *
     * @see whiteboard::ResourceProvider::startTimer
     * @see whiteboard::ResourceProvider::stopTimer
     */
    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE FINAL;

    /**
     *  GET request handler.
     *
     *  @param requestId ID of the request
     *  @param clientId ID of the client that should receive the result
     *  @param resourceId ID of the associated resource
     *  @param parameters List of parameters for the request
     *  @return Result of the operation
     */
    void onGetRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE FINAL;

    /**
     *  PUT request handler.
     *
     *  @param requestId ID of the request
     *  @param clientId ID of the client that should receive the result
     *  @param resourceId ID of the associated resource
     *  @param parameters List of parameters for the request
     *  @return Result of the operation
     */
    void onPutRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;

    /**
     *  Subscribe notification callback.
     *
     *  @param clientId ID of the client that is subscribing
     *  @param resourceId ID of the associated resource
     *  @return Result of the operation
     */
    virtual void onSubscribe(const whiteboard::Request& request,
                             const whiteboard::ParameterList& parameters) OVERRIDE FINAL;

    /**
     *  Unsubscribe notification callback.
     *
     *  @param clientId ID of the client that is unsubscribing
     *  @param resourceId ID of the associated resource
     *  @return Result of the operation
     */
    virtual void onUnsubscribe(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE FINAL;

    /**
     * Performs shutdown procedure (required hw deinit); Should be called when all shutdown
     * subscribers have unregistered.
     *
     * Depending on next requested system mode, either performs power off or reset.
     */
    void systemShutdown();

    // first part of shutdown logic. triggered by timer from PUT
    void shutdownProcedure_part1();
    // 2nd part of shutdown procedure. triggered by 1st part or last unsubscribe or timeout timer
    void shutdownProcedure_part2();

    // Shutdown timer and state
    whiteboard::TimerId mTimerId;
    uint32 mShutdownStartTime;
    uint16 mSubscriberCount;
};

} // namespace nea
