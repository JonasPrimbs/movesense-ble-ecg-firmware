#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/Identifiers.h" // SuuntoSerial
#include <sstream>

namespace whiteboard
{

// Forward declarations
class CommAdapter;

namespace device_discovery
{

// Forward declarations
class IDeviceEnumerator;
struct Connection;

/**
* Base class for devices.
*/
class WB_API IDevice
{
public:
    /** Destructor */
    virtual ~IDevice() {}

    /** Gets the device serial as a string.
    
    @param pDevice Device of which serial to get as a string
    @return device serial string, eg "1640100001"
    */
    static std::string getSerialString(const IDevice* pDevice);
    
    /** Gets the device + address as a string

    @param pDevice Device of which name-address string to build.

    @return device name address string e.g "COM1,9600,8,N,1,OFF"
    */
    static std::string getDeviceAddressString(const IDevice* pDevice);

    /** Gets owning enumerator
     *
     * @return Owner enumerator instance
     */
    virtual IDeviceEnumerator* getEnumerator() = 0;

    /** Gets type of the device */
    virtual const char* getType() const = 0;

    /** Gets the device serial number (eg. USB serial) */
    virtual SuuntoSerial getSerial() const = 0;

    /** Gets the device description (eg. from USB descriptor) */
    virtual const char* getDescription() const = 0;

    /** Gets the device name. For example "COM6" on Windows or "/dev/cu.usbmodem1411" on OS X. */
    virtual const char* getDevice() const = 0;

    /** Gets the address to use when connecting. For example "127.0.0.1:7809" for TCP connection. Empty for 
     * com ports and other devices which have no addressing. */
    virtual const char* getAddress() const = 0;

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
    virtual CommAdapter* createInterface() const = 0;

    /** Frees communication interface associated with the device
    *
    * @param pInterface Communication interface handle to free
    */
    virtual void freeInterface(CommAdapter* pInterface) const = 0;
};

} // namespace device_discovery
} // namespace whiteboard
