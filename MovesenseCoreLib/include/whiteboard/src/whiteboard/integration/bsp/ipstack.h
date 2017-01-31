#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

typedef uint32 WbIPv4Address;

struct WbIPv4SocketAddress
{
    WbIPv4Address host; /** 32-bit IPv4 host address */
    uint16 port;        /** 16-bit protocol port */
};

#define WbIPv4AddrAny 0x00000000
#define WbIPv4AddrLoopback 0x7f000001
#define WbIPv4AddrBroadcast 0xFFFFFFFF

#define WB_INVALID_SOCKET NULL

/** Initializes the IP stack
*
* @return A value indicating whether the stack was successfully initialized
*/
bool WbIpStackInitialize();

/** Terminates use of the IP stack */
void WbIpStackDeinitialize();

/** Checks whether IP stack has been initialized
*
* @return A value indicating whether the stack has been successfully initialized
*/
bool WbIsIpStackInitialized();

/** Resolves string form host address
*
* @param address String form address of a host
* @param rResolved On successful output contains resolved host address
* @return A value indicating whether the operation succeeded
*/
bool WbIPv4ResolveAddress(const char* address, WbIPv4Address& rResolved);

/** Formats socket address to human readable string form
*
* @param rAddress Address that should be formatted
* @param bufferSize Maximum number of charactes in the buffer including zero terminator
* @param addressBuffer Buffer where address should be written
* @return A value indicating whether the operation succeeded
*/
bool WbIPv4FormatSocketAddress(const WbIPv4SocketAddress& rAddress, size_t bufferSize, char* addressBuffer);

typedef uint16 WbTcpPort;

// Forward declare; hides implementation
struct WbTcpSocket;
typedef struct WbTcpSocket* WbIPv4TcpSocket;

/** Builds TCP socket address from given IP address and port
*
* @param rSocketAddress On output contains the socket address
* @param rAddress IP address to use
* @param port Port to use
*/
void WbIPv4TcpMakeSocketAddress(WbIPv4SocketAddress& rSocketAddress, const WbIPv4Address& rAddress, WbTcpPort port);

/** Creates new TCP server socket
*
* @param rAddress Interface address
* @param port Port that should be used
* @return New TCP server socket
*/
WbIPv4TcpSocket WbIPv4TcpCreateServerSocket(const WbIPv4Address& rAddress, WbTcpPort port);

/** Accepts client connection on TCP socket
*
* @param socket Socket to which client is connecting
* @param rSocketAddress On output contains the address of connecting client
* @return TCP socket that can be used to communicate with the client
*/
WbIPv4TcpSocket WbIPv4TcpAccept(WbIPv4TcpSocket socket, WbIPv4SocketAddress& rSocketAddress);

/** Connects to server socket
*
* @param rSocketAddress Address of server
* @return TCP socket that can be used to communicate with the server
*/
WbIPv4TcpSocket WbIPv4TcpConnect(const WbIPv4SocketAddress& rSocketAddress);

/** Closes given TCP socket
*
* @param socket Socket to close
*/
void WbIPv4TcpCloseSocket(WbIPv4TcpSocket socket);

/** Sends a message
*
* @param socket Socket from which the message should be sent
* @param pBuffer Buffer that contains the message
* @param lengthInBytes Length of the buffer in bytes
* @return A value indicating whether the operation succeeded
*/
bool WbIPv4TcpSend(WbIPv4TcpSocket socket, const void* pBuffer, uint32 lengthInBytes);

/** Receives a message
*
* @param socket Socket from which the message should be sent
* @param pBuffer Buffer that contains the message
* @param rLengthInBytes On input contains length of the buffer in bytes. On output contains length of the data in bytes
* @return A value indicating whether the operation succeeded
*/
bool WbIPv4TcpRecv(WbIPv4TcpSocket socket, void* pBuffer, uint32& rLengthInBytes);

typedef uint16 WbUdpPort;

// Forward declare; hides implementation
struct WbUdpSocket;
typedef struct WbUdpSocket* WbIPv4UdpSocket;

