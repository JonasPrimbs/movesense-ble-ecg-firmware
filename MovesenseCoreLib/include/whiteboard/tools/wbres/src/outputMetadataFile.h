#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "shared.h"

/** Writes output binary file for metadata
 *
 * @param rBlob Metadata blob
 * @param rOutput Output stream
 * @return A value indicating whether the operation succeeded
 */
bool outputMetadataFile(const std::vector<uint8_t>& rBlob, std::ostream& rOutput);
