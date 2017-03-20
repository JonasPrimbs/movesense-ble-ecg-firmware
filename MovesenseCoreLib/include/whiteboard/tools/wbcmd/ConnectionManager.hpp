#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <whiteboard/devicediscovery/DeviceDiscoverer.h>
#include <string>
#include <vector>
#include "plugin-api/plugin-api.hpp"

namespace wbcmd
{

/** Manager for whiteboard connections */
class ConnectionManager final
{
public:
    /** Constructor */
    ConnectionManager();

    /** Destructor */
    ~ConnectionManager();

public:
    /** Adds a new connection from given address string
     *
     * @param address Address to use for new connection
     */
    void addConnection(const std::string& address);

    /** Enables device discovery */
    void enableDiscovery(unsigned short vid, unsigned short pid);

    /** Establishes connection to a whiteboard.
    *
    * @note Call blocks until connection is made or connection attemp timeouts
    * @param whiteboardAddress Address of the whiteboard to connect
    * @return True if connection was established successfully (or device was already connected)
    */
    bool connect(const Connection* pConnection, bool loadMetadata);

    /** Terminates connection to a whiteboard.
    *
    * @param whiteboardAddress Address of the whiteboard to connect
    */
    void disconnect(const Connection* pConnection);

    /** Gets list of currently discovered devices
    *
    * @return List of currently discovered devices
    */
    const std::vector<const Connection*> getConnections() const;

private:
    /** Helper function for constructing connection information structure and 
    * adding it to list of connections
    *
    * @param connection Connection that should be added
    */
    void addConnection(const whiteboard::device_discovery::Connection& connection);

    /** Updates list of connected devices
     *
     * @param connectionList New list of connections
     */
    void updateConnections(const whiteboard::device_discovery::ConnectedDeviceList_t& connectionList);

private:
    /** List of current connections */
    typedef std::pair<whiteboard::device_discovery::Connection, Connection*> ConnectionInfo;
    std::vector<ConnectionInfo> mConnections;

    /** DeviceDiscoverer instance */
    std::unique_ptr<whiteboard::device_discovery::DeviceDiscoverer> mDiscoverer;
};

}
