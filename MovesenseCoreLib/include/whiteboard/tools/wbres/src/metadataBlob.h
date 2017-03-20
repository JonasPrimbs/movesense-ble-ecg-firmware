#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2017.
All rights reserved.
******************************************************************************/

#include "shared.h"
#include "document.h"

/** Generates metadata blob for different output options
 *
 * @param rInputFiles List of input files
 * @return Byte array
 */
std::vector<uint8_t> createMetadataBlob(const std::vector<InputFile*>& rInputFiles);
