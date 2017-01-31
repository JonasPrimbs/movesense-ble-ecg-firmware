#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/RoutingTableEntryHandle.h"
#include "whiteboard/ICommunicationInterface.h"
#include "whiteboard/devicediscovery/Device.h"

namespace whiteboard
{

namespace device_discovery
{

/** Connection type interface. */
struct Connection final
{
    Device* mpDevice;
    ICommunicationInterface* mpInterface;
    RoutingTableEntryHandle mRouteHandle;
    bool isConnected() const { return mpInterface != nullptr; }
};

} // namespace device_discovery

} // namespace whiteboard
