#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "plugin-api.hpp"

namespace wbcmd
{

/** Prototype for getting Whiteboard ID for given whiteboard address */
typedef wb::WhiteboardId (*GetWhiteboardIdProc)(const std::string& whiteboardAddress);

/** Prototype for construction Whiteboard ParameterListInstance from given request parameters. */
typedef std::unique_ptr<wb::ParameterListInstance<WB_MAX_NUMBER_OF_PARAMETERS>> (*MakeParameterListProc)(
    const wb::ResourceId resourceId,
    const wb::RequestType requestType,
    OperationParameters_t& rParameters,
    const wb::IDataTypeMetadataContainer* pMetadataContainer,
    std::vector<std::unique_ptr<ICleanable>> &rCleanupList);

/** Prototype for structure to JSON document conversion */
typedef wbjson::Json (*ConvertStructureToJsonProc)(const wb::IDataTypeMetadataContainer* pMetadataContainer, const wb::UnknownStructure& structure);

/** Prototype for value to JSON document conversion */
typedef wbjson::Json(*ConvertValueToJsonProc)(const wb::IDataTypeMetadataContainer* pMetadataContainer, const wb::Value& value);

/** Prototype for Pretty prints Whiteboard value to string
 */
typedef std::string (*PrettyPrintWhiteboardResultProc)(const wb::IDataTypeMetadataContainer* pMetadataContainer, const wb::Value& result);

/** Prototype for function that connects a whiteboard
*/
typedef bool (*ConnectProc)(const Connection* pConnection, bool loadMetadata);

/** Prototype for function that terminates whiteboard connection
*/
typedef void (*DisconnectProc)(const Connection* pConnection);

/** Prototype for a function that lists currently discovered devices
*/
typedef std::vector<const Connection*> (*GetDevicesProc)();

/** Prototype for registering new command handlers */
typedef void (*RegisterCommandHandlersProc)(const std::vector<CommandHandlerInfo>& commandHandlers);

/** Prototype for parsing command line */
typedef CommandLineArguments_t(*ParseCommandLineProc)(const std::string& commandLine);

/** Callbacks */
struct WbCmdPluginApiCallbacks
{
    CommandHandlerProc executeRequest;
    GetWhiteboardIdProc getWhiteboardId;
    MakeParameterListProc makeParameterList;
    ConvertStructureToJsonProc convertStructureToJson;
    ConvertValueToJsonProc convertValueToJson;
    PrettyPrintWhiteboardResultProc prettyPrintWhiteboardResult;
    ConnectProc connect;
    DisconnectProc disconnect;
    GetDevicesProc getDevices;
    RegisterCommandHandlersProc registerCommandHandlers;
    ParseCommandLineProc parseCommandLine;
};

/** Plugin entry point. Sets up the callbacks and calls plugin initialization function wbcmdPluginInitialize */
WBCMD_PLUGIN_API void wbcmdPluginEntry(const WbCmdPluginApiCallbacks& callbacks);

} // namespace wbcmd
