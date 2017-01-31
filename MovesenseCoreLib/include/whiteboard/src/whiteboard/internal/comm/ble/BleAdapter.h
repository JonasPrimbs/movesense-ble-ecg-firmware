#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/CommAdapter.h"
#ifdef TARGET_OS_IPHONE
#include "whiteboard/internal/comm/ble_ios/BleAdapter_iOS.h"
#else

namespace whiteboard
{

/** Class that implements BLE wrapper for whiteboard */
class BleAdapter FINAL : public CommAdapter
{
private:
    /** Prevents the use of the constructor */
    BleAdapter();

public:
    /** Destructor */
    virtual ~BleAdapter();

    /** Creates a new BleAdapter
     *
     * @return New BleAdapter instance or NULL if adapter could not be created
     */
    static BleAdapter* create();

    /**
    *   Gets name of communication adapter
    *
    *   @return Name of the adapter
    */
    const char* getName() const OVERRIDE { return "Ble"; }

    /** Adds a new route to the routing table that uses this interface
    *
    * @param address Address of the remote end
    * @param serialNumber Serial number of the remote end
    * @param buddy A value indicating whether this should be the buddy route
    * @param rRouteHandle On output contains handle to routing table entry that was just added
    * @return Result of the operation
    */
    Result addRoute(const char* address, const char* serialNumber, bool buddy, RoutingTableEntryHandle& rRouteHandle) OVERRIDE FINAL;

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

    /** Cancels an on going connection attempt
    *
    * @param rDestination Address of the destination
    * @param reason Reason for the cancellation
    * @return Result of the operation
    */
    Result cancelConnect(const Address& rDestination, Result reason) OVERRIDE;

    /** Disconnects the adapter from destination
    *
    * @param rDestination Address of the destination
    * @return Result of the operation
    */
    Result disconnect(const Address& rDestination) OVERRIDE;

    /** Gets buffer allocator used to allocate and deallocate adapter buffers
     *
     * @return Buffer allocator instance
     */
    IBufferAllocator& getAllocator() const OVERRIDE;

    /** Sends a message using the adapter to specified destination
     *
     * @param rDestination Message destination
     * @param pBuffer Buffer that contains the message
     * @return Result of the operation
     */
    Result send(const Address& rDestination, Buffer* pBuffer) OVERRIDE;

    /** Cancels a send request
     *
     * @param pBuffer Buffer that contains the message that should be canceled
     * @param reason Reason for the cancellation
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

public:
    using CommAdapter::onConnectCompleted;
    using CommAdapter::onDisconnectCompleted;
    using CommAdapter::onDataReceived;
    using CommAdapter::onSendCompleted;
};

} // namespace whiteboard
#endif
