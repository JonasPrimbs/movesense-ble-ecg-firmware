#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <whiteboard/comm/CommAdapter.h>

namespace whiteboard {

// Forward declarations
class BLECentral;

namespace comm_ble {

/** Class that implements iOS BLE wrapper for whiteboard */
class BleAdapter FINAL : public CommAdapter
{
public:
    /** Constructor */
    BleAdapter();
    
    /** Destructor */
    virtual ~BleAdapter();
    
    /**
    * Gets name of communication adapter
    *
    * @return Name of the adapter
    */
    const char* getName() const OVERRIDE { return "Ble"; }
    
    /** Adds new route to the routing table that uses this interface
     *
     * @param address Address of remote end
     * @param serialNumber Serial number of the remote end
     * @param buddy A value indicating whether this should be the buddy route
     * @param rRouteHandle On output contains handle to routing table entry that was just added
     * @return Result of the operation
     */
    Result addRoute(const char* address, const char* serialNumber, bool buddy, RoutingTableEntryHandle& rRouteHandle) OVERRIDE;

    /** Enables the adapter
    *
    * @return Result of the operation
    */
    Result enableAdapter() OVERRIDE;

    /** Disables the adapter
    *
    * @return Result of the operation
    */
    Result disableAdapter() OVERRIDE;

    /** Connects the adapter to destination
    *
    * @param rDestination Address of the destination
    * @return Result of the operation
    */
    Result connect(const Address& rDestination) OVERRIDE;

    /** Disconnects the adapter from destination
    *
    * @param rDestination Address of the destination
    * @return Result of the operation
    */
    Result disconnect(const Address& rDestination) OVERRIDE;
    
    /** Cancels an on going connection attempt
     *
     * @param rDestination Address of the destination
     * @param reason Reason for the cancellation
     * @return Result of the operation
     */
    Result cancelConnect(const Address& rDestination, Result reason) OVERRIDE;

    /** Sends a message using the communication adapter to specified destination
    *
    * @param rDestination Message destination
    * @param pBuffer pBuffer Buffer that contains the message
    * @return Result of the operation
    */
    Result send(const Address& rDestination, Buffer* pBuffer) OVERRIDE;

    /** Cancels an on going send request
    *
    * @param pBufferP Buffer that contains the message that should be canceled
    * @param reason Reason for the cancelation
    * @return Result of the operation
    */
    Result cancelSend(Buffer* pBuffer, Result reason) OVERRIDE;

    /** Formats the adapter address to human readable form
    *
    * @param rAddress Address that should be formatted
    * @param bufferSize Maximum number of charactes in the buffer including zero terminator
    * @param addressBuffer Buffer where address should be written
    * @return Result of the operation
    */
    Result formatAddress(const Address& rAddress, size_t bufferSize, char* addressBuffer) OVERRIDE;

private:
    /** BLE Central implementation */
    BLECentral *mBLECentral;
};

} // namespace comm_ble
} // namespace whiteboard
