#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/comm/internal/ip/TcpIpAdapter.h"
#include "whiteboard/comm/ICommServer.h"

namespace whiteboard
{

/** Class that implements TCP / IP protocol server for listening incoming whiteboard connections */
class TcpIpServer FINAL : public ICommServer, public IDynamicallyAllocatable, private IpReceiver::IClient
{
public:
    /** Constructor. Do not call this directly.
    *
    * @param socket Socket that should be used
    */
    TcpIpServer(const WbIPv4TcpSocket socket);

    /** Destructor */
    virtual ~TcpIpServer();

    /** Creates a new TCP server and binds it to specific interface address
     *
     * @param rInterfaceAddress Interface address to use
     * @param port TCP port to use
     * @return New TcpIpServer instance or NULL if adapter could not be created
     */
    static TcpIpServer* create(const WbIPv4Address& rInterfaceAddress, WbTcpPort port);

    /**
    *   Gets name of communication adapter
    *
    *   @return Name of the adapter
    */
    const char* getName() const OVERRIDE { return "TcpIpSrv"; }

    /** Enables the server instance
    *
    * @return Result of the operation
    */
    Result enable() OVERRIDE;

    /** Disables the server instance
    *
    * @return Result of the operation
    */
    Result disable() OVERRIDE;

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
    /** Adaptation of TcpIpAdapter class for connected clients that remove route when
    * it is disconnected
    */
    class ConnectedClient FINAL : public TcpIpAdapter
    {
    public:
        /** Constructor
         *
         * @param mutexHandle Handle of mutex to use for synchronization
         */
        ConnectedClient(WbMutexHandle mutexHandle);

        /** Destructor */
        virtual ~ConnectedClient();

        /** Constructs TcpIpAdapter instance for connecting client socket
        *
        * @param socket Socket that should be used
        * @param rAddress Address of connecting client
        * @return ConnectedClient instance
        */
        static ConnectedClient* create(WbIPv4TcpSocket socket, const Address& rAddress);

        /**
        *   Gets name of communication adapter
        *
        *   @return Name of the adapter
        */
        const char* getName() const OVERRIDE { return "TcpIpRmCl"; }

        /** Disconnects the adapter from destination
        *
        * @return Result of the operation
        */
        Result disconnect(const Address& rDestination) OVERRIDE;

    protected:
        /** Notifies adapter of error state. */
        void error() OVERRIDE;
    };

private:
    /** Socket that is associated with this server */
    WbIPv4TcpSocket mServerSocket;
};

} // namespace whiteboard
