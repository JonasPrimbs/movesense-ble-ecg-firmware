#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "whiteboard/Identifiers.h"
#include "wb-resources/resources.h"
#include "whiteboard/internal/BuildConfig.h"

namespace whiteboard
{

/** Forward declaration */
struct RoutingTableEntry;

/**
*	Interface class to obtain information from RoutingTable.
*/
class WB_API IRoutingTableObserver
{
public:

    /** Destructor */
    virtual ~IRoutingTableObserver() {}

    /** Routing table change notifications */
    enum RoutingTableNotification
    {
        /** Route has been added to the routing table. Connection has not been established. RoutingTableEntry may have
         * a serial, but it might be wrong. */
        ROUTE_NOTIFICATION_INSERT,

        /** Route has completed handshake and route has been moved to established state. Serial has been updated. */
        ROUTE_NOTIFICATION_HANDSHAKE_UPDATE,

        /** Route's state information has changed */
        ROUTE_NOTIFICATION_STATE_CHANGE,

        /** Route to destination has changed (Used communication adapter or next hop on the route) */
        ROUTE_NOTIFICATION_ROUTE_CHANGE,
        
        /** Route has been removed from the routing table */
        ROUTE_NOTIFICATION_DELETE
    };

    /**
    *   Callback for routing table changes.
    *
    *   @note This callback can be called (not simultaneously) from multiple threads.
    *
    *   @param rRouteEntry Routing table entry
    *   @param type Type of the notification.
    *   @param previousState Previous state of the routing table entry
    */
    virtual void onRoutingTableChange(
        const RoutingTableEntry& rRouteEntry, RoutingTableNotification type, WB_RES::RouteState previousState) = 0;
};

} // namespace whiteboard
