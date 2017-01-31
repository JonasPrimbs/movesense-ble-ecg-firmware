#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/builtinTypes/IStructureVisitor.h"
#include "whiteboard/IValueCleaner.h"

namespace whiteboard
{

#ifdef WB_NEED_STRUCTURE_CLEANING

/** Class that handles cleanup of pointers that are stored in pointer wrapper pool */
class StructureCleaner FINAL : public IStructureVisitor
{
public:
    /** Default constructor
    */
    EXPLICIT inline StructureCleaner() {}

    /** Destructor */
    inline virtual ~StructureCleaner() {}

    /** Cleans pointer wrappers from given object
    *
    * @tparam StructureDataType Type of the structure
    * @param rData Reference to the object that should be cleaned
    */
    template <typename StructureDataType>
    inline static void cleanup(StructureDataType& rData)
    {
        StructureCleaner cleaner;
        static_cast<IStructureVisitor&>(cleaner).visit(rData);
    }

    /** Visitor pattern implementation.
    *
    * @return Reference to this object
    */
    IStructureVisitor& visit(bool&) OVERRIDE { return *this; }
    IStructureVisitor& visit(int8&) OVERRIDE { return *this; }
    IStructureVisitor& visit(int16&) OVERRIDE { return *this; }
    IStructureVisitor& visit(int32&) OVERRIDE { return *this; }
    IStructureVisitor& visit(int64&) OVERRIDE { return *this; }
    IStructureVisitor& visit(uint8&) OVERRIDE { return *this; }
    IStructureVisitor& visit(uint16&) OVERRIDE { return *this; }
    IStructureVisitor& visit(uint32&) OVERRIDE { return *this; }
    IStructureVisitor& visit(uint64&) OVERRIDE { return *this; }
    IStructureVisitor& visit(float&) OVERRIDE { return *this; }
    IStructureVisitor& visit(double&) OVERRIDE { return *this; }

    /** Visitor pattern implementation for strings.
    *
    * @param rData Wrapped pointer object
    * @return Reference to this object
    */
    IStructureVisitor& visit(WrapperFor32BitPointer<const char>& rData) OVERRIDE;

    /** Handles optional property visitation
    *
    * @param rValueSet Reference to Optional's valueSet flag
    * @return A value indicating whether sub item should be processed
    */
    bool handleOptional(bool& rValueSet) OVERRIDE;

    /** Handles external sub structure visitation
    *
    * @param rGenericData Pointer to untyped data structure
    * @param alignmentOfData Alignment of the data structure
    * @param sizeOfData Size of the data structure
    * @return Pointer to data structure where visitor implementation should continue
    */
    void* handleExternalSubStructure(WrapperFor32BitPointer<void>& rGenericData, uint8, size_t) OVERRIDE;

    /** Handles array visitation
    *
    * @param rGenericData Pointer to untyped array
    * @param alignmentOfData Alignment of the array data
    * @param sizeOfItem Size of the single item
    * @param rNumberOfItems On output contains number of items in the array
    * @return Pointer to item array where visitor implementation should continue
    */
    void* handleArray(Array<int32>& rGenericData, uint8, size_t, size_t& rNumberOfItems) OVERRIDE;
};
#endif

} // namespace whiteboard
