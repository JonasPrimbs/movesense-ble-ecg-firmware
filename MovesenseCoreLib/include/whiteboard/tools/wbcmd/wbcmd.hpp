#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/
#include <string> 
#include <vector>
#include "plugin-api/plugin-api.hpp"

namespace wbcmd
{

/** Get the wbcmd's own device id. */
std::string getWbCmdDeviceId();

/** Get path to the application data directory.
 * Example: C:\Users\<user>\AppData\Roaming\Suunto */
std::string getApplicationDataDirectory();

/** Handlers for wbcmd's scripting features.
 * If any of these is recognized, the required logic is called and
 * the program is exited. */
class CmdLine;
void handleQuickOptions(const CmdLine& rCmdLine, const std::vector<const Connection*>& connections);

/** Prints out command help */
int help(const std::vector<const Connection*>& connections);

/** Array of registered command handlers */
struct CommandHandlerInfo;
extern std::vector<CommandHandlerInfo> sCommandHandlers;

/** Loads metadata for given Whiteboard */
class MetadataContainer;
void loadMetadataFromDeviceOrFile(
    const std::string& whiteboardAddress,
    MetadataContainer& metadataContainer);

} // namespace wbcmd
