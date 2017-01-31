//
//  BLECentral.h
//  Whiteboard
//
//  Created by Nikita Zhuk on 06/07/15.
//  Copyright (c) 2015 Suunto. All rights reserved.
//

#ifndef __Whiteboard__BLECentral__
#define __Whiteboard__BLECentral__

#include "whiteboard/devicediscovery/Device.h"

#include <string>
#include <vector>
#include <map>
#include <functional>

namespace whiteboard
{

// Callback typedefs
typedef std::function<void(const std::string& peripheralUUID, bool result)> BLECentralDidConnectToPeripheralCallback;
typedef std::function<void(const std::string& peripheralUUID, bool wasRemotelyInitiated)> BLECentralDidDisconnectToPeripheralCallback;
typedef std::function<void(const std::string& peripheralUUID, const void* data, int dataLength)> BLECentralDidReceiveDataCallback;
typedef std::function<void(const std::string& peripheralUUID, bool result)> BLECentralDidNotifyConnectToPeripheralCallback;
typedef std::function<void(const std::string& peripheralUUID, bool wasRemotelyInitiated)> BLECentralDidNotifyDisconnectToPeripheralCallback;

// Forward declarations
struct WBCentralImpl;

struct BLEDeviceInfo
{
    std::string serialNumber;
    std::string description;
    std::string device;
    std::string uuid;
};

/** Class that provides a C++11 wrapper for the WBCentral ObjC class.
 For full documentation of method names, @see WBCentral.h
 */
class BLECentral
{
public:
    ~BLECentral();
    BLECentral(const BLECentral&) = delete;            // Delete copy constructor
    BLECentral& operator=(BLECentral const&) = delete; // Delete copy assignment operator
    
    static BLECentral* getInstance();
    
    void connect(const std::string& peripheralUUID) noexcept;
    bool disconnect(const std::string& peripheralUUID) noexcept;
    void writeData(const std::string& peripheralUUID,
                   const void* bytes,
                   int byteLength,
                   std::function<void(bool)> completionCallback) noexcept;

    std::vector<BLEDeviceInfo> connectedDevices() noexcept;

    BLECentralDidConnectToPeripheralCallback didConnectToPeripheralCallback;
    BLECentralDidDisconnectToPeripheralCallback didDisconnectFromPeripheralCallback;
    BLECentralDidReceiveDataCallback didReceiveDataFromPeripheral;
    BLECentralDidNotifyConnectToPeripheralCallback didNotifyConnectToPeripheralCallback;
    BLECentralDidNotifyDisconnectToPeripheralCallback didNotifyDisconnectFromPeripheralCallback;

    std::unique_ptr<WBCentralImpl> mCentralImpl; // PIMPL to WBCentral ObjC object

private:
    BLECentral();
};

} // namespace whiteboard

#endif /* defined(__Whiteboard__BLECentral__) */
