#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/internal/BuildConfig.h"

#include "whiteboard/comm/CommAdapter.h"
#include "whiteboard/internal/BufferPool.h"
#include "whiteboard/comm/internal/RoutingTable.h"
#include "whiteboard/comm/internal/WhiteboardCommunication.h"
#include <cstdlib>

#if WB_UNITTEST_BUILD

namespace whiteboard
{

class CommAdapterMockup;

/** Class that implements message counting for comm adapter mockups */
class CommAdapterMockupMessageCounter
{
public:
    /** Action information
    *
    * @note When message counter matches action list, it will use EVERY action that matches the filter.
    */
    struct Action
    {
        /** Type of the action */
        enum
        {
            INCREMENT_ON_SEND, /** Increment counter value on send */
            INCREMENT_ON_RECEIVE, /** Increment counter value on receive */
        } type;

        /** Set messageType to this to match all messages */
        static const whiteboard::MessageType EVERY_MESSAGE_TYPE = whiteboard::WB_MSG_INVALID;

        /** Type of the message. */
        whiteboard::MessageType messageType;

        /** Counter value */
        size_t counter;

        /** Timestamp of last change */
        WbTimestamp timeStamp;
    };

    /** Constructor
    *
    * @param numberOfItems Number of items in the action list
    * @param pActions List of actions.
    */
    CommAdapterMockupMessageCounter(size_t numberOfItems, Action* pActions);

    /** Called when message is sent
    *
    * @param rCommAdapter Destination communication adapter
    * @param senderIndex Index of the sender adapter
    * @param pBuffer Communication buffer to send
    */
    void onSend(const CommAdapterMockup& rCommAdapter, const size_t senderIndex, const Buffer* pBuffer);

    /** Called when message is received
    *
    * @param rCommAdapter Destination communication adapter
    * @param senderIndex Index of the sender adapter
    * @param pBuffer Communication buffer to send
    */
    void onReceive(const CommAdapterMockup& rCommAdapter, const size_t senderIndex, const Buffer* pBuffer);

private:
    /** Number of items in the action list */
    size_t mNumberOfItems;

    /** List of actions. */
    Action* mpActions;
};

/** Class that implements error simulation for comm adapter mockups */
class CommAdapterMockupFailureSimulator
{
public:
    /** Failure information
    *
    * @note When failure simulator matches failure list, it will use FIRST failure that matches the filter.
    *       Packet index counter for all other matching failures will be incremented, but they are not used.
    */
    struct Failure
    {
        /** Type of the failure */
        enum
        {
            CONNECT_FAILURE, /** I/O level error when connecting */
            ASYNC_CONNECT_TIMEOUT, /** Asynchronous connect timeouts */
            SEND_FAILURE, /** I/O level error when sending */
            ASYNC_SEND_TIMEOUT, /** Asynchronous send timeouts, before actual packet is sent */
            LOST_MESSAGE, /** Packet is lost in transmission */
            CORRUPT_MAGIC, /** Packet's magic byte has been corrupted */
            CORRUPT_LENGTH, /** Packet's length information has been corrupted */
            NOT_SUPPORTED_MESSAGE_TYPE, /** Packet's message type is not supported */
        } type;

        /** Set messageType to this to match all messages */
        static const whiteboard::MessageType EVERY_MESSAGE_TYPE = whiteboard::WB_MSG_INVALID;

        /** Type of the message. */
        whiteboard::MessageType messageType;

        /** Set packetIndex to this to match all messages */
        static const int32 EVERY_PACKET_INDEX = -1;

        /** Packet index to match. Packet counter is incremented every time messageType has matched. */
        int32 packetIndex;
    };

    /** Constructor
    *
    * @param numberOfItems Number of items in the failure list
    * @param pFailures List of failures. Note that you need a separate failure list for each communication adapter
    */
    CommAdapterMockupFailureSimulator(size_t numberOfItems, Failure* pFailures);

    /** Gets instance for happy case handling */
    static CommAdapterMockupFailureSimulator& getHappyCaseDummy();

    /** Simulates send 
     *
     * @param rCommAdapter Destination communication adapter
     * @param senderIndex Index of the sender adapter
     * @param pBuffer Communication buffer to send
     */
    Result send(CommAdapterMockup& rCommAdapter, size_t senderIndex, Buffer* pBuffer);

private:
    /** Default constructor that is always happy */
    CommAdapterMockupFailureSimulator();

    /** Finds failure 
     *
     * @param pBuffer Buffer to send
     * @return Failure from the failure list or NULL if there should be no failures
     */
    Failure* findFailure(Buffer* pBuffer);

private:
    /** Number of items in the failure list */
    size_t mNumberOfItems;

    /** List of failures. */
    Failure* mpFailures;
};

// Custom communication adapter to do virtual messaging between whiteboards
class CommAdapterMockup : public whiteboard::CommAdapter
{
public:
    /** Default traits.
     * 5s connect timeout
     * 5s send timeout
     * no internal keep alives
     * no unidirectional connect
     * no internal encryption
     */
    static const Traits DEFAULT_TRAITS;

