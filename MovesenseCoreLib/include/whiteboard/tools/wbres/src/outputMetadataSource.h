#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "shared.h"
#include "document.h"

/**
* Writes strings to the output
*
* @param rInputFiles List of input files
* @param rOutput Output stream
* @return A value indicating whether output was successfully written
*/
bool outputMetadataStrings(const std::vector<InputFile*>& rInputFiles, std::ostream& rOutput);


/** Writes output C++ source file for metadata
 *
 * @param metdataName Name of the variable that will store the metadata blob
 * @param rBlob Metadata blob
 * @param rOutput Output stream
 * @return A value indicating whether the operation succeeded
 */
bool outputMetadataSource(const std::string& metadataName, const std::vector<uint8_t>& rBlob, std::ostream& rOutput);
