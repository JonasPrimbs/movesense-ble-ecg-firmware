#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/CommAdapter.h"
#include "whiteboard/internal/comm/bus/IBusCommunication.h"

namespace whiteboard
{

/** Class that implements whiteboard adapter for communication links
 * that implement IBusCommunication interface */
class BusAdapter FINAL : public CommAdapter, private IBusCommunication::IBusCallback
{

private:
    /** Constructor
     *
     * @param rAdapterTraits Structure that describes adapter properties
     */
    BusAdapter(const Traits& rAdapterTraits);

public:
    /** Destructor */
    virtual ~BusAdapter();

    /** Type of function that constructs the bus communication link
     *
     * @param rCallbacks Callbacks for the data transfer operations
     * @return New instance of the IBusCommunication class implementation
     */
    typedef IBusCommunication* (*CreateFunc)(IBusCommunication::IBusCallback& rCallbacks);

    /** Creates a new BusAdapter
     *
     * @param rAdapterTraits Structure that describes adapter properties
     * @param createFunc Function that constructs the communication link
     * @return New BusAdapter instance or NULL if adapter could not be created
     */
    static BusAdapter* create(const Traits& rAdapterTraits, CreateFunc createFunc);

    /**
    * Gets name of communication adapter
    *
    * @return Name of the adapter
    */
    const char* getName() const OVERRIDE { return "Bus"; }

    /** Adds a new route to the routing table that uses this adapter
    *
    * @param address Address of the remove end (Set to NULL for BusAdapter routes)
    * @param serialNumber Serial number of the remote end
    * @param buddy A value indicating whether this should be the buddy route
    * @param rRouteHandle On output contains handle to routing table entry that was just added
    * @return Result of the operation
    */
    Result addRoute(const char* address,
                    const char* serialNumber,
                    bool buddy,
                    RoutingTableEntryHandle& rRouteHandle) OVERRIDE;

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
    IBufferAllocator& getAllocator() const OVERRIDE;

    /** Sends a message using the communication adapter to specified destination
    *
    * @param rDestination Message destination
    * @param pBuffer pBuffer Buffer that contains the message
    * @return Result of the operation
    */
    Result send(const Address& rDestination, Buffer* pBuffer) OVERRIDE;

    /** Cancels an on going send request
    * @from IBusCommunication::IBusCallback
    * @param pBufferP Buffer that contains the message that should be canceled
    * @param reason Reason for the cancelation
    * @return Result of the operation
    */
    Result cancelSend(Buffer* pBuffer, Result reason) OVERRIDE;

    /** Callback when data has been received by the communication link
    * @from IBusCommunication::IBusCallback
    * @note Client is responsible for freeing the buffer by calling freeBuffer method
    *
    * @param pBuffer Buffer that was received
    */
    void dataReceived(Buffer* pBuffer) OVERRIDE;

    /** Callback that is called when data buffer has actually been sent
    * @from IBusCommunication::IBusCallback
    * @param pBuffer Buffer that was sent
    * @param resultCode Result of send operation
    */
    void dataSent(Buffer* pBuffer, Result resultCode) OVERRIDE;

    /** Callback for changes in bus's link state
    *
    * @param aState Bus's Link state changed either by explicitly invoked enable() /disable()
    * Or via internal bus specific discovery mechanism.
    */
    void linkStateChanged(enum IBusCommunication::BusCommunicationLinkState aState) OVERRIDE;

    /** Formats the adapter address to human readable form
    *
    * @param rAddress Address that should be formatted
    * @param bufferSize Maximum number of charactes in the buffer including zero terminator
    * @param addressBuffer Buffer where address should be written
    * @return Result of the operation
    */
    Result formatAddress(const Address& rAddress, size_t bufferSize, char* addressBuffer) OVERRIDE;

private:
    /** Bus communication implementation */
    IBusCommunication* mpCommunicationLink;
};

} // namespace whiteboard
