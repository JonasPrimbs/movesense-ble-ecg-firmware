#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/Identifiers.h" // SuuntoSerial
#include <sstream>

namespace whiteboard
{

// Forward declarations
class ICommunicationInterface;

namespace device_discovery
{

// Forward declarations
class IDeviceEnumerator;
struct Connection;

/**
* Base class for devices.
*/
class Device
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

    /** Get serial number structure. */
    SuuntoSerial getSerialNumber() const { return mSerial; }

    /** Get the serial number as string. */
    std::string getSerialString() const;

    /** Get the device - address pair string. */
    std::string getDeviceAddressString() const;

    /** Default constructor */
    Device();

    /** Destructor */
    virtual ~Device() {}

    /** Prevent use of copy constructor */
    Device(const Device &device) = delete;

    /** Prevent use of copy operator */
    Device& operator =(const Device &device) = delete;

    /** Prevent use of comparsion operator */
    bool operator ==(const Device &device) = delete;

    /** Get a printable string of this device instance. */
    virtual std::string to_string() const = 0;

    /** Constructs a connection from the device and adds the connection interface
    * to Whiteboard routing table. The route will be disabled. Application needs
    * to enable the route to actually use it.
    *
    * @param connection Connection object
    * @return A value indicating whether the connection was successfully constructed
    */
    bool createConnection(Connection& connection);

    /** Destroys the connection constructed with createConnection
     *
     * @param connection Connection object
     */
    static void destroyConnection(Connection& connection);

protected:
    /** Instantiates the communication interface associated with the device
    *
    * @return Communication interface handle
    */
    virtual ICommunicationInterface* createInterface() const = 0;

    /** Frees communication interface associated with the device
    *
    * @param pInterface Communication interface handle to free
    */
    virtual void freeInterface(ICommunicationInterface* pInterface) const = 0;
};

} // namespace device_discovery
} // namespace whiteboard
