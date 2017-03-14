#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/
/**
@file preprocessor.h
Performs preprocessing stage for YAML files
*/

#include "shared.h"

/** Interface for preprocessor */
class IPreprocessor
{
public:
    /** Destructor */
    virtual ~IPreprocessor()
    {
    }

    /** Adds include path
     *
     * @param rPath Path that should be added
     */
    virtual void addIncludePath(const std::string& rPath) = 0;

    /** Adds macro definition
    *
    * @param rDefinition Definition that should be added
    * @param predefine A value indicating whether this define can be removed with '#undef'
    */
    virtual void addDefinition(const std::string& rDefinition, bool predefine) = 0;

    /** Preprocesses given file
    *
    * @param inputFile Name of the file to preprocess
    * @return Preprocessed output
    */
    virtual std::istream* preprocess(const std::string& inputFile) = 0;

    /** Preprocesses given file
     *
     * @param inputFile Name of the file to preprocess
     * @param rOutput String that will hold the preprocessed output
     */
    virtual void preprocessToString(const std::string& inputFile, std::string& rOutput) = 0;

    /** Preprocesses given file
    *
    * @param inputFile Name of the file to preprocess
    * @param outputFile Name of the file where output should be written
    */
    virtual void preprocessToFile(const std::string& inputFile, const std::string& outputFile) = 0;
};

/** Constructs a new preprocessor instance 
*
* @return Pointer to a new instance
*/
IPreprocessor* createPreprocessor();
