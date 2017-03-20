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
* @param exportSymbol Symbol to use when exporting symbols
* @param dependencies List of additional include depenencied
* @param rOutput Output stream
* @return A value indicating whether the operation succeeded
*/
bool outputHeader(
    const std::vector<InputFile*>& rInputFiles, 
    const std::string& exportSymbol,
    const std::vector<std::string>& dependencies,
    std::ostream& rOutput);

/** Generates C source file from given interface specification documentation
*
* @param rInputFiles List of input files
* @param rOutput Output stream
* @return A value indicating whether the operation succeeded
*/
bool outputCHeader(const std::vector<InputFile*>& rInputFiles, std::ostream& rOutput);

/** Outputs include guard (pragma once)
*
* @param rOutput Output stream
* @return A value indicating whether the operation succeeded
*/
bool outputHeaderPragmaOnce(std::ostream& rOutput);

/**
* Writes header namespace begin to the output
*
* @param rOutput Output stream
* @return A value indicating whether output was successfully written
*/
bool outputHeaderNamespaceBegin(std::ostream& rOutput);

/**
* Writes header namespace end to the output
*
* @param rOutput Output stream
* @return A value indicating whether output was successfully written
*/
bool outputHeaderNamespaceEnd(std::ostream& rOutput);
