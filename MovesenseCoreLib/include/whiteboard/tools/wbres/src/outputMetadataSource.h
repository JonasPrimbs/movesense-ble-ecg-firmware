#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "shared.h"
#include "document.h"

/** Writes output C++ source file for metadata
 *
 * @param rInputFiles List of input files
 * @param metdataName Name of the variable that will store the metadata blob
 * @param rBlob Metadata blob
 * @param rOutput Output stream
 * @return A value indicating whether the operation succeeded
 */
bool outputMetadataSource(
    const std::vector<InputFile*>& rInputFiles, const std::string& metadataName, const std::vector<uint8_t>& rBlob, std::ostream& rOutput);