/** Builds UDP socket address from given IP address and port
*
* @param rSocketAddress On output contains the socket address
* @param rAddress IP address to use
* @param port Port to use
*/
void WbIPv4UdpMakeSocketAddress(WbIPv4SocketAddress& rSocketAddress, const WbIPv4Address& rAddress, WbUdpPort port);

/** Creates new UDP socket
*
* @param rAddress Interface address
* @param port Port that should be used
* @return New UDP socket
*/
WbIPv4UdpSocket WbIPv4UdpCreateSocket(const WbIPv4Address& rAddress, WbUdpPort port);

/** Closes given UDP socket
*
* @param socket Socket to close
*/
void WbIPv4UdpCloseSocket(WbIPv4UdpSocket socket);

/** Sends a message to given destination address
*
* @param socket Socket from which the message should be sent
* @param rDestination Destination address
* @param pBuffer Buffer that contains the message
* @param lengthInBytes Length of the buffer in bytes
* @return A value indicating whether the operation succeeded
*/
bool WbIPv4UdpSendTo(WbIPv4UdpSocket socket, const WbIPv4SocketAddress& rDestination, const void* pBuffer, uint32 lengthInBytes);

/** Receives a message and records sender address
*
* @param socket Socket from which the message should be sent
* @param rSource On output contains source address
* @param pBuffer Buffer that contains the message
* @param rLengthInBytes On input contains length of the buffer in bytes. On output contains length of the data in bytes
* @return A value indicating whether the operation succeeded
*/
bool WbIPv4UdpRecvFrom(WbIPv4UdpSocket socket, WbIPv4SocketAddress& rSource, void* pBuffer, uint32& rLengthInBytes);

// Forward declaration hides the implementation
struct WbSocketSetImpl;

/** Set of sockets */
class WbIpSocketSet
{
public:
    /** Default constructor */
    WbIpSocketSet();

    ~WbIpSocketSet();

    /** Waits for available data
    *
    * @param timeoutMs Timeout for the wait
    * @return Number of sockets that had data available
    */
    size_t select(uint32 timeoutMs);

#if defined(WB_HAVE_TCP_COMM)

    /** Adds a TCP socket to the read set
    *
    * @param socket Socket that should be added
    * @return A value indicating whether the operation succeeded
    */
    bool addIPv4TcpSocket(WbIPv4TcpSocket socket);

    /** Removes TCP socket from the read set
    *
    * @param socket Socket that should be removed
    * @return A value indicating whether the operation succeeded
    */
    bool removeIPv4TcpSocket(WbIPv4TcpSocket socket);

    /** Checks whether socket is in error state (state after last select call)
    *
    * @param socket Socket that should be checked
    * @return A value indicating whether the socket is in error state
    */
    bool hasErrorIPv4TcpSocket(WbIPv4TcpSocket socket) const;

    /** Checks whether socket has available data (state after last select call)
    *
    * @param socket Socket that should be checked
    * @return A value indicating whether the socket has data available
    */
    bool hasDataIPv4TcpSocket(WbIPv4TcpSocket socket) const;

#endif 

#if defined(WB_HAVE_UDP_COMM)
    /** Adds a UDP socket to the read set
    *
    * @param socket Socket that should be added
    * @return A value indicating whether the operation succeeded
    */
    bool addIPv4UdpSocket(WbIPv4UdpSocket socket);

    /** Removes UDP socket from the read set
    *
    * @param socket Socket that should be removed
    * @return A value indicating whether the operation succeeded
    */
    bool removeIPv4UdpSocket(WbIPv4UdpSocket socket);

    /** Checks whether socket in error state (state after last select call)
    *
    * @param socket Socket that should be checked
    * @return A value indicating whether the socket is in error state
    */
    bool hasErrorIPv4UdpSocket(WbIPv4UdpSocket socket) const;

    /** Checks whether socket has available data (state after last select call)
    *
    * @param socket Socket that should be checked
    * @return A value indicating whether the socket has data available
    */
    bool hasDataIPv4UdpSocket(WbIPv4UdpSocket socket) const;

#endif

private:

    WbSocketSetImpl* mpImpl;
};
