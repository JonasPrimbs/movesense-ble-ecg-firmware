#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/comm/CommAdapter.h"
#include "whiteboard/containers/TypedPool.h"

namespace whiteboard
{

/** Class that handles message receiving for all IP based protocols */
class IpReceiver : public IDynamicallyAllocatable
{
public:
    /** Constructor
    *
    * @param rConfiguration Configuration
    */
    IpReceiver(const Config& rConfiguration);

    /** Destructor */
    ~IpReceiver();

    /** Creates singleton instance of IpReceiver class
    *
    * @param rConfiguration Configuration
    * @return A value indicating wether the instance was successfully created
    */
    static bool createInstance(const Config& rConfiguration);

    /** Deletes singleton instance of IpReceiver class
    */
    static void deleteInstance();

    /** Gets existing IpReceiver instance
     *
     * @return Reference to IpReceiver singleton object
     */
    static IpReceiver& getInstance();

    /** Decrements reference count of singleton instance.
     * Instance is freed when no reference exists anymore
     */
    void releaseInstance();

    /** Interface for  class of IP protocol adapters for whiteboard */
    class IClient
    {
    public:
        /** Destructor */
        virtual ~IClient() { }

        /** Adds current adapter to given socket set
        *
        * @param rSocketSet Socket set that indicates available data
        * @return A value indicating whether adapter was successfully added
        */
        virtual bool addTo(WbIpSocketSet& rSocketSet) = 0;

        /** Removes current adapter from given socket set
        *
        * @param rSocketSet Socket set that indicates available data
        * @return A value indicating whether adapter was successfully removed
        */
        virtual bool removeFrom(WbIpSocketSet& rSocketSet) = 0;

        /** Checks whether current adapter is in error state
        *
        * @param rSocketSet Socket set that indicates error condition
        * @return A value indicating whether socket is in error state
        */
        virtual bool checkError(const WbIpSocketSet& rSocketSet) = 0;

        /** Notifies adapter of error state. */
        virtual void error() = 0;

        /** Checks whether current adapter has data in socket set
        *
        * @param rSocketSet Socket set that indicates available data
        * @return A value indicating whether socket of adapter had data
        */
        virtual bool checkData(const WbIpSocketSet& rSocketSet) = 0;

        /** Receives packet from the adapter. Called only when socket has pending data.
        * @return false if connection has been lost
        */
        virtual bool receive() = 0;
    };

    /** Adds a client to listener.
    *
    * @param rAdapter Adapter that should be added
    * @return A value indicating whether adapter was added
    */
    bool add(IClient& rClient);

    /** Removes a client from the listener
    *
    * @param rAdapter Adapter that should be removed
    * @return A value indicating whether adapter was removed
    */
    bool remove(IClient& rAdapter);

private:
    /** Thread loop */
    void threadFunc();

    /** Entry point of receiver thread
    *
    * @param pUserData Pointer to execution context instance
    */
    static void threadRunnerCB(void* pUserData);

    /** Adds client to IpReceivers internal structures
    *
    * @param pClient Client that should be added
    * @return A value indicating whether the client was added
    */
    bool addClientDirect(IClient* pClient);

    /** Removes client to IpReceivers internal structures
    *
    * @param pClient Client that should be removed
    * @return A value indicating whether the client was removed
    */
    bool removeClientDirect(IClient* pClient);

private:
    /** Mutual exclusion */
    WbMutexHandle mMutex;

    /** Event queue that dispatches commands to receiver thread that handles internal data structures */
    WbEventQueueHandle mCommandQueue;

    /** Semaphore that is used for waiting command completition */
    WbSemaphoreHandle mCommandSemaphore;

    enum
    {
        /** Event for command that adds new client to the receiver */
        EVENT_ID_ADD,

        /** Event for command that removes existing client from the receiver */
        EVENT_ID_REMOVE
    };

    /** Registry of clients */
    typedef TypedPool<Pool::KeyType, IClient*> ClientPool;
    ClientPool mClients;

    /** Set of listened sockets */
    WbIpSocketSet mSocketSet;

    /** Handle of the thread */
    WbThreadHandle mThreadHandle;
};

} // namespace whiteboard
