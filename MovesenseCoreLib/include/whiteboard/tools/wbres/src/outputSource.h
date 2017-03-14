#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "shared.h"
#include "document.h"

/** Generates source file from given interface specification documentation
 *
 * @param rInputFiles List of input files
 * @param rOutput Output stream
 * @return A value indicating whether the operation succeeded
 */
bool outputSource(const std::vector<InputFile*>& rInputFiles, std::ostream& rOutput);
