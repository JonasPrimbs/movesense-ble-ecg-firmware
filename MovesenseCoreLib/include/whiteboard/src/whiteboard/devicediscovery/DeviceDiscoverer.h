#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <mutex>
#include "whiteboard/comm/Communication.h"

#include "whiteboard/devicediscovery/IDeviceEnumerator.h"
#include "whiteboard/devicediscovery/Connection.h"

namespace whiteboard {
namespace device_discovery {

typedef std::vector<Connection> ConnectedDeviceList_t;
typedef std::function<void(const ConnectedDeviceList_t&)> DeviceDiscoveryCallback_t;

/**
* Discovering and connecting devices discovered by device enumerators.
*
* Devices are ad hoc connected when enumerator reports a set of devices.
*/
class WB_API DeviceDiscoverer final
{
public:
    /** Construct new instance of DeviceDiscoverer class.
    *
    * @param rEnumerators List of enumerators to use
    * @param discoveryCb The user-specified function to call when device is detected
    * @note The callback might be called from a different thread than where the class was originally constructed.
    */
    DeviceDiscoverer(DeviceEnumeratorList_t& rEnumerators, const DeviceDiscoveryCallback_t discoveryCb);
    
    /** Destructor */
    ~DeviceDiscoverer();

    /** Creates new connection to a serial device
    *
    * @param portDescriptor Port descriptor. A string with port parameters separated
    *        with space or comma. Eg. COM1,9600,8,N,1,OFF
    *        Name of the port is required. Rest of the fields are optional and will
    *        be set to default values if value is not specified by the descriptor.
    */
    static Connection connectToSerialDevice(const char* portDescriptor);

    /** Creates new connection to a serial device
    *
    * @param portName Name of the serial port to use
    * @param baudRate Communication baud rate
    * @param dataBits Number of data bits
    * @param parity Communication parity mode
    * @param stopBits Number of stop bits
    * @param flowControl Communication flow control setting
    */
    static Connection connectToSerialDevice(
        const char* portName,
        uint32 baudRate,
        WbSerialDataBits dataBits,
        WbSerialParity parity,
        WbSerialStopBits stopBits,
        WbSerialFlowControl flowControl);

    /** Creates new connection to a TCP/IP device
    *
    * @param address IP address and port pair that should be connected.
    *        If port has not been specified, default Whiteboard port will be used.
    */
    static Connection connectToTcpIpDevice(const char* address);

    /** Creates new connection to a BLE device
    *
    * @param address BLE Mac address that should be connected.
    */
    static Connection connectToBleDevice(const char* address);

private:
    /** Implementation */
    class DeviceDiscovererImpl* mpImplementation;
};

} // namespace device_discovery
} // namespace whiteboard

