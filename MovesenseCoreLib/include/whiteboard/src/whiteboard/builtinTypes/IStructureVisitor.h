#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/Array.h"
#include "whiteboard/builtinTypes/Optional.h"
#include "whiteboard/builtinTypes/TypedEnum.h"
#include "whiteboard/builtinTypes/WrapperFor32BitPointer.h"

namespace whiteboard
{

/** Interface for all classes that use visitor pattern for accessing data structures */
class WB_API IStructureVisitor
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~IStructureVisitor() {}

public:

    /** Visitor pattern implementation. Post processes data after copying data to serialization buffer.
    *
    * @return Reference to this object
    */
    virtual IStructureVisitor& visit(bool&) = 0;
    virtual IStructureVisitor& visit(int8&) = 0;
    virtual IStructureVisitor& visit(int16&) = 0;
    virtual IStructureVisitor& visit(int32&) = 0;
    virtual IStructureVisitor& visit(int64&) = 0;
    virtual IStructureVisitor& visit(uint8&) = 0;
    virtual IStructureVisitor& visit(uint16&) = 0;
    virtual IStructureVisitor& visit(uint32&) = 0;
    virtual IStructureVisitor& visit(uint64&) = 0;
    virtual IStructureVisitor& visit(float&) = 0;
    virtual IStructureVisitor& visit(double&) = 0;

    /** Visitor pattern implementation for enumerations.
    *
    * @tparam Definition Structure type that contains the enumeration definition
    * @tparam DefinitionType Enumeration type
    * @tparam BaseType Underlying data storage type
    * @param rData Enumeration object
    * @return Reference to this object
    */
    template <typename Definition, typename DefinitionType, typename BaseType>
    inline IStructureVisitor& visit(TypedEnum<Definition, DefinitionType, BaseType>& rData)
    {
        return visit(rData.operator BaseType&());
    }

    /** Visitor pattern implementation for optionals.
    *
    * @param rOptional Optional object
    */
    template <typename ItemType>
    inline IStructureVisitor& visit(Optional<ItemType>& rOptional)
    {
        bool valueSet = handleOptional(rOptional.mValueSet);
        if (valueSet)
        {
            visit(rOptional.mValue);
        }

        handleOptionalDone();
        return *this;
    }

    /** Handles optional property visitation
    *
    * @param rValueSet Reference to Optional's valueSet flag
    * @return A value indicating whether sub item should be processed
    */
    virtual bool handleOptional(bool& rValueSet) = 0;

    /** Called when optional visitation is complete */
    inline virtual void handleOptionalDone() {}

    /** Visitor pattern implementation for strings.
    *
    * @param rData Wrapped pointer object
    * @return Reference to this object
    */
    virtual IStructureVisitor& visit(WrapperFor32BitPointer<const char>& rData) = 0;

    /** Visitor pattern implementation for structures
    *
    * @tparam StructureDataType Type of structure
    * @param rData Structure object
    * @return Reference to this object
    */
    template <typename StructureDataType>
    inline IStructureVisitor& visit(StructureDataType& rData)
    {
        handleSubStructure();
        rData.visit(*this);
        handleSubStructureDone();
        return *this;
    }

    /** Called when sub structure is visited */
    inline virtual void handleSubStructure() {};

    /** Called when sub structure visitation is complete */
    inline virtual void handleSubStructureDone() {}

    /** Visitor pattern implementation for sub structures that are not directly
    * stored in parent structure
    *
    * @tparam StructureDataType Sub structure type
    * @param rData Structure object
    * @return Reference to this object
    */
    template <typename StructureDataType>
    inline IStructureVisitor& visit(WrapperFor32BitPointer<StructureDataType>& rData)
    {
        WrapperFor32BitPointer<void>& rGenericPointer = reinterpret_cast<WrapperFor32BitPointer<void>&>(rData);
        void* pointer = handleExternalSubStructure(
            rGenericPointer,
            WB_STRUCTURE_MEMBER_ALIGNMENT(StructureDataType),
            sizeof(StructureDataType));
        StructureDataType* pItem = static_cast<StructureDataType*>(pointer);

        pItem->visit(*this);

        handleExternalSubStructureDone();
        return *this;
    }

    /** Handles external sub structure visitation
     *
     * @param rGenericData Pointer to untyped data structure
     * @param alignmentOfData Alignment of the data structure
     * @param sizeOfData Size of the data structure
     * @return Pointer to data structure where visitor implementation should continue
     */
    virtual void* handleExternalSubStructure(WrapperFor32BitPointer<void>& rGenericData, uint8 alignmentOfData, size_t sizeOfData) = 0;

    /** Called when external sub structure visitation is complete */
    inline virtual void handleExternalSubStructureDone() {}

    /** Visitor pattern implementation for arrays
    *
    * @tparam ItemType Type of array items
    * @param rArray Array object
    * @return Reference to this object
    */
    template <typename ItemType> 
    inline IStructureVisitor& visit(Array<ItemType>& rArray)
    {
        Array<int32>& rGenericArray = reinterpret_cast<Array<int32>&>(rArray);
        size_t numberOfItems;
        void* pointer = handleArray(
            rGenericArray,
            WB_STRUCTURE_MEMBER_ALIGNMENT(ItemType),
            sizeof(ItemType),
            numberOfItems);

        ItemType* pItems = static_cast<ItemType*>(pointer);
        for (size_t i = 0; i < numberOfItems; ++i)
        {
            visit(pItems[i]);
        }

        handleArrayDone();
        return *this;
    }

    /** Handles array visitation
    *
    * @param rGenericData Pointer to untyped array
    * @param alignmentOfData Alignment of the array data
    * @param sizeOfItem Size of the single item
    * @param rNumberOfItems On output contains number of items in the array
    * @return Pointer to item array where visitor implementation should continue
    */
    virtual void* handleArray(Array<int32>& rGenericData, uint8 alignmentOfData, size_t sizeOfItem, size_t& rNumberOfItems) = 0;

    /** Called when array visitation is complete */
    inline virtual void handleArrayDone() {}
};

/** Interface for wrapping visitor pattern calls to structures
* without need for them to implement some IVisitable interface
*/
class WB_API IStructureVisitorWrapper
{
protected:
    /** Prevent deleting implementations through this interface */
    virtual ~IStructureVisitorWrapper() {};

public:
    /** Visitor entry point for the structure
    *
    * @param pData Data that should be visited
    * @param rVisitor Visitor implementation
    */
    virtual void visit(void* pData, IStructureVisitor& rVisitor) const = 0;
};

} // namespace whiteboard
