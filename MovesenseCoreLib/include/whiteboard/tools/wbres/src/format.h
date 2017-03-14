#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2014.
All rights reserved.

******************************************************************************/
/**
@file format.h
Processes whiteboard resource descriptons and generates C++ code file for it.
*/

#include "shared.h"

class Path;
class DataType;

/** Helper function for converting string to lower case
*
* @param str String to convert
* @return Lower case string
*/
std::string tolower(const std::string& str);

/**
* Pads string from right to specified length
*
* @param s Strign to pad
* @param n Required length
* @param c Character to use in padding
* @return Padded string
*/
std::string padRight(const std::string& s, std::string::size_type n, char c = ' ');

/** Case of formatted name */
typedef enum
{
    NO_CHANGE,
    LOWER_CASE,
    UPPER_CASE,
    PASCAL_CASE,
    CAMEL_CASE
} Case;

/**
* Formats identifier name.
*
* @param rName name to format
* @param casing A value that indicates how name should be formatted
* @return Formatted name
*/
std::string formatName(const std::string& rName, Case casing);

/**
* Formats name of the path.
*
* @param rPath Path to format
* @param casing A value that indicates how name should be formatted
* @return Formatted name
*/
std::string formatName(const Path& rPath, Case casing);

/**
* Outputs data type to the given stream in pretty format
*
* @param rStream Output stream
* @param pDataType Data type to output
*/
void prettyPrintDataType(std::ostream& rStream, const DataType* pDataType);

/** Splits string with given delimeter
*
* @param s String to split
* @param delim String delimeter
* @param parts Vector where the string parts should be added
* @return Reference to parts vector
*/
std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string>& parts);

/** Splits string with given delimeter
 *
 * @param s String to split
 * @param delim String delimeter
 * @return splitted string parts
 */
std::vector<std::string> split(const std::string &s, char delim);