    CommAdapterMockup(
        whiteboard::WhiteboardCommunication& rCommunication,
        size_t index,
        CommAdapterMockupMessageCounter* pMessageCounter = NULL,
        CommAdapterMockupFailureSimulator& rFailureSimulator = CommAdapterMockupFailureSimulator::getHappyCaseDummy(),
        const Traits& rAdapterTraits = DEFAULT_TRAITS);
    virtual ~CommAdapterMockup();

    const char* getName() const OVERRIDE { return "Mockup"; }

    bool waitForConnection(RoutingTableEntryHandle& rRouteHandle);

    using CommAdapter::addRoute;

    RoutingTableEntryHandle addRoute(
        const char* address, const char* serialNumber, bool buddy = false);
    
    RoutingTableEntryHandle addRouteAndWait(
        const char* address, const char* serialNumber, bool buddy = false);

    RoutingTableEntryHandle addRouteAndWait(
        const char* serialNumber,
        const char* nextHopSerialNumber,
        HopCount numberOfHops,
        SequenceNumber sequenceNumber,
        WbTimestamp lifeTime);

    Result parseAddress(const char* address, Address& rAddress) OVERRIDE FINAL;

    Result formatAddress(const Address& rAddress, size_t bufferSize, char* addressBuffer) OVERRIDE FINAL;

    whiteboard::Result enableAdapter() OVERRIDE FINAL;

    whiteboard::Result disableAdapter() OVERRIDE FINAL;

    whiteboard::Result connect(const whiteboard::Address& /*rDestination*/) OVERRIDE;

    whiteboard::Result cancelConnect(const whiteboard::Address& /*rDestination*/, whiteboard::Result /*reason*/) OVERRIDE;

    whiteboard::Result disconnect(const whiteboard::Address& /*rDestination*/) OVERRIDE FINAL;

    whiteboard::IBufferPool& getBufferPool() OVERRIDE FINAL;

    /**
        For test to control the return result code of disableAdapter()
        @param result Result code to return from disableAdapter(), result == static_cast<Result>(0) indicates "not used"
    */
    void setControlledDisableResult(const Result result) { mControlledDisableResult = result; }

    static int findFreeIndex();

protected:
    static void indexToAddress(size_t index, whiteboard::Address& rAddressData);
    static size_t addressToIndex(const whiteboard::Address& rAddressData);

private:
    friend class CommAdapterMockupFailureSimulator;
    void receiveData(size_t sourceIndex, const whiteboard::Buffer* pBuffer);

public:
    virtual whiteboard::Result send(const whiteboard::Address& rDestination, whiteboard::Buffer* pBuffer) OVERRIDE;

    whiteboard::Result cancelSend(whiteboard::Buffer* /*pBuffer*/, whiteboard::Result /*reason*/) OVERRIDE;

protected:
    /** Whiteboard communication instance */
    whiteboard::WhiteboardCommunication& mrCommunication;

    /** Index of communication adapter */
    size_t mIndex;

    /** Message counter instance */
    CommAdapterMockupMessageCounter* mpMessageCounter;

    /** Failure simulator instance */
    CommAdapterMockupFailureSimulator& mrFailureSimulator;

    /** Disable result code controlled from the test itself */
    Result mControlledDisableResult;
};

// Mockup that does asynchronous connects and sends
class AsyncCommAdapterMockup : public CommAdapterMockup
{
public:
    static const size_t INVALID_ADDRESS = static_cast<size_t>(-1);

    AsyncCommAdapterMockup(
        whiteboard::WhiteboardCommunication& rCommunication,
        size_t index,
        uint32 connectDelay,
        uint32 sendDelay,
        CommAdapterMockupMessageCounter* pMessageCounter = NULL,
        CommAdapterMockupFailureSimulator& rFailureSimulator = CommAdapterMockupFailureSimulator::getHappyCaseDummy(),
        const Traits& rAdapterTraits = CommAdapterMockup::DEFAULT_TRAITS);
    virtual ~AsyncCommAdapterMockup();

    whiteboard::Result connect(const whiteboard::Address& rDestination) OVERRIDE;
    whiteboard::Result send(const whiteboard::Address& rDestination, whiteboard::Buffer* pBuffer) OVERRIDE;

    whiteboard::Result cancelConnect(const whiteboard::Address& /*rDestination*/, whiteboard::Result /*reason*/) OVERRIDE;
    whiteboard::Result cancelSend(whiteboard::Buffer* pBuffer, whiteboard::Result /*reason*/) OVERRIDE;

    void setSendTimeout(uint32 sendDelay);

private:
    static void onConnectCompletedInternal(AsyncCommAdapterMockup* pMockup);
    static void onSendCompletedInternal(AsyncCommAdapterMockup* pMockup);

    static void connectTimerCallback(WbTimerHandle handle);
    static void sendTimerCallback(WbTimerHandle handle);

private:
    /** Connect delay */
    uint32 mConnectDelay;

    /** Send delay */
    uint32 mSendDelay;

    /** Timer for simulating asynchronous connect operations */
    WbTimerHandle mConnectTimer;

    /** Timer for simulating asynchronous connect operations */
    WbTimerHandle mSendTimer;

    /** Semaphore that guards that there is only one asynchronous operation running simultaneously */
    WbSemaphoreHandle mSemaphore;

    /** Address of the destination */
    volatile size_t mDestinationAddress;

    /** Current send buffer */
    volatile whiteboard::Buffer* mpSendBuffer;
};

} // namespace whiteboard

#endif
