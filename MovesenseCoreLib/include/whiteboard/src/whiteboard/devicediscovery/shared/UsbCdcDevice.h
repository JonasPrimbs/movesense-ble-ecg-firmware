// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/UsbCdcEnumerator.h"

namespace whiteboard {
namespace device_discovery {

/** UsbCdcDevice class implements Device interface for USB CDC serial ports. */
class UsbCdcDevice : public Device
{
protected:
    /** Class is constructed within enumerator */
    friend class UsbCdcEnumeratorImpl;
    UsbCdcDevice();

public:
    /** Vendor identifier */
    UsbVendorId mVid;

    /** Product identifier */
    UsbProductId mPid;

    /** Destructor */
    virtual ~UsbCdcDevice() {}

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
