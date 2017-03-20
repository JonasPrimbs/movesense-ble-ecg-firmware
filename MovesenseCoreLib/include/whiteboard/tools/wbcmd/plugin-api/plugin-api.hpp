#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <whiteboard/Identifiers.h>
#include <whiteboard/Result.h>
#include <whiteboard/Request.h>
#include <whiteboard/ParameterList.h>

#include "json11/json11.hpp"
#include <chrono>

#if defined(_MSC_VER)
  #if defined(WBCMD_PLUGIN_DECLARE_API_EXPORT)
    #define WBCMD_PLUGIN_API __declspec(dllexport)
    #define WBCMD_PLUGIN_INTERNAL
  #elif defined(WBCMD_PLUGIN_DECLARE_API_IMPORT)
    #define WBCMD_PLUGIN_API __declspec(dllimport)
    #define WBCMD_PLUGIN_INTERNAL
  #endif
#elif defined(__GNUC__) || defined(__clang__)
  #define WBCMD_PLUGIN_API __attribute__((visibility("default")))
  #define WBCMD_PLUGIN_INTERNAL __attribute__((visibility("hidden")))
#endif

#ifndef WBCMD_PLUGIN_API
  #define WBCMD_PLUGIN_API
  #define WBCMD_PLUGIN_INTERNAL
#endif

#define WBCMD_EXECUTION_CONTEXT 1

#include "cleanable.hpp"
#include "cmdline.hpp"

// Forward declarations
namespace whiteboard
{
class IDataTypeMetadataContainer;
class UnknownStructure;
class Value;
}

// Shorthand version for std::chrono::high_resolution_clock::now
inline std::chrono::high_resolution_clock::time_point NOW()
{
    return std::chrono::high_resolution_clock::now();
}

namespace wbcmd
{

/** Gets resource ID of the resource specified by requestInfo structure
 */
wb::ResourceId getResourceId(
    const std::string& whiteboardAddress,
    const std::string& resourcePath,
    const wb::IDataTypeMetadataContainer* pMetadataContainer);

/** Executes a single request using the given command line and execution context.
* @return Returns the result as JSON object. */
wbjson::Json executeRequest(
    const wbcmd::CmdLine& rCmdLine,
    const std::string& whiteboardAddress,
    const wb::IDataTypeMetadataContainer* pMetadataContainer);

/** Gets whiteboard ID of given remote whiteboard */
wb::WhiteboardId getWhiteboardId(const std::string& whiteboardAddress);

/** Get the ParameterListInstance for given command line parameters to be used in direct Whiteboard operations. */
std::unique_ptr<wb::ParameterListInstance<WB_MAX_NUMBER_OF_PARAMETERS>> makeParameterList(
    const wb::ResourceId resourceId,
    const wb::RequestType requestType,
    OperationParameters_t& rParameters,
    const wb::IDataTypeMetadataContainer* pMetadataContainer,
    std::vector<std::unique_ptr<ICleanable>> &rCleanupList);

/** Converts structure to JSON document
 *
 * @param pMetadataContainer MetadataContainer instance that contains metadata for the structure
 * @param structure Stucture instance that should be converted to JSON
 * @return Converted JSON
 */
wbjson::Json convertStructureToJson(const wb::IDataTypeMetadataContainer* pMetadataContainer, const wb::UnknownStructure& structure);

/** Converts value to JSON document
*
* @param pMetadataContainer MetadataContainer instance that contains metadata for the datatypes
* @param value Value instance that should be converted to JSON
* @return Converted JSON
*/
wbjson::Json convertValueToJson(const wb::IDataTypeMetadataContainer* pMetadataContainer, const wb::Value& value);

/** Pretty prints Whiteboard value to string
 *
 * @param pMetadataContainer MetadataContainer instance that contains metadata for the structure
 * @param result Value to print
 * @return Formatted Whiteboard value
 */
std::string prettyPrintWhiteboardResult(const wb::IDataTypeMetadataContainer* pMetadataContainer, const wb::Value& result);

/** Information about a device */
struct Connection
{
    std::string whiteboardAddress;
    wb::WhiteboardId whiteboardId;
    std::string description;
    std::string deviceAddress;
    const wb::IDataTypeMetadataContainer* pMetadataContainer;

    /** Constructor */
    Connection(
        const std::string& whiteboardAddress,
        wb::WhiteboardId whiteboardId,
        const std::string& description,
        const std::string& deviceAddress,
        const wb::IDataTypeMetadataContainer* pMetadataContainer);

    /** Prevent use of default constructor */
    Connection() = delete;

    /** Prevent use of copy constructor */
    Connection(const Connection&) = delete;

    /** Prevent use of assignment operator */
    const Connection& operator=(const Connection&) = delete;

    /** Establishes connection to this device.
     *
     * @note Call blocks until connection is made or connection attemp timeouts
     * @param loadMetadata A value indicating whether metadata of the whiteboard should be loaded
     * @return True if connection was established successfully (or device was already connected)
     */
    bool connect(bool loadMetadata = true) const;

    /** Terminates connection to this device.
     */
    void disconnect() const;
};

/** Gets list of currently discovered devices
*
* @return List of currently discovered devices
*/
std::vector<const Connection*> getDevices();

/** Prototype for command handlers */
typedef wbjson::Json(*CommandHandlerProc)(
    const wbcmd::CmdLine& rCmdLine,
    const std::string& whiteboardAddress,
    const wb::IDataTypeMetadataContainer* pMetadataContainer);

const wbjson::Json makeResult(
    const std::string& operation,
    wb::Result resultCode,
    const std::string& message = "",
    const std::string& uri = "",
    const wbjson::Json& content = wbjson::Json());

/** Command line option information */
struct CommandLineOption
{
    const char* name;
    const char* commandLineHelp;
    const char* defaultValue;
};

/** Information about a command handler */
struct CommandHandlerInfo
{
    const char* command;
    bool requiresMetadata;
    bool singleConnection;
    const char* commandLineHelpSection;
    const char* commandLineHelp;
    const std::vector<CommandLineOption> commandLineOptions;
    CommandHandlerProc commandHandler;
};

/** Registers new command handlers */
void registerCommandHandlers(const std::vector<CommandHandlerInfo>& commandHandlers);

} // namespace wbcmd
