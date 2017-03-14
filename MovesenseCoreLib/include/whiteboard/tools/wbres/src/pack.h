#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "shared.h"
#include "document.h"

/**
* Packs document
*
* @param rDocument Document that should be packed
*/
void packDocument(Document& rDocument);

/**
* Packs input files
*
* @param rInputFiles List of input files
*/
void packInputFiles(std::vector<InputFile*>& rInputFiles);

/**
* Post processes packed input files
*
* @param rInputFiles List of input files
*/
void postProcessPackedInputFiles(std::vector<InputFile*>& rInputFiles);

