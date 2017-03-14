#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "shared.h"
#include "document.h"

/**
* Reads and parses input files
*
* @param rPreprocessor Preprocessor to use
* @param rIncludeDirs Include directories for resolving file references
* @param rInputFiles List of input files
* @return A value indicating whether the input files were successfully parsed
*/
bool parseInputFiles(class IPreprocessor& rPreprocessor, const std::vector<std::string>& rIncludeDirs, std::vector<InputFile*>& rInputFiles);

/**
* Combines input files with same whiteboard ID
*
* @param rInputFiles List of input files
*/
void combineInputFiles(std::vector<InputFile*>& rInputFiles);