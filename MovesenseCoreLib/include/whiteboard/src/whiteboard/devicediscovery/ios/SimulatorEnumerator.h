/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

******************************************************************************/

#pragma once

// Dummy no-op implementation for other platforms
#include <vector>
#include "Device.h"
namespace whiteboard
{
namespace device_discovery
{

class SimulatorEnumerator final
{
public:
    SimulatorEnumerator(const std::vector<std::string> &knownWindowTitles, const DeviceChangeListener_t listener) { }
    ~SimulatorEnumerator() { }

    std::vector<Device> doEnumerate() const { return{}; }
};

} // namespace device_discovery

} // namespace whiteboard

#endif
