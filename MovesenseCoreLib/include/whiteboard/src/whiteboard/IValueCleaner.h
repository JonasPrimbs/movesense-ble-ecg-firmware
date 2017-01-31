#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/builtinTypes/WrapperFor32BitPointer.h"

namespace whiteboard
{

#ifndef WB_NEED_POINTER_WRAPPER_POOL
#define WB_WHEN_STRUCTURE_CLEANING_NEEDED(stmt)
#else
#define WB_NEED_STRUCTURE_CLEANING
#define WB_WHEN_STRUCTURE_CLEANING_NEEDED(stmt)   stmt

/** Interface for value class cleaning (freeing temporary resources on scope exit) */
class IValueCleaner
{
public:
    /** Cleans the value data after it has been used
    *
    * @param pData Pointer to data that should be cleaned
    */
    virtual void clean(void* pData) const = 0;

    /** Destructor */
    virtual ~IValueCleaner() {}
};
#endif

} // namespace whiteboard
