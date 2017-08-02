#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/integration/port.h"
#include "whiteboard/comm/CommAdapter.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/RoutingTableEntryHandle.h"
#include "whiteboard/containers/BitmapPoolAllocator.h"
#include "whiteboard/containers/TypedPool.h"
#include "whiteboard/internal/Status.h"
#include "whiteboard/internal/protocol/Messages.h"
#include "whiteboard/internal/protocol/IProtocol.h"
#include "whiteboard/comm/internal/IRoutingTableObserver.h"
#include "wb-resources/resources.h"

namespace whiteboard
{

// Forward declarations
class IBufferAllocator;
class WhiteboardCommunication;

/** Routing table entry details */
struct RoutingTableEntry
{
    /** Serial number of the destination whiteboard */
    SuuntoSerial serialNumber;

    /** Version of the routing table entry. Used to
     * check if routing table entry has been reassigned. */
    volatile uint8 version;

    /** Protocol version to use when communicating with target.
    *
    * @note This is not minimum nor maximum protocol version supported by
    * target, but something that the both sides can use
    */
    ProtocolVersion protocolVersion;

    /** State of the route */
    volatile uint8 state;

    /** A value indicating whether keep alives should be sent for the route */
    uint8 sendKeepAlives : 1;

    /** A value indicating whether not connected routes can be connected */
    uint8 connectable : 1;

    /** A value indicating whether disconnected routes can be reconnected */
    uint8 reconnectable : 1;

    /** A value indicating whether data should be encrypted */
    uint8 encryptData : 1;

    /** Timeout of current state */
    WbTimestamp stateTimeout;

    /** Timestamp of last sent message. This is used for sending keep alive messages. */
    WbTimestamp lastMessageSent;

    /** Life time of routing table entry. This is used
    * to remove outdated routing table entries. */
    WbTimestamp lifeTime;

    /** Whiteboard version info of the remote whiteboard */
    WhiteboardVersion whiteboardVersion;

    /** ID of remote whiteboard in local scope */
    WhiteboardId remoteWhiteboardIdInLocalScope;

    /** ID of local whiteboard in remote scope */
    WhiteboardId localWhiteboardIdInRemoteScope;

    /** Sequence number of last route discovery */
    SequenceNumber sequenceNumber;

    /** Number of hops to reach the destination */
    HopCount numberOfHops;

    /** List of commands */
    enum RoutingTableEntryCommand
    {
        /** Do nothing */
        RTE_CMD_NONE,

        /** Disable adapter */
        RTE_CMD_DISABLE_ADAPTER
    };

    /** Routing table entry related command. @see RoutingTableEntryCommand */
    uint8 command;

    union
    {
#ifdef WB_HAVE_COMM_ROUTING
        struct
        {
            /** Next hop on route to destination.
             * Valid only if numberOfHops is greater than zero. */
            RoutingTableEntryHandle routingTableEntryHandle;
        } nextHop;
#endif

        struct
        {
            /** Network interface that should be used to reach the destination.
             * Valid only if numberOfHops is zero. */
            CommAdapter* pInterface;

            /** Network interface specific destination address.
            * Valid only if numberOfHops is zero. */
            Address address;
        } destination;
    };

    /** Default constructor */
    RoutingTableEntry();

    /** Gets handle of the routing table entry
     *
     * @return Handle to routing table entry
     */
    RoutingTableEntryHandle getHandle();

    /** Gets next hop routing table entry 
     *
     * @note Use this only when routing table is locked
     * @return Reference to next hop routing table entry (this might be this routing table entry) 
     */
    RoutingTableEntry& getNextHop();
};

/** Routing table */
class RoutingTable
{
public:
    /** Routing table index type */
    typedef uint8 Index;

    /** Constructor
     *
     * @param rMutexHandle Handle of the mutex that should be used to synchronize
     * @param size Size of the pool
     */
    RoutingTable(WbRecursiveMutexHandle& rMutexHandle, Config::PoolSize size);

    /** Destructor */
    ~RoutingTable();

    /** Attaches new observer to the routing table.
    *
    *	@param rObserver Routing table observer instance.
    */
    void attach(IRoutingTableObserver& rObserver);

    /** Detaches observer from the routing table.
    *
    *	@param rObserver Routing table observer instance.
    */
    void detach(IRoutingTableObserver& rObserver);

private:
    /** Locks the routing table */
    inline void lock();

    /** Unlocks the routing table */
    inline void unlock();

public:
    /**
    * Gets routing table entry with specified whiteboard ID (local scope)
    *
    * @param whiteboardId ID of a whiteboard (local scope)
    * @return Routing table entry with specified whiteboard ID or NULL if it was not found
    */
    RoutingTableEntryHandle getEntry(WhiteboardId whiteboardId);

    /**
    * Gets routing table entry with specified serial number
    *
    * @param rSerial Serial number of a whiteboard
    * @return Routing table entry with specified serial number or NULL if it was not found
    */
    RoutingTableEntryHandle getEntry(const SuuntoSerial& rSerial);

