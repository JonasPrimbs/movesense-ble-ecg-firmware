// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/UsbCdcEnumerator.h"
#include "whiteboard/devicediscovery/DeviceBase.h"

namespace whiteboard {
namespace device_discovery {

/** UsbCdcDevice class implements Device interface for USB CDC serial ports. */
class UsbCdcDevice : public DeviceBase
{
public:
    /** Vendor identifier */
    UsbVendorId mVid;

    /** Product identifier */
    UsbProductId mPid;

    /** Constructor */
    UsbCdcDevice();

    /** Destructor */
    virtual ~UsbCdcDevice() {}

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
