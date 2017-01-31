#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/builtinTypes/IStructureVisitor.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Actual implemetation of structrue serilization length calculation */
class StructureSerializationLengthCalculatorImplementation FINAL : public IStructureVisitor
{
public:
    /** Default constructor
    *
    * @param bufferOffset Offset of the buffer (for correct alignment)
    */
    inline StructureSerializationLengthCalculatorImplementation(size_t bufferOffset)
        : mBufferOffset(bufferOffset), mReserved(0)
    {
    }

    /** Destructor */
    virtual ~StructureSerializationLengthCalculatorImplementation() {}

    /** Reserves space for a block serialization
    *
    * @param alignment Alignment of the block
    * @param numberOfBytes Number of bytes to reserve
    */
    void reserve(uint8 alignment, size_t numberOfBytes);

    /** Gets currently reserved space
    *
    * @return Reserved space in bytes
    */
    inline size_t getReservedSpace() const
    {
        return mReserved;
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
    void* handleExternalSubStructure(WrapperFor32BitPointer<void>& rGenericData, uint8 alignmentOfData, size_t sizeOfData) OVERRIDE;

    /** Handles array visitation
    *
    * @param rGenericData Pointer to untyped array
    * @param alignmentOfData Alignment of the array data
    * @param sizeOfItem Size of the single item
    * @param rNumberOfItems On output contains number of items in the array
    * @return Pointer to item array where visitor implementation should continue
    */
    void* handleArray(Array<int32>& rGenericData, uint8 alignmentOfData, size_t sizeOfItem, size_t& rNumberOfItems) OVERRIDE;

private:
    /** Buffer serialization offset (for alignment) */
    size_t mBufferOffset;

    /** Current chunk reservation position */
    size_t mReserved;
};

} // namespace protocol_v9
} // namespace whiteboard
