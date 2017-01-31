#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2016.
All rights reserved.
******************************************************************************/

#include <gtest/gtest.h>

#include "whiteboard/builtinTypes/Structures.h"
#include "whiteboard/builtinTypes/AlienStructure.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"

const static size_t pointerSize32Bit = 4;

#define ALIGN(length, alignment) (((length + (alignment - 1)) & ~(alignment - 1)) - length)
