#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

#include "whiteboard/DpcFunctor.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/Initialization.h"
#include "whiteboard/ResourceProvider.h"
#include "whiteboard/containers/InterlockedQueue.h"
#include "whiteboard/internal/BuildConfig.h"
#include "whiteboard/comm/internal/IRoutingTableObserver.h"

namespace whiteboard
{

// Forward declarations
class RoutingTable;
class WhiteboardCommunication;

namespace services
{

/**
*	Routing table service provider. Provides content and notifications from RoutingTable.
*/
class RoutingTableProvider FINAL : public ResourceProvider, private IRoutingTableObserver
{
public:
    
    /** Initializes a new instance of RoutingTableProvider
     *
     * @param rCommunication WhiteboardCommunication instance to use
     */
    RoutingTableProvider(WhiteboardCommunication& rCommunication);

    /** Destructor */
    virtual ~RoutingTableProvider();

    /// Called by the WhiteboardServices launchable module
    void initSubmodule();
    void deinitSubmodule();

private:
    /**
    *	GET request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *	@return Result of the operation
    */
    void onGetRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

private:
    /**
    *   Callback for routing table changes.
    *
    *   @note This callback can be called (not simultaneously) from multiple threads.
    *
    *   @param rRouteEntry Routing table entry
    *   @param type Type of the notification.
    *   @param previousState Previous state of the routing table entry
    */
    void onRoutingTableChange(
        const RoutingTableEntry& rRouteEntry, RoutingTableNotification type, WB_RES::RouteState previousState) OVERRIDE;

private:
    /** Prevent use of default constructor */
    RoutingTableProvider() DELETED;
    /** Prevent use of copy constructor */
    RoutingTableProvider(const RoutingTableProvider&) DELETED;
    /** Prevent use of assigment operator */
    const RoutingTableProvider& operator=(const RoutingTableProvider&) DELETED;

    /** Handles /Net GET request */
    void handleRoutingTableRequest(const Request& rRequest, const ParameterList& rParameters);

    /** Handles /Net/Info GET request */
    void handleRoutingTableEntryRequest(const Request& rRequest, const ParameterList& rParameters);

    /** Handles /Net/Adapter/{index}/Name GET request */
    void handleAdapterNameRequest(const Request& rRequest, const ParameterList& rParameters);

    /** Handles /Net/Adapter/{index}/Status GET request */
    void handleAdapterStatusRequest(const Request& rRequest, const ParameterList& rParameters);

    /** Handles /Net/Info/{remoteWhiteboardIdInLocalScope}/Address GET request */
    void handleRoutingTableEntryAddressRequest(const Request& rRequest, const ParameterList& rParameters);

    /** Handles /Net/Info/{remoteWhiteboardIdInLocalScope}/SerialNumber GET request */
    void handleRoutingTableEntrySerialNumberRequest(const Request& rRequest, const ParameterList& rParameters);

    /** Handles /Net/Info/{remoteWhiteboardIdInLocalScope}/State GET request */
    void handleRoutingTableEntryStateRequest(const Request& rRequest, const ParameterList& rParameters);

    // Update callback for the DPC
    void update();

private:
    // Associated WhiteboardCommunication instance
    WhiteboardCommunication& mrCommunication;

    // Current reincarnament of the routing table
    uint8 mListReincarnament;

    // Notification information
    struct Notification
    {
        /** Type of the notification */
        WB_RES::RoutingTableNotificationType type;

        /** ID of the whiteboard */
        WhiteboardId whiteboardId;

        /** Serial number of the whiteboard */
        SuuntoSerial serial;
    };
    
    /** Notification queue */
    InterlockedQueue<Notification> mNotificationQueue;

    /** Dpc for updating the resource */
    DpcFunctor mUpdateDpc;
};

} // namespace services
} // namespace whiteboard
