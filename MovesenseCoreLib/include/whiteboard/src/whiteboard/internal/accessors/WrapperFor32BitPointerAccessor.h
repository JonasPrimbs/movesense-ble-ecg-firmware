#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

#include "whiteboard/builtinTypes/WrapperFor32BitPointer.h"

namespace whiteboard
{

/** Static interface class to provide a more priviledged access to WrapperFor32BitPointerAccessor class */
class WrapperFor32BitPointerAccessor
{
public:
    /** Assigns a new value to the pointer
    *
    * @tparam ItemType Type of the pointer
    * @param rPointer WrapperFor32BitPointer instance which value should be assigned
    * @param pointer New value for the pointer
    * @return Reference to wrapper object
    */
    template <typename T>
    inline static WrapperFor32BitPointer<T>& assign(WrapperFor32BitPointer<T>& rPointer, const void* pointer)
    {
        return rPointer.assign(pointer);
    }
};

} // namespace whiteboard
