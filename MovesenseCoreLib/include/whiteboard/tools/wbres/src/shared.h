#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2014.
    All rights reserved.
******************************************************************************/

#include <stdio.h>
#include <memory.h>
#include <assert.h>

#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#ifdef WIN32
#  include <io.h>
#elif defined(__APPLE__)
#  include <sys/uio.h>
#else
#  include <sys/io.h>
#endif

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <sstream>
#include <limits.h>
#include <stdint.h>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
#include "quantity/quantity/quantity.h"

#ifdef _MSC_VER
#define sscanf sscanf_s
#endif

#define __WBRES__ 2.0

#define ELEMENTS(arr)		(sizeof(arr) / sizeof((arr)[0]))

/** Macro for converting macro parameter to string */
#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)


enum ErrorLevel
{
    EL_INFO,
    EL_WARNING,
    EL_ERROR
};
extern ErrorLevel gPendanticWarnings;
#define EL_PENDATIC gPendanticWarnings

class FileException : public std::runtime_error
{
public:
    FileException() = delete;

    FileException(const FileException& rOther)
        : std::runtime_error(rOther.what()), errorLevel(rOther.errorLevel), filename(rOther.filename), pInnerException(NULL)
    {
        if (rOther.pInnerException != NULL)
        {
            pInnerException = new FileException(*rOther.pInnerException);
        }
    }

    FileException(const std::string& filename_, const std::string& what, ErrorLevel errorLevel_ = EL_ERROR)
        : std::runtime_error(build_what(errorLevel_, filename_, what)), errorLevel(errorLevel_), filename(filename_), pInnerException(NULL)
    {
    }

    FileException(const std::string& filename_, const std::runtime_error& e, ErrorLevel errorLevel_ = EL_ERROR)
        : std::runtime_error(build_what(errorLevel_, filename_, e)), errorLevel(errorLevel_), filename(filename_), pInnerException(NULL)
    {
        try
        {
            auto fpe = dynamic_cast<const FileException&>(e);
            pInnerException = new FileException(fpe);
        }
        catch (std::bad_cast)
        {
        }
    }

    ~FileException()
    {
        delete pInnerException;
    }

    ErrorLevel errorLevel;
    std::string filename;
    const FileException* pInnerException;

private:
    static const std::string build_what(ErrorLevel errorLevel, const std::string& filename, const std::string& what)
    {
        std::stringstream output;

        output << filename << "(1): ";

        switch (errorLevel)
        {
        case EL_ERROR: output << "error"; break;
        case EL_WARNING: output << "warning"; break;
        case EL_INFO: output << "info"; break;
        default: assert(0);
        }

        output << " WBRES " << what;
        return output.str();
    }

    static const std::string build_what(ErrorLevel errorLevel, const std::string& filename, const std::runtime_error& e)
    {
        std::stringstream output;

        try
        {
            auto fpe = dynamic_cast<const FileException&>(e);
            output << fpe.what() << std::endl << "... while parsing path '" << filename << "'.";
        }
        catch (std::bad_cast)
        {
            // Other errors
            std::string msg = e.what();

            // Try to parse yaml-cpp style error message
            unsigned int line, column;
            if (sscanf(msg.c_str(), "Error at line %u, column %u: ", &line, &column) != 2)
            {
                output << filename << "(1): ";
                
            }
            else
            {
                output << filename << "(" << line << "," << column << "): ";
                msg = strchr(msg.c_str(), ':');
            }

            switch (errorLevel)
            {
            case EL_ERROR: output << "error"; break;
            case EL_WARNING: output << "warning"; break;
            case EL_INFO: output << "info"; break;
            default: assert(0);
            }

            output << " WBRES " << msg;
        }

        return output.str();
    }
};

class InputException : public std::runtime_error
{
public:
    InputException() = delete;
    InputException(const InputException&) = default;

    InputException(const std::string& filename_, const size_t line_, const size_t column_, const std::string& msg_, ErrorLevel errorLevel_ = EL_ERROR)
        : std::runtime_error(build_what(line_, column_, msg_)), errorLevel(errorLevel_), filename(filename_), line(line_), column(column_) {}
    virtual ~InputException() throw() {}

    ErrorLevel errorLevel;
    std::string filename;
    size_t line;
    size_t column;
    std::string msg;

private:
    static const std::string build_what(const size_t line, const size_t column, const std::string& msg)
    {
        std::stringstream output;
        output << "Error at line " << line << ", column " << column << ": " << msg;
        return output.str();
    }
};

inline bool startsWithIgnoreCase(const std::string& str, const std::string& start)
{
    if (str.length() < start.length())
        return false;

    for (size_t i = 0; i < start.length(); ++i)
    {
        if (tolower(str[i]) != tolower(start[i]))
            return false;
    }

    return true;
}

inline bool endsWithIgnoreCase(const std::string& str, const std::string& end)
{
    if (str.length() < end.length())
        return false;

    size_t offset = str.length() - end.length();
    for (size_t i = 0; i < end.length(); ++i)
    {
        if (tolower(str[i + offset]) != tolower(end[i]))
            return false;
    }

    return true;
}

template <typename T, class Comparator = std::equal_to<T> >
bool addToListIfUnique(std::vector<T>& rUniqueList, T data)
{
    Comparator comparator;
    auto predicate = [&](const T& rValue)
    {
        return comparator.operator()(rValue, data);
    };

    if (std::find_if(rUniqueList.begin(), rUniqueList.end(), predicate) != rUniqueList.end())
    {
        return false;
    }

    rUniqueList.push_back(data);
    return true;
}