    /**
    * Gets routing table entry with specified target address
    *
    * @param rAdapter Communication adapter associated with the address
    * @param rAddress Communication interface specific address
    * @return Routing table entry with specified serial number or NULL if it was not found
    */
    RoutingTableEntryHandle getEntry(const CommAdapter& rAdapter, const Address& rAddress);

    /** Removes all routes from the routing table.
    * Invalidates all data returned by @ref addRoute. */
    void clear();

    /**
    *	Function to get list of current serial numbers in routing table
    *
    *	@param pSerialNumbers Pointer to allocated list of strings for serial numbers.
    *   @param continuationIndex Index where to start the copy
    *   @param maxCount Maximum number of items to copy
    *	@param rSerialNumberCount Reference to variable to hold number of entries in routing table.
    *	@return A value indicating whether the routing table had more entries that could not fit in given array.
    */
    bool getSerialNumbers(
        SuuntoSerial::UnpackedSerialNumber* pSerialNumbers,
        Index continuationIndex,
        Index maxCount,
        Index& rSerialNumberCount);

    /**
    *   Gets serial number of requested remote whiteboard.
    *
    *   @param remoteWhiteboardId Remote whiteboard ID to search from the routing table.
    *   @param serialNumberBuffer Buffer to copy the plain serial number of the routing table entry
    *   @return Result of the operation.
    */
    Result getSerialNumber(WhiteboardId remoteWhiteboardId, SuuntoSerial::UnpackedSerialNumber serialNumberBuffer);

#if WB_HAVE_NETWORK_INFO
    /**
    *   Gets route state of requested remote whiteboard.
    *
    *   @param remoteWhiteboardId Remote whiteboard ID to search from the routing table.
    *   @param rRouteState On output contains state of the route
    *   @return Result of the operation.
    */
    bool getState(WhiteboardId remoteWhiteboardId, WB_RES::RouteState& rRouteState);

    /**
    *   Gets adapter specific next hop address in human readable form of requested remote whiteboard.
    *
    *   @param remoteWhiteboardId Remote whiteboard ID to search from the routing table.
    *   @param bufferSize Maximum number of charactes in the buffer including zero terminator
    *   @param addressBuffer Buffer where address should be written
    *   @return Result of the operation.
    */
    Result getAdapterSpecificAddress(WhiteboardId remoteWhiteboardId, size_t bufferSize, char* addressBuffer);

    /**
    *   Fills WB_RES generated routing table entry resource of requested remote whiteboard.
    *
    *   @param remoteWhiteboardId Remote whiteboard ID to search from the routing table.
    *   @param serialNumberBuffer Buffer to copy the plain serial number of the routing table entry
    *   @param rProviderEntry The routing table entry
    *   @return Result of the operation.
    */
    Result getRoutingTableEntryAsProviderEntry(
        WhiteboardId remoteWhiteboardId,
        SuuntoSerial::UnpackedSerialNumber serialNumberBuffer,
        WB_RES::RoutingTableEntry& rProviderEntry);
#endif
    /** Adds new route to the routing table
    *
    * @param serialNumber Serial number of destination whiteboard
    * @param pInterface Communication interface
    * @param rAddress Communication interface specific destination address
    * @param lifeTime Life time of the route. Use WB_INVALID_TIMESTAMP to keep the route until it is manually removed.
    * @param buddy A value indicating whether this route should be set as the buddy route.
    * @return Handle of new route
    */
    RoutingTableEntryHandle addRoute(
        const SuuntoSerial& rSerialNumber,
        CommAdapter* pInterface,
        const Address& rAddress,
        WbTimestamp lifeTime,
        bool buddy);

    /** Adds new or updates existing route
    *
    * @param serialNumber Serial number of destination whiteboard
    * @param rNextHopSerialNumber Serial number of next hop on route to destination
    * @param numberOfHops Number of hops on route to destination
    * @param sequenceNumber Sequence number of route discovery. If route exists with smaller sequence number, route is not
    * updated.
    * @param lifeTime Life time of the route. Use WB_INVALID_TIMESTAMP to keep the route until it is manually removed.
    * @param buddy A value indicating whether this route should be set as the buddy route.
    * @return Handle of new route
    */
    RoutingTableEntryHandle addRoute(
        const SuuntoSerial& rSerialNumber,
        const SuuntoSerial& rNextHopSerialNumber,
        HopCount numberOfHops,
        SequenceNumber sequenceNumber,
        WbTimestamp lifeTime);

    /** Adds new or updates existing route
    *
    * @param rSerialNumber Serial number of destination whiteboard
    * @param pInterface Communication interface
    * @param rAddress Communication interface specific destination address
    * @param rNextHop Next hop on route to destination
    * @param numberOfHops Number of hops on route to destination
    * @param sequenceNumber Sequence number of route discovery. If route exists with smaller sequence number, route is not
    * updated.
    * @param lifeTime Life time of the route. Use WB_INVALID_TIMESTAMP to keep the route until it is manually removed.
    * @param buddy A value indicating whether this route should be set as the buddy route.
    * @return Handle of new route
    */
    RoutingTableEntryHandle addRoute(
        const SuuntoSerial& rSerialNumber,
        CommAdapter* pInterface,
        const Address& rAddress,
        const RoutingTableEntryHandle& rNextHop,
        HopCount numberOfHops,
        SequenceNumber sequenceNumber,
        WbTimestamp lifeTime);

