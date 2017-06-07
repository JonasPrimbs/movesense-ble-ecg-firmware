#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/RoutingTableEntryHandle.h"
#include "whiteboard/comm/CommAdapter.h"
#include "whiteboard/devicediscovery/IDevice.h"

namespace whiteboard {
namespace device_discovery {

/** Connection type interface. */
struct Connection final
{
    /** Device interface */
    IDevice* mpDevice;

    /** Communication interface (Whiteboard communication adapter) */
    CommAdapter* mpInterface;

    /** Route handle */
    RoutingTableEntryHandle mRouteHandle;

    /** Checks whether this Connection object is valid */
    bool isValid() const { return mpDevice != nullptr; }

    /** Checks whether connection has been established */
    bool isConnected() const { return mpInterface != nullptr; }
};

} // namespace device_discovery
} // namespace whiteboard
