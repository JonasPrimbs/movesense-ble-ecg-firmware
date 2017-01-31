#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2014.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/Result.h"

namespace whiteboard
{

// Forward declarations
class IBufferAllocator;
struct Buffer;

/** Interface for bus based communication links */
class IBusCommunication
{
public:
    /** Communication link states */
    enum BusCommunicationLinkState
    {
        BusCommunicationLinkStateNone,
        BusCommunicationLinkStateEnabled,
        BusCommunicationLinkStateDisabled,
        BusCommunicationLinkStateBroken,
    };

    /** Callbacks for data transfer operations */
    class IBusCallback
    {
    public:
        /** Callback when data has been received by the communication link
         *
         * @note Client is responsible for freeing the buffer by calling freeBuffer method
         *
         * @param pBuffer Buffer that was received
         */
        virtual void dataReceived(Buffer* pBuffer) = 0;

        /** Callback that is called when data buffer has actually been sent
         *
         * @param pBuffer Buffer that was sent
         * @param resultCode Result of send operation
         */
        virtual void dataSent(Buffer* pBuffer, Result resultCode) = 0;

        /** Callback for changes in bus's link state
        *
        * @param aState Bus's Link state changed either by explicitly invoked enable() /disable()
        * Or via internal bus specific discovery mechanism.
        */
        virtual void linkStateChanged(enum IBusCommunication::BusCommunicationLinkState aState) = 0;

        /** Destructor */
        virtual ~IBusCallback() {}
    };

    /** Enables the communication link
    *
    * @return Result of the operation
    */
    virtual Result enable() = 0;

    /** Disables the communication link
    *
    * @return Result of the operation
    */
    virtual Result disable() = 0;

    /** Gets buffer allocator that should be used to allocate and deallocate
    * adapter buffers
    *
    * @return Buffer allocator instance
    */
    virtual IBufferAllocator& getAllocator() const = 0;

    /** Starts asynchronous send operation
     *
     * @note Driver will free the buffer when data has been sent
     * @param pBuffer Buffer to send
     * @return Result of the operation
     */
    virtual Result sendData(Buffer* pBuffer) = 0;

    /** Cancels an on going send request
    *
    * @param pBufferP Buffer that contains the message that should be canceled
    * @param reason Reason for the cancelation
    * @return Result of the operation
    */
    virtual Result cancelSend(Buffer* pBuffer, Result reason) = 0;

    /** Destructor */
    virtual ~IBusCommunication() {}
};

} // namespace whiteboard
