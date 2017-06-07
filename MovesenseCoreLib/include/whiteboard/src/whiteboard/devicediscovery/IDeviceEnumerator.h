#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <functional>
#include <vector>
#include "whiteboard/devicediscovery/IDevice.h"

namespace whiteboard {
namespace device_discovery {

/** Interface for device enumerator implementations */
class WB_API IDeviceEnumerator
{
public:
    /// Destructor
    virtual ~IDeviceEnumerator() {}

    /** Customized new operator to allocate memory from commonMemPool
    *
    * @param size Number of bytes to allocate
    * @return Pointer to allocated memory block
    */
    static void* operator new(size_t size)
    {
        return WbMemAlloc(size);
    }

    /** Matching customized delete operator
    *
    * @param ptr Pointer to allocated memory block that should be freed
    */
    static void operator delete(void* ptr)
    {
        return WbMemFree(ptr);
    }

    /// Callback that is called when a new device is plugged into the system or a device is removed from the system.
    typedef std::function<void(IDeviceEnumerator&)> DeviceChangeListener_t;

    /** Initializes the enumerator
     *
     * @param listener The listener which is notified whenever device list changes.
     */
    virtual void init(const DeviceChangeListener_t listener) = 0;

    /** Enumerates currently connected devices
     *
     * @param enumCallback Callback that should be called for each of the connected device
     */
    virtual void enumerateDevices(std::function<void(IDevice*)> enumCallback) = 0;
};

typedef std::vector<IDeviceEnumerator*> DeviceEnumeratorList_t;

} // namespace device_discovery
} // namespace whiteboard
