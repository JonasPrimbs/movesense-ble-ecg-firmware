#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/Result.h"
#include "whiteboard/RoutingTableEntryHandle.h"

WB_HEADER_CHECK_DEFINE(WB_HAVE_CUSTOM_BUFFER_POOLS);

namespace whiteboard
{

// Forward declarations
struct Buffer;
class IBufferPool;
class WhiteboardCommunication;

/** Type of buffer that stores communication addresses */
typedef uint8 Address[WB_COMM_MAX_ADDRESS_LENGTH];

/** Empty address */
static const Address NULL_ADDRESS = { 0 };

/** Base class for communication protocol adapters */
class WB_API CommAdapter : public IDynamicallyAllocatable
{
public:
    /** CommAdapter traits */
    struct Traits
    {
        /** Default connect timeout in milliseconds. */
        size_t defaultConnectTimeoutMs;

        /** Default send timeout in milliseconds */
        size_t defaultSendTimeoutMs;

        /** A value indicating whether the communications channel is using internal keep alive
         * messaging. When this bit is set, Whiteboard avoids sending unnecessary keep alive messages itself.
         */
        uint8 internalKeepAlive : 1;

        /** A value indicating whether the connect operations using this adapter can succeed.
        */
        uint8 canConnect : 1;

        /** A value indicating whether the connect operation on communications channel can succeed only in one
        * direction. When this bit is set, remote Whiteboard cannot connect to this Whiteboard.
        */
        uint8 unidirectionalConnect : 1;

        /** A value indicating whether the communications channel is sufficiently well encrypted.
        * AES CCM 128-bit or similar strength. When this bit is set, Whiteboard avoids doing
        * unnecessary message encryption itself.
        */
        uint8 encrypted : 1;

        /** Number of bytes to reserve for communication layer headers */
        uint8 payloadOffset;
    };

    /** Constructor
     *
     * @param rAdapterTraits Communication adapter traits
     * @param pCommunication WhiteboardCommunication instance to associate this adapter. Used for unittest mockups.
     */
    CommAdapter(
        const Traits& rAdapterTraits, 
        WhiteboardCommunication* pCommunication = NULL);

    /** Destructor */
    virtual ~CommAdapter();

    /** Increments reference count of the adapter */
    inline void incrementReferenceCount();

    /** Decrements reference count of the adapter. If reference 
    * count reaches zero, adapter is freed.
    */
    inline void decrementReferenceCount();

    /** Gets Whiteboard communication instance associated with this adapter */
    inline WhiteboardCommunication& getCommunication();

    /** Enables the adapter
    *
    * @return Result of the operation
    */
    Result enable();

    /** Disables the adapter
    *
    * @param removeAdapterRoutes A value indicating whether all routes should be removed
    *                            from the routing table
    * @return Result of the operation
    */
    Result disable(bool removeAdapterRoutes);

    /**
    *   Gets name of communication adapter
    *
    *   @return Name of the adapter
    */
    virtual const char* getName() const = 0;

    /** Adds new route to the routing table that uses this adapter
    *
    * @param address Address of remote end
    * @param serialNumber Serial number of the remote end
    * @param buddy A value indicating whether this should be the buddy route
    * @param rRouteHandle On output contains handle to routing table entry that was just added
    * @return Result of the operation
    */
    virtual Result addRoute(
        const char* address, const char* serialNumber, bool buddy, RoutingTableEntryHandle& rRouteHandle) = 0;

    /** Adds new route to the routing table
    *
    * @param serialNumber Serial number of destination whiteboard
    * @param rAddress Communication interface specific destination address
    * @param lifeTime Life time of the route. Use WB_INVALID_TIMESTAMP to keep the route until it is manually removed.
    * @param buddy A value indicating whether this route should be set as the buddy route.
    * @return Handle of new route
    */
    RoutingTableEntryHandle addRoute(
        const SuuntoSerial& rSerialNumber,
        const Address& rAddress,
        WbTimestamp lifeTime,
        bool buddy);

    /** Removes a route from the routing table.
    *
    * @param rRouteHandle Handle to the previously added route.
    * @return Result of the operation
    */
    Result removeRoute(const RoutingTableEntryHandle& rRouteHandle);

    /** Removes a route from the routing table.
    *
    * @param rAddress Address of the routing table entry to remove
    * @return Result of the operation.
    */
    Result removeRoute(const Address& rAddress);

    /** Removes all routes of the adatapeter from the routing table.
    */
    void removeRoutes();

    /** Communication adapter states */
    enum State
    {
        /** Communication adapter has been disabled */
        STATE_DISABLED,

        /** Communication adapter is working */
        STATE_ENABLED,

        /** Communication adapter is in erroneous state and should not be used */
        STATE_ERROR
    };

    /** Gets current state of the communication adapter
     *
     * @return Current state
     */
    inline State getState() const;

    /** Gets communication adapter trais
    *
    * @see Traits
    * @return Reference to adapter traits
    */
    inline const Traits& getTraits() const;

