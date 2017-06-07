#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/comm/CommAdapter.h"
#include "whiteboard/comm/BufferPoolAllocator.h"
#include "whiteboard/comm/internal/ip/IpReceiver.h"

namespace whiteboard
{

// Forward declarations
class IpReceiver;

/** Class that implements UDP / IP protocol wrapper for whiteboard */
class UdpIpAdapter FINAL : public CommAdapter, private IpReceiver::IClient
{
private:
    /** Prevents use of the constructor
     *
     * @param socket Socket that should be used
     */
    UdpIpAdapter(WbIPv4UdpSocket socket);

public:
    /** Destructor */
    virtual ~UdpIpAdapter();

    /** Creates a new UDP adapter and binds it to specific interface address
     *
     * @param rInterfaceAddress Interface address to use
     * @param port UDP port to use
     * @return New UdpIpAdapter instance or NULL if adapter could not be created
     */
    static UdpIpAdapter* create(const WbIPv4Address& rInterfaceAddress, WbUdpPort port);

    /**
    *   Gets name of communication adapter
    *
    *   @return Name of the adapter
    */
    const char* getName() const OVERRIDE { return "UdpIp"; }

    /** Adds new route to the routing table that uses this adapter
    *
    * @param address Address of the remote end
    * @param serialNumber Serial number of the remote end
    * @param buddy A value indicating whether this should be the buddy route
    * @param rRouteHandle On output contains handle to routing table entry that was just added
    * @return Result of the operation
    */
    Result addRoute(
        const char* address,
        const char* serialNumber,
        bool buddy,
        RoutingTableEntryHandle& rRouteHandle) OVERRIDE;

    /** Adds new route to the routing table that uses this adapter
    *
    * @param rAddress IP address of the remote end
    * @param port UDP port of the remote end
    * @param serialNumber Serial number of the remote end
    * @param buddy A value indicating whether this should be the buddy route
    * @param rRouteHandle On output contains handle to routing table entry that was just added
    * @return Result of the operation
    */
    Result addRoute(
        const WbIPv4Address& rAddress,
        WbUdpPort port,
        const char* serialNumber,
        bool buddy,
        RoutingTableEntryHandle& rRouteHandle);

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
    * @return Result of the operation
    */
    Result disconnect(const Address& rDestination) OVERRIDE;

    /** Gets buffer allocator that should be used to allocate and deallocate
    * adapter buffers
    *
    * @return Buffer allocator instance
    */
    IBufferAllocator& getAllocator() OVERRIDE;

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

private:
    /** Adds current adapter to given socket set
    *
    * @param rSocketSet Socket set that indicates available data
    * @return A value indicating whether adapter was successfully added
    */
    bool addTo(WbIpSocketSet& rSocketSet) OVERRIDE;

    /** Removes current adapter from given socket set
    *
    * @param rSocketSet Socket set that indicates available data
    * @return A value indicating whether adapter was successfully removed
    */
    bool removeFrom(WbIpSocketSet& rSocketSet) OVERRIDE;

    /** Checks whether current adapter is in error state
    *
    * @param rSocketSet Socket set that indicates error condition
    * @return A value indicating whether socket is in error state
    */
    bool checkError(const WbIpSocketSet& rSocketSet) OVERRIDE;

    /** Notifies adapter of error state. */
    void error() OVERRIDE;

    /** Checks whether current adapter has data in socket set
    *
    * @param rSocketSet Socket set that indicates available data
    * @return A value indicating whether socket of adapter had data
    */
    bool checkData(const WbIpSocketSet& rSocketSet) OVERRIDE;

    /** Receives packet from the adapter. Called only when socket has pending data. */
    bool receive() OVERRIDE;

private:
#ifdef WB_UNITTEST_BUILD
    /** Buffer allocator */
    BufferPoolAllocator mBufferAllocator;
#endif

    /** Socket that is associated with this adapter */
    WbIPv4UdpSocket mSocket;
};

} // namespace whiteboard
