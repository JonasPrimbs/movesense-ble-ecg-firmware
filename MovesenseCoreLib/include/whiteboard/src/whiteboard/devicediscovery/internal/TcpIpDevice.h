// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/devicediscovery/DeviceBase.h"

namespace whiteboard {
namespace device_discovery {

/**
* TcpIpDevice wraps instantiation and destruction of a TCP/IP port as well as
* required route table entry modifications.
*/
class TcpIpDevice final : public DeviceBase
{
public:
    /** Constructs a new instance of a TcpIpDevice class.
    *
    * @param address IP address and port pair that should be connected. 
    *        If port has not been specified, default Whiteboard port will be used.
    */
    TcpIpDevice(const char* address);

    /** Destructor */
    ~TcpIpDevice();

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