    /** Connects the adapter to destination
    *
    * @param rDestination Address of the destination
    * @return Result of the operation
    */
    virtual Result connect(const Address& rDestination) = 0;

    /** Cancels an on going connection attempt. Implementation should be synchronous.
    *
    * @param rDestination Address of the destination
    * @param reason Reason for the cancellation
    * @return Result of the operation
    */
    virtual Result cancelConnect(const Address& rDestination, Result reason) = 0;

    /** Disconnects the adapter from destination
    *
    * @param rDestination Address of the destination
    * @return Result of the operation
    */
    virtual Result disconnect(const Address& rDestination) = 0;

    /** Gets buffer pool that should be used to allocate and deallocate
     * adapter buffers
     *
     * @return Buffer pool instance
     */
#ifdef WB_HAVE_CUSTOM_BUFFER_POOLS
    virtual
#endif
    IBufferPool& getBufferPool();

    /** Sends a message using the communication adapter to specified destination
     *
     * @param rDestination Message destination
     * @param pBuffer Buffer that contains the message
     * @return Result of the operation
     */
    virtual Result send(const Address& rDestination, Buffer* pBuffer) = 0;

    /** Cancels an on going send request
     *
     * @param pBufferP Buffer that contains the message that should be canceled
     * @param reason Reason for the cancellation
     * @return Result of the operation
     */
    virtual Result cancelSend(Buffer* pBuffer, Result reason) = 0;

    /** Formats the adapter address to human readable form
    *
    * @param rAddress Address that should be formatted
    * @param bufferSize Maximum number of charactes in the buffer including zero terminator
    * @param addressBuffer Buffer where address should be written
    * @return Result of the operation
    */
    virtual Result formatAddress(const Address& rAddress, size_t bufferSize, char* addressBuffer) = 0;

protected:
    /** Enables the interface
    *
    * @return Result of the operation
    */
    virtual Result enableAdapter() = 0 ;

    /** Disables the interface
    *
    * @return Result of the operation
    */
    virtual Result disableAdapter() = 0;

protected:
    /** Call this when communication adapter encounters an error that cannot be
    * resolved without reseting hardware
    */
    void onError();

    /** Call this when Whiteboard initiated CommAdapter::connect has completed
    * asynchronously (it has returned HTTP_CODE_ACCEPTED).
    *
    * @param rDestination Connect destination
    * @param resultCode Result code of the operation
    */
    void onConnectCompleted(const Address& rDestination, Result resultCode);

    /** Call this when Whiteboard initiated CommAdapter::disconnect has completed
    * asynchronously (it has returned HTTP_CODE_ACCEPTED).
    *
    * @param rDestination Disconnect destination
    * @param resultCode Result code of the operation
    */
    void onDisconnectCompleted(const Address& rDestination, Result resultCode);

    /** Called when adapter has received data
    *
    * @param rSource Sender of the message
    * @param pBuffer Buffer that contains the message
    * @return Result of the operation
    */
    Result onDataReceived(const Address& rSource, Buffer* pBuffer);

    /** Called when adapter has completed asynchronous data send operation
    *
    * @param pBuffer Buffer that contains the message
    * @param resultCode Result code of the operation
    */
    void onSendCompleted(Buffer* pBuffer, Result resultCode);

private:
    /** Enables routes associated with this communication adapter that are in disabled state
    *
    * @return Result of the operation
    */
    Result enableRoutes();

    /** Disables routes associated with this communication adapter
    *
    * @param gracefully A value indicating whether the routes should be closed gracefully i.e.
    *                   whether communication interface's disconnect should be called
    * @return Result of the operation
    */
    Result disableRoutes(bool gracefully);

private:
    /** WhiteboardCommunication uses these directly */
    friend class WhiteboardCommunication;

    /** Next registered communication adapter in WhiteboardCommunication's linked list */
    CommAdapter* mpNextAdapter;

private:
    /** Communication adapter traits */
    const Traits& mrTraits;

    /** Whiteboard communication instance associated with this adapter */
    WhiteboardCommunication& mrCommunication;

    /** Current reference count */
    uint8 mReferenceCount;

    /** Current adapter state */
    uint8 mState;
};

inline void CommAdapter::incrementReferenceCount()
{
    ++mReferenceCount;
    WB_DEBUG_ASSERT(mReferenceCount > 0);
}

inline void CommAdapter::decrementReferenceCount()
{
    WB_DEBUG_ASSERT(mReferenceCount > 0);
    if (--mReferenceCount == 0)
    {
        delete this;
    }
}

inline WhiteboardCommunication& CommAdapter::getCommunication()
{
    return mrCommunication;
}

inline CommAdapter::State CommAdapter::getState() const
{
    return static_cast<State>(mState);
}

inline const CommAdapter::Traits& CommAdapter::getTraits() const
{
    return mrTraits;
}

} // namespace whiteboard
