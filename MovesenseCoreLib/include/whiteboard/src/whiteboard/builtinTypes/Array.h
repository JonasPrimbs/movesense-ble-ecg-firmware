#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/Structures.h"
#include "whiteboard/builtinTypes/WrapperFor32BitPointer.h"

namespace whiteboard
{

// Forward declarations
class ArrayAccessor;

/** Structure that handles optionality of an structure member
*
* @tparam ItemType Type of the array item
*/
template <typename ItemType> struct Array FINAL
{
private:
    /** Library internal implementation can access these members */
    friend class ArrayAccessor;

    /** Number of items */
    WB_ALIGNED(uint16) mNumberOfItems;

    /** Pointer to items */
    WB_ALIGNED(WrapperFor32BitPointer<ItemType>) mItems;

public:
    /** Default constructor */
    inline Array() : mNumberOfItems(0), mItems(NULL) {}

    /** Constructor that initializes Array object from an C array
    *
    * @tparam ItemType Type of the array item
    * @tparam NUMBER_OF_ITEMS Size of the array
    * @param rItemArray C array of items
    */
    template <size_t NUMBER_OF_ITEMS>
    inline Array(const ItemType(&rItemArray)[NUMBER_OF_ITEMS])
        : mNumberOfItems(static_cast<uint16>(NUMBER_OF_ITEMS)), mItems(&rItemArray[0])
    {
    }

    /** Constructor
    *
    * @param pItemArray C array of items
    * @param numberOfItems Number of items in the array
    */
    inline Array(const ItemType* pItemArray, size_t numberOfItems)
        : mNumberOfItems(static_cast<uint16>(numberOfItems)), mItems(pItemArray)
    {
    }

    /** Destructor */
    inline ~Array() {}

    /** Assignment operator that takes C array as its parameter */
    template <size_t NUMBER_OF_ITEMS> inline Array& operator=(const ItemType(&rItemArray)[NUMBER_OF_ITEMS])
    {
        mNumberOfItems = NUMBER_OF_ITEMS;
        mItems = &rItemArray[0];
        return *this;
    }

    /** Gets begin iterator of the array
     *
     * @return Iterator that points to start of the array
     */
    inline const ItemType* begin() const { return mItems.operator const ItemType*(); }

    /** Gets end iterator of the array
    *
    * @return Iterator that points to one past end of the array
    */
    inline const ItemType* end() const { return mItems.operator const ItemType*() + mNumberOfItems; }

    /* Gets number of items in the array. */
    inline size_t getNumberOfItems() const { return static_cast<size_t>(mNumberOfItems); }

    /* Gets number of items in the array. */
    inline size_t size() const { return static_cast<size_t>(mNumberOfItems); }

    /* Gets pointer to item array */
    inline const WrapperFor32BitPointer<ItemType>& getItems() const { return mItems; }

    /** Gets item with given index from the array */
    inline const ItemType& operator[](size_t i) const
    {
        WB_ASSERT(i < mNumberOfItems);
        const ItemType* pItemArray = mItems.operator const ItemType*();
        return pItemArray[i];
    }
};

WB_STATIC_VERIFY(sizeof(Array<char>) == 8, SizeOf_Array_Is_Not_Eight_Bytes);
WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(Array<char>) == 4, Array_Is_Not_Aligned_On_Double_Word_Boundary);

/** Helper for constructing arrays without specifying template parameters
 *
 * @tparam ItemType Type of the array item
 * @tparam NUMBER_OF_ITEMS Size of the array
 */
template <typename ItemType, size_t NUMBER_OF_ITEMS>
inline Array<ItemType> MakeArray(const ItemType(&rItemArray)[NUMBER_OF_ITEMS])
{
    return Array<ItemType>(rItemArray);
}

/** Helper for constructing arrays without specifying template parameters
*
* @param pItemArray C array of items
* @param numberOfItems Number of items in the array
*/
template <typename ItemType>
inline Array<ItemType> MakeArray(const ItemType* pItemArray, size_t numberOfItems)
{
    return Array<ItemType>(pItemArray, numberOfItems);
}

} // namespace whiteboard
