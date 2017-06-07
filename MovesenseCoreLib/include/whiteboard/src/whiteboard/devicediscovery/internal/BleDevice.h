#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#include "whiteboard/devicediscovery/DeviceBase.h"

namespace whiteboard {
namespace device_discovery {

/**
* BleDevice wraps instantiation and destruction of a BLE adapter as well as
* required route table entry modifications.
*/
class BleDevice final : public DeviceBase
{
public:
    /** Constructs a new instance of a BleDevice class.
    *
    * @param address BLE Mac address that should be connected. 
    */
    BleDevice(const char* address);

    /** Destructor */
    ~BleDevice();

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
