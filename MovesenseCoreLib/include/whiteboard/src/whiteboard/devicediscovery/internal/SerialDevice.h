// Copyright (c) Suunto Oy 2016. All rights reserved.
#pragma once

#include "whiteboard/devicediscovery/DeviceBase.h"

namespace whiteboard {
namespace device_discovery {

/**
* SerialDevice wraps instantiation and destruction of a serial port as well as
* required route table entry modifications.
*/
class SerialDevice final : public DeviceBase
{
public:
    /** Constructs a new instance of a SerialDevice class.
    *
    * @param portDescriptor Port descriptor. A string with port parameters separated 
    *        with space or comma. Eg. COM1,9600,8,N,1,OFF
    *        Name of the port is required. Rest of the fields are optional and will
    *        be set to default values if value is not specified by the descriptor.
    */
    SerialDevice(const char* portDescriptor);

    /** Constructs a new instance of a SerialDevice class.
    *
    * @param portName Name of the serial port to use
    * @param baudRate Communication baud rate
    * @param dataBits Number of data bits
    * @param parity Communication parity mode
    * @param stopBits Number of stop bits
    * @param flowControl Communication flow control setting
    */
    SerialDevice(
        const char* portName,
        uint32 baudRate,
        WbSerialDataBits dataBits,
        WbSerialParity parity,
        WbSerialStopBits stopBits,
        WbSerialFlowControl flowControl);

    /** Destructor */
    ~SerialDevice();

protected:
    /** Instantiates the communication interface associated with the device
    *
    * @return Communication interface handle
    */
    virtual CommAdapter* createInterface() const override;

    /** Frees communication interface associated with the device
    *
    * @param pInterface Communication interface handle to free
    */
    virtual void freeInterface(CommAdapter* pInterface) const override;
};

} // namespace device_discovery
} // namespace whiteboard
