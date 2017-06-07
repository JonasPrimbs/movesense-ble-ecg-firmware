#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/IDevice.h"
#include <sstream>

namespace whiteboard {
namespace device_discovery {

/**
* Base class for devices.
*/
class DeviceBase : public IDevice
{
public:
    /// Owning enumerator
    IDeviceEnumerator* mpEnumerator;

    /// Type of the device
    std::string mType;

    /// The device serial number (USB or simulator serial)
    SuuntoSerial mSerial;

    /// The device description (from USB description or simulator)
    std::string mDescription;

    /// The device name. For example "COM6" on Windows or "/dev/cu.usbmodem1411" on OS X.
    std::string mDevice;

    /// The address to use when connecting. For example "127.0.0.1:7809" for TCP connection. Empty for 
    /// com ports and other devices which have no addressing.
    std::string mAddress;

    /** Costructor */
    DeviceBase() {}

    /** Destructor */
    virtual ~DeviceBase() {}

    /** Gets owning enumerator
    *
    * @return Owner enumerator instance
    */
    virtual IDeviceEnumerator* getEnumerator() override final
    {
        return mpEnumerator;
    }

    /** Gets type of the device */
    virtual const char* getType() const override final
    {
        return mType.c_str();
    }

    /** Gets the device serial number (eg. USB serial) */
    virtual SuuntoSerial getSerial() const override final
    {
        return mSerial;
    }

    /** Gets the device description (eg. from USB descriptor) */
    virtual const char* getDescription() const override final
    {
        return mDescription.c_str();
    }

    /** Gets the device name. For example "COM6" on Windows or "/dev/cu.usbmodem1411" on OS X. */
    virtual const char* getDevice() const override final
    {
        return mDevice.c_str();
    }

    /** Gets the address to use when connecting. For example "127.0.0.1:7809" for TCP connection. Empty for
    * com ports and other devices which have no addressing. */
    virtual const char* getAddress() const override final
    {
        return mAddress.c_str();
    }

    /** Prevent use of copy constructor */
    DeviceBase(const DeviceBase& device) = delete;

    /** Prevent use of copy operator */
    DeviceBase& operator =(const DeviceBase& device) = delete;

    /** Prevent use of comparsion operator */
    bool operator ==(const DeviceBase& device) = delete;

    /** Get a printable string of this device instance. */
//    virtual std::string to_string() const = 0;
};

} // namespace device_discovery
} // namespace whiteboard
