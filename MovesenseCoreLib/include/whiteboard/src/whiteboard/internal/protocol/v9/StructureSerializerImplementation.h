#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/builtinTypes/IStructureVisitor.h"
#include "whiteboard/internal/protocol/Padding.h"

namespace whiteboard
{
namespace protocol_v9
{

//** Structure serializer implementation */
class StructureSerializerImplementation FINAL : public IStructureVisitor
{
public:
    /** Constructor
    *
    * @param pBuffer Destination buffer
    * @param bufferLength Length of the buffer
    */
    inline StructureSerializerImplementation(void* pBuffer, size_t bufferLength)
        : mBuffer(pBuffer), mMaximumLength(bufferLength), mReserved(0), mPadding(0)
    {
    }

    /** Destructor */
    virtual ~StructureSerializerImplementation() {}

    /** Calculates new offset for given alignment
    *
    * @param rOffset Serialization offset
    * @param alignment Data alignment
    * @return A value indicating whether the offset remained in valid data range
    */
    inline bool align(size_t& rOffset, uint8 alignment)
    {
        // Aling in respect to the current buffer alignment
        size_t bufferOffset = reinterpret_cast<size_t>(mBuffer) + rOffset;
        bufferOffset += padding(bufferOffset, alignment);
        rOffset = static_cast<uint32>(bufferOffset - reinterpret_cast<size_t>(mBuffer));
        return rOffset < mMaximumLength;
    }

    /** Reserves space for a block serialization
    *
    * @param alignment Alignment of the block
    * @param numberOfBytes Number of bytes to reserve
    * @return Offset of the reserved block
    */
    size_t reserve(uint8 alignment, size_t numberOfBytes);

    /** Writes data
    *
    * @param rOffset Write offset
    * @param pData Data buffer that should be written
    * @param numberOfBytes Length of the buffer in bytes
    * @return A value indicating whether data was successfully written
    */
    virtual bool write(size_t& rOffset, const void* pData, size_t numberOfBytes);

    /** Writes data
    *
    * @param rOffset Write offset
    * @param rData Data structure that should be written
    * @return A value indicating whether data was successfully written
    */
    template <typename Type>
    inline bool write(size_t& rOffset, const Type& rData)
    {
        if (!write(rOffset, &rData, sizeof(Type)))
        {
            return false;
        }

        return true;
    }

    /** Gets currently reserved space
    *
    * @return Reserved space in bytes
    */
    inline size_t getReservedSpace() const
    {
        return mReserved;
    }

    /** Gets padding between the structure header and the structure in the current serialization buffer
    * to align the structure properly
    *
    * @returns Padding in bytes
    */
    inline uint8 getPadding() const
    {
        return mPadding;
    }

    /** Sets padding between the structure header and the structure in the current serialization buffer
    * to align the structure properly
    *
    * @param padding Padding in bytes
    */
    inline void setPadding(uint8 padding)
    {
        mPadding = padding;
    }

    /** Converts offset from serialization start to offset to structure start
    *
    * @param offset Offset to serialization start
    * @return Offset to structure start
    */
    inline uint32 toStructureOffset(size_t offset)
    {
        return static_cast<uint32>(offset - mPadding);
    }

    /** Visitor pattern implementation. Post processes data after copying data to serialization buffer.
    *
    * @return Reference to this object
    */
    IStructureVisitor& visit(bool&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(int8&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(int16&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(int32&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(int64&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(uint8&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(uint16&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(uint32&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(uint64&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(float&) OVERRIDE FINAL { return *this; }
    IStructureVisitor& visit(double&) OVERRIDE FINAL { return *this; }

    /** Visitor pattern implementation for strings.
    *
    * @param rData Wrapped pointer object
    * @return Reference to this object
    */
    IStructureVisitor& visit(WrapperFor32BitPointer<const char>& rData) OVERRIDE FINAL;

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
    void* handleExternalSubStructure(WrapperFor32BitPointer<void>& rGenericData, uint8 alignmentOfData, size_t sizeOfData) OVERRIDE FINAL;

    /** Handles array visitation
    *
    * @param rGenericData Pointer to untyped array
    * @param alignmentOfData Alignment of the array data
    * @param sizeOfItem Size of the single item
    * @param rNumberOfItems On output contains number of items in the array
    * @return Pointer to item array where visitor implementation should continue
    */
    void* handleArray(Array<int32>& rGenericData, uint8 alignmentOfData, size_t sizeOfItem, size_t& rNumberOfItems) OVERRIDE FINAL;

private:
    /** Data buffer */
    void* mBuffer;

    /** Maximum length of the buffer */
    size_t mMaximumLength;

    /** Current chunk reservation position */
    size_t mReserved;

    /** Padding between the structure header and the structure in the current serialization buffer to align the structure properly */
    uint8 mPadding;
};

} // namespace protocol_v9
} // namespace whiteboard
