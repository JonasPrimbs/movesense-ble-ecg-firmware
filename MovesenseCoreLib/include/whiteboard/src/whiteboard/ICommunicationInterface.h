#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
 ******************************************************************************/
#include "whiteboard/Result.h"
#include "whiteboard/RoutingTableEntryHandle.h"

namespace whiteboard
{

// forward declarations
struct RoutingTableEntryHandle;

/** Interface for communication interfaces */
class WB_API ICommunicationInterface
{
protected:
    /** Communication adapters should be destroyed through Communication class. */
    friend class Communication;

    /** Destructor. */
    virtual ~ICommunicationInterface() {}

public:

    /**
    *   Gets name of communication adapter
    *
    *   @return Name of the adapter
    */
    virtual const char* getName() const = 0;

    /** Adds new route to the routing table that uses this interface
    *
    * @param address Address of remote end
    * @param serialNumber Serial number of the remote end
    * @param buddy A value indicating whether this should be the buddy route
    * @param rRouteHandle On output contains handle to routing table entry that was just added
    * @return Result of the operation
    */
    virtual Result addRoute(
        const char* address, const char* serialNumber, bool buddy, RoutingTableEntryHandle& rRouteHandle) = 0;

    /** Remove route from the routing table.
    *
    * @param rRouteHandle Handle to the previously added route.
    * @return true if successful.
    */
    virtual Result removeRoute(const RoutingTableEntryHandle& rRouteHandle) = 0;

    /** Removes all routes of the adatapeter from the routing table.
    */
    virtual void removeRoutes() = 0;

    /** Enables the interface
     *
     * @return Result of the operation
     */
    virtual Result enable() = 0;

    /** Disables the interface
    *
    * @param removeAdapterRoutes A value indicating whether all routes should be removed
    *                            from the routing table
    * @return Result of the operation
    */
    virtual Result disable(bool removeAdapterRoutes) = 0;

};

} // namespace whiteboard
