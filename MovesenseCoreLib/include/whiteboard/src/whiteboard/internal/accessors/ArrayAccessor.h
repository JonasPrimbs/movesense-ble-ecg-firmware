#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

#include "whiteboard/builtinTypes/Array.h"

namespace whiteboard
{

/** Static interface class to provide a more priviledged access to Array class */
class ArrayAccessor
{
public:
    /** Gets reference to array's mNumberOfItems member
     *
     * @param rArray The array to access
     * @return Reference to nmber of items
     */
    template <typename ItemType>
    inline static uint16& refNumberOfItems(Array<ItemType>& rArray)
    {
        return rArray.mNumberOfItems;
    }

    /** Gets reference to array's mItems member
    *
    * @param rArray The array to access
    * @return Reference to items
    */
    template <typename ItemType>
    inline static WrapperFor32BitPointer<ItemType>& refItems(Array<ItemType>& rArray)
    {
        return rArray.mItems;
    }
};

} // namespace whiteboard
