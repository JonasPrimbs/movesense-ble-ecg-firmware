#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/IValueCleaner.h"
#include "whiteboard/IValueSerializer.h"

#include "whiteboard/builtinTypes/StructurePack.h"
#include "whiteboard/builtinTypes/IStructureVisitor.h"
#include "whiteboard/builtinTypes/StructureCleaner.h"

namespace whiteboard
{

// SFINAE test for checking if type is a structure
template <typename Type> class IsStructure
{
    typedef int8 one;
    typedef int32 two;

    template <typename C> static one test(typename C::Structure);
    template <typename C> static two test(...);

public:
    enum
    {
        value = sizeof(test<Type>(0)) == sizeof(int8)
    };
};

/** Base helper class for doing structure serialization from Value class objects.
*/
class WB_API StructureValueSerializerBase : public IValueSerializer, protected IStructureVisitorWrapper
{
public:
    /** Default constructor */
    inline StructureValueSerializerBase() {}

    /** Destructor */
    inline ~StructureValueSerializerBase() {}

    /** Calculates serialization length of a structure
    *
    * @param rStructureSerializationLengthCalculator Structure serialization length calculator to use
    * @param bufferOffset Buffer offset for serialization start. Needed for alignment calculations.
    * @param pData Pointer to the actual object that should be serialized
    * @return Required serialization length in bytes.
    */
    size_t getStructureSerializationLength(
        const IStructureSerializationLengthCalculator& rStructureSerializationLengthCalculator,
        size_t bufferOffset,
        const void* pData) const OVERRIDE FINAL;

    /** Serializes the structure
    *
    * @param rStructureSerializer Structure serializer to use
    * @param pBuffer Destination buffer
    * @param bufferLength Length of the buffer
    * @param pData Pointer to the actual object that should be serialized
    * @param isReceiverDataType Is the message in receiver format (true)
    * @return Length of the serialized data in bytes.
    */
    size_t serializeStructure(
        const IStructureSerializer& rStructureSerializer,
        void* pBuffer,
        size_t bufferLength,
        const void* pData,
        bool isReceiverDataType) const OVERRIDE FINAL;


    /// @see whiteboard::IValueSerializer::deserializeStructureHeader
    bool deserializeStructureHeader(
        const void* pData,
        const IStructureDeserializer& rStructureDeserializer,
        bool& rIsReceiverDataType,
        bool& rIsSenderDataType) const OVERRIDE;

    /** Deserializes a structure
    *
    * @param rStructureDeserializer Structure deserializer instance
    * @param pBuffer Pointer to the buffer that has serialized structure
    * @param rIsReceiverDataType On output contains a value that indicates whether this is data type with receiver's data type ID
    * @param rIsSenderDataType On output contains a value that indicates whether this is data type with sender's data type ID
    * @return  Pointer to deserialized structure within the buffer or NULL on failure
    */
    const void* deserializeStructure(
        const IStructureDeserializer& rStructureDeserializer,
        void* pBuffer,
        bool& rIsReceiverDataType,
        bool& rIsSenderDataType) const OVERRIDE FINAL;

protected:
    /** Gets alignment of the structure
     *
     * @param rSize On output contains size of the structure
     * @param rAlignment On output contains alignment of the structure 
     */
    virtual void getStructureSizeAndAlignment(size_t& rSize, uint8& rAlignment) const = 0;
};

/** Helper class for doing structure serialization from Value class objects.
*
* @tparam StructureType Type of the structure
*/
template <typename StructureType> class WB_API StructureValueSerializer FINAL 
    : public StructureValueSerializerBase
{
public:
    /** Default constructor */
    inline StructureValueSerializer() {}

    /** Destructor */
    inline ~StructureValueSerializer() {}

    /** Gets alignment of the structure
    *
    * @param rSize On output contains size of the structure
    * @param rAlignment On output contains alignment of the structure
    */
    void getStructureSizeAndAlignment(size_t& rSize, uint8& rAlignment) const OVERRIDE
    {
        rSize = sizeof(StructureType);
        rAlignment = WB_STRUCTURE_MEMBER_ALIGNMENT(StructureType);
    }

private:
    /** Visitor entry point for the structure
    *
    * @param pData Data that should be visited
    * @param rVisitor Visitor implementation
    */
    void visit(void* pData, IStructureVisitor& rVisitor) const OVERRIDE
    {
        static_cast<StructureType*>(pData)->visit(rVisitor);
    }
};

template <typename ItemType> class WB_API StructureValueSerializer<Array<ItemType> > FINAL
    : public StructureValueSerializerBase
{
public:
    /** Default constructor */
    inline StructureValueSerializer() {}

    /** Destructor */
    inline ~StructureValueSerializer() {}

    /** Gets alignment of the structure
    *
    * @param rSize On output contains size of the structure
    * @param rAlignment On output contains alignment of the structure
    */
    void getStructureSizeAndAlignment(size_t& rSize, uint8& rAlignment) const OVERRIDE
    {
        rSize = sizeof(Array<ItemType>);
        rAlignment = WB_STRUCTURE_MEMBER_ALIGNMENT(Array<ItemType>);
    }

private:
    /** Visitor entry point for the structure
    *
    * @param pData Data that should be visited
    * @param rVisitor Visitor implementation
    */
    void visit(void* pData, IStructureVisitor& rVisitor) const OVERRIDE
    {
        // Calling visitor here directly as Array doesn't implement visitor
        // pattern. Can't do that, because of cyclic header depeendencies
        rVisitor.visit(*static_cast<Array<ItemType>*>(pData));
    }
};

#ifdef WB_NEED_STRUCTURE_CLEANING
/** Helper class for doing structure cleaner after deserialized structure is not used any more.
*
* @tparam StructureType Type of the structure
*/
template <typename StructureType> class WB_API StructureValueCleaner : public IValueCleaner
{
public:
    /** Default constructor */
    inline StructureValueCleaner() {}

    /** Destructor */
    inline ~StructureValueCleaner() {}

    /** Cleans the structure after it has been used
    *
    * @param pData Pointer to data that should be cleaned
    */
    inline void clean(void* pData) const OVERRIDE FINAL
    {
        StructureCleaner::cleanup<StructureType>(*static_cast<StructureType*>(pData));
    }
};
#endif

} // namespace whiteboard
