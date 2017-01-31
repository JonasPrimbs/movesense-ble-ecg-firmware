#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <functional>
#include <vector>
#include "whiteboard/devicediscovery/Device.h"
#include "whiteboard/CommAdapter.h"

namespace whiteboard
{
namespace device_discovery
{

/** Interface for device enumerator implementations */
class IDeviceEnumerator
{
public:
    /// Event that happens when a new device is plugged into the system or a device is removed from the system.
    typedef std::function<void(IDeviceEnumerator&)> DeviceChangeListener_t;

    /** Initializes the enumerator
     *
     * @param listener The listener which is notified whenever device list changes.
     */
    virtual void init(const DeviceChangeListener_t listener) = 0;

    /** Gets list of currently connected devices
     *
     * @return List of currently connected devices
     */
    virtual std::vector<Device*> getDevices() = 0;
};

typedef std::vector<IDeviceEnumerator*> DeviceEnumeratorList_t;

} // namespace device_discovery

} // namespace whiteboard
