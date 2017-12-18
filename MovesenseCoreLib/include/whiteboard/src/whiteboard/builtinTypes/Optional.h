#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/NoType.h"
#include "whiteboard/builtinTypes/Structures.h"

namespace whiteboard
{

// Forward declarations
class IStructureVisitor;

/** Structure that handles optionality of an structure member */
template <typename ItemType>
struct Optional
{
private:
    friend class IStructureVisitor;

    /** A value indicating whether the item has an value */
    WB_ALIGNED(bool) mValueSet;

    /** Value of the item */
    WB_ALIGNED(ItemType) mValue;

public:
    /** Default constructor */
    inline Optional() : mValueSet(false) {}

    /** Copy constructor */
    inline Optional(const Optional& rOther) { *this = rOther; }

    /** Constructor that initializes the item from given value */
    inline Optional(const ItemType& rValue) : mValueSet(true), mValue(rValue) {}

    /** Constructor that initializes the item with no value */
    inline Optional(const NoType&) : mValueSet(false) {}

    /** Checks whether value has been set */
    inline bool hasValue() const { return mValueSet; }

    /** Gets value (mutable) */
    inline ItemType& getValue()
    {
        WB_ASSERT(mValueSet);
        return mValue;
    }

    /** Gets value (non-mutable) */
    inline const ItemType& getValue() const
    {
        WB_ASSERT(mValueSet);
        return mValue;
    }

    /** Assignment operator */
    inline Optional& operator=(const Optional& rValue)
    {
        mValueSet = rValue.mValueSet;
        if (mValueSet)
        {
            mValue = rValue.mValue;
        }

        return *this;
    }

    /** Assigns given value to the item */
    inline Optional& operator=(const ItemType& rValue)
    {
        mValueSet = true;
        mValue = rValue;
        return *this;
    }

    /** Assigns empty value to the item */
    inline Optional& operator=(const NoType&)
    {
        mValueSet = false;
        return *this;
    }

    /** Comparison operators */
    inline friend bool operator==(const Optional& lhs, const Optional& rhs)
    {
        return (lhs.mValueSet == rhs.mValueSet) && (lhs.mValue == rhs.mValue);
    }
    inline friend bool operator==(const Optional& lhs, const ItemType& rhs) { return lhs.mValueSet && (lhs.mValue == rhs); }
    inline friend bool operator==(const ItemType& lhs, const Optional& rhs) { return rhs.mValueSet && (rhs.mValue == lhs); }
    inline friend bool operator!=(const Optional& lhs, const Optional& rhs) { return !(lhs == rhs); }
    inline friend bool operator!=(const Optional& lhs, const ItemType& rhs) { return !(lhs == rhs); }
    inline friend bool operator!=(const ItemType& lhs, const Optional& rhs) { return !(lhs == rhs); }
};

} // namespace whiteboard