    /** Removes a route from the routing table
    *
    * @param rHandle Handle of the route that should be removed
    * @param pAdapter Optional pointer to communication adapter to use it in sanity checks
    * @return A value indicating whether the route was removed
    */
    Result removeRoute(const RoutingTableEntryHandle& rHandle, const CommAdapter* pAdapter);

    /** Removes route from the routing table
    *
    * @note Expects that routing table is already locked
    *
    * @param rEntry Route that should be removed
    */
    void removeRoute(RoutingTableEntry& rEntry);

    /** Removes all routes from the routing table
    *
    * @param rAdapter Adapter which routes should be enabled
    * @return HTTP_CODE_NOT_FOUND if there were no routes to remove. HTTP_CODE_OK otherwise.
    */
    Result removeRoutes(const CommAdapter& rAdapter);

    /** Checks whether routing table is full */
    bool isFull() const;

    /** Enables routes associated with given communication adapter that are in disabled state
    *
    * @param rAdapter Adapter which routes should be enabled
    * @return Result of the operation
    */
    Result enableRoutes(const CommAdapter& rAdapter);

    /** Disables all routes associated with given communication adapter
    *
    * @param rAdapter Adapter which routes should be enabled
    * @param gracefully A value indicating whether the routes should be closed gracefully i.e.
    *                   whether communication interface's disconnect should be called
    * @return Result of the operation
    */
    Result disableRoutes(const CommAdapter& rAdapter, bool gracefully);

    /** Notifies observer(s) of routing table changes
    *
    *	@param rEntry Changed routing table entry
    *	@param type Type of the notification
    *   @param previousState Previous state of the route
    */
    void notify(
        const RoutingTableEntry& rEntry,
        IRoutingTableObserver::RoutingTableNotification type,
        WB_RES::RouteState previousState);

    /**
    * Gets routing table entry to whiteboard that shares namespace of local whiteboard
    *
    * @return Routing table entry of buddy whiteboard
    */
    RoutingTableEntryHandle getBuddy();

    /** Gets ID of whiteboard that is associated with the routing table entry
    *
    * @param rHandle Handle of the routing table entry
    * @note This function returns ID of the whiteboard at the time
    * the routing table entry was assigned. Current ID may have changed
    * since then.
    * @return ID of the whiteboard in local scope
    */
    inline WhiteboardId getWhiteboardId(const RoutingTableEntryHandle& rHandle) const;

    /** Type for routing table entries */
    typedef TypedPool<Index, RoutingTableEntry> EntryPool;

    /** Get iterator to routing table contents */
    inline EntryPool::Iterator begin();

private:
    /** Helper to convert routing table index to Whiteboard ID
    *
    * @param index Routing table index
    * @return Whiteboard ID
    */
    static inline WhiteboardId indexToWhiteboardId(Index index);

    /** Helper to convert whiteboard ID to routing table index
    *
    * @param remoteWhiteboardId Whiteboard ID
    * @return Routing table index
    */
    static inline Index whiteboardIdToIndex(WhiteboardId remoteWhiteboardId);

private:
    /** Allocator for routing table entry pool */
    BitmapPoolAllocator mAllocator;

    /** Routing table entries */
    EntryPool mEntries;

    /** Mutex that protects the internal data */
    WbRecursiveMutexHandle& mrMutexHandle;

    /** Buddy whiteboard entry */
    RoutingTableEntry* mpBuddy;

    /** Number of attached routing table observers */
    size_t mNumberOfObservers;

    /** Table holding pointers to routing table observers */
    IRoutingTableObserver* mObservers[WB_COMM_MAX_NUMBER_OF_ROUTING_TABLE_OBSERVERS];
};

inline WhiteboardId RoutingTable::indexToWhiteboardId(RoutingTable::Index index)
{
    return static_cast<WhiteboardId>(index + 1);
}

inline RoutingTable::Index RoutingTable::whiteboardIdToIndex(WhiteboardId remoteWhiteboardId)
{
    return static_cast<RoutingTable::Index>(remoteWhiteboardId - 1);
}

inline void RoutingTable::lock()
{
    WbRecursiveMutexLock(mrMutexHandle);
}

inline void RoutingTable::unlock()
{
    WbRecursiveMutexUnlock(mrMutexHandle);
}

inline WhiteboardId RoutingTable::getWhiteboardId(const RoutingTableEntryHandle& rHandle) const
{
    return indexToWhiteboardId(mEntries.getKey(*rHandle.pEntry));
}

inline RoutingTable::EntryPool::Iterator RoutingTable::begin()
{
    return mEntries.begin();
}

} // namespace whiteboard
