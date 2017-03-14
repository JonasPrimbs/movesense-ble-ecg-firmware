#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

namespace wbcmd
{

struct CommandLineArgument
{
    std::string name;
    std::string value;
};

typedef std::vector<CommandLineArgument> CommandLineArguments_t;

struct OperationParameter
{
    std::string dataType;
    std::string srcData;
};

typedef std::vector<OperationParameter> OperationParameters_t;

#define REQUEST_NO_TIMEOUT -1
#define REQUEST_DEFAULT_TIMEOUT 15000

class CmdLine final
{
public:
    /** Constructor. Parses given command line.
    */
    CmdLine(const char* commandLine);

    /** Constructor. Parses given command line.
     */
    CmdLine(const std::string& commandLine);

    /** Command line arguments */
    const CommandLineArguments_t commandlineArguments;

    /** Constructor. Uses given command line arguments.
    */
    CmdLine(const CommandLineArguments_t commandlineArguments);

    /** Checks whether argument exists */
    bool hasArgument(const std::string& name) const
    {
        auto it = std::find_if(commandlineArguments.cbegin(), commandlineArguments.cend(), [&name](const CommandLineArgument& arg)
        {
            return arg.name == name;
        });
        return it != commandlineArguments.end();
    }

    /** Gets argument value with given name */
    std::string getArgument(const std::string& name) const
    {
        auto it = std::find_if(commandlineArguments.cbegin(), commandlineArguments.cend(), [&name](const CommandLineArgument& arg)
        {
            return arg.name == name;
        });
        if (it == commandlineArguments.end())
        {
            return "";
        }

        return it->value;
    }

    /** Gets name of operation given on command line */
    inline std::string getOperation() const
    {
        return getArgument("op");
    }

    /** Gets resource path given on command line */
    inline std::string getResourcePath() const
    {
        return getArgument("path");
    }

    /// Get the full resource path instead of the short path.
    /// For example: "/net/ECKIE2A81546/Device/UserInteraction/Button/Event"
    std::string getFullResourcePath(const std::string& whiteboardAddress) const
    {
        return getFullResourcePath(whiteboardAddress, getResourcePath());
    }

    /// Get the full resource path instead of the short path.
    /// For example: "/net/ECKIE2A81546/Device/UserInteraction/Button/Event"
    static std::string getFullResourcePath(const std::string& whiteboardAddress, const std::string& resourcePath)
    {
        static const std::string NET_PREFIX{ "/net/" };

        assert(resourcePath.find(NET_PREFIX) == std::string::npos);
        const std::string extraSlash = resourcePath[0] == '/' ? "" : "/";

        return NET_PREFIX + whiteboardAddress + extraSlash + resourcePath;
    }

    /** Gets timeout given on command line */
    inline int getTimeout() const
    {
        const auto timeoutArg = getArgument("timeout");
        if (timeoutArg.empty())
        {
            return REQUEST_DEFAULT_TIMEOUT;
        }

        return std::stoi(timeoutArg);
    }

    /** Gets operation parameters given on command line */
    OperationParameters_t getParameters() const;

    /** Converst command line back to string */
    const std::string toString() const
    { 
        std::string result;
        for (const auto& argument : commandlineArguments)
        {
            if (&argument != &commandlineArguments[0])
            {
                result += " ";
            }

            result += "--" + argument.name;
            if (!argument.value.empty())
            {
                result += " \"" + argument.value + "\"";
            }
        }

        return result;
    }
};

}
