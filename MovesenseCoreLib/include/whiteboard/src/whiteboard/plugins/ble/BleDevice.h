// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <whiteboard/devicediscovery/IDevice.h>
#include <string>

namespace whiteboard {
namespace comm_ble {

/** BleDevice class implements Device interface for BLE ports. */
class BleDevice FINAL : public device_discovery::IDevice
{
public:
    /// Owning enumerator
    device_discovery::IDeviceEnumerator* mpEnumerator;

    /// The device serial number (USB or simulator serial)
    SuuntoSerial mSerial;

    /// The device description (from USB description or simulator)
    std::string mDescription;

    /// The device name. For example "COM6" on Windows or "/dev/cu.usbmodem1411" on OS X.
    std::string mDevice;

    /// The address to use when connecting. For example "127.0.0.1:7809" for TCP connection. Empty for 
    /// com ports and other devices which have no addressing.
    std::string mAddress;

    /** Constructor */
    BleDevice();

    /** Destructor */
    virtual ~BleDevice() {}

    /** Gets owning enumerator
    *
    * @return Owner enumerator instance
    */
    virtual device_discovery::IDeviceEnumerator* getEnumerator() OVERRIDE
    {
        return mpEnumerator;
    }

    /** Gets type of the device */
    virtual const char* getType() const OVERRIDE
    {
        return "Ble";
    }

    /** Gets the device serial number (eg. USB serial) */
    virtual SuuntoSerial getSerial() const OVERRIDE
    {
        return mSerial;
    }

    /** Gets the device description (eg. from USB descriptor) */
    virtual const char* getDescription() const OVERRIDE
    {
        return mDescription.c_str();
    }

    /** Gets the device name. For example "COM6" on Windows or "/dev/cu.usbmodem1411" on OS X. */
    virtual const char* getDevice() const OVERRIDE
    {
        return mDevice.c_str();
    }

    /** Gets the address to use when connecting. For example "127.0.0.1:7809" for TCP connection. Empty for
    * com ports and other devices which have no addressing. */
    virtual const char* getAddress() const OVERRIDE
    {
        return mAddress.c_str();
    }

    /** Get a printable string of this device instance. */
    //virtual std::string to_string() const OVERRIDE;

protected:
    /** Instantiates the communication interface associated with the device
    *
    * @return Communication interface handle
    */
    virtual CommAdapter* createInterface() const OVERRIDE;

    /** Frees communication interface associated with the device
    *
    * @param pInterface Communication interface handle to free
    */
    virtual void freeInterface(CommAdapter* pInterface) const OVERRIDE;
};

} // namespace comm_ble
} // namespace whiteboard
