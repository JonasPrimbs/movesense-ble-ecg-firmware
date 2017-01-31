// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/BleEnumerator.h"

namespace whiteboard {
namespace device_discovery {

/** BleDevice class implements Device interface for BLE ports. */
class BleDevice : public Device
{
protected:
    /** Class is constructed within enumerator */
    friend class BleEnumeratorImpl;
    BleDevice();

public:
    /** Destructor */
    virtual ~BleDevice() {}

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
