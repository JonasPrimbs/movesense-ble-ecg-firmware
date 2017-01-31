// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/devicediscovery/Device.h"

namespace whiteboard
{
namespace device_discovery
{

/**
* TcpIpDevice wraps instantiation and destruction of a TCP/IP port as well as
* required route table entry modifications.
*/
class TcpIpDevice final : public Device
{
public:
    /** Constructs a new instance of a TcpIpDevice class.
    *
    * @param address IP address and port pair that should be connected. 
    *        If port has not been specified, default Whiteboard port will be used.
    */
    TcpIpDevice(
        const char* address);

    /** Destructor */
    ~TcpIpDevice();

    /** Get a printable string of this device instance. */
    virtual std::string to_string() const override;

protected:
    /** Instantiates the communication interface associated with the device
    *
    * @return Communication interface handle
    */
    virtual ICommunicationInterface* createInterface() const override;

    /** Frees communication interface associated with the device
    *
    * @param pInterface Communication interface handle to free
    */
    virtual void freeInterface(ICommunicationInterface* pInterface) const override;
};

} // namespace device_discovery

} // namespace whiteboard
