#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2016.
All rights reserved.
******************************************************************************/

#include "StructuresTypesTest.Shared.h"

WB_STRUCT_PACK_BEGIN()

struct StructuresUnitTestTypesDef
{
    enum Type
    {
        GTEST = 15,
        CPPUNIT = 14,
        NUNIT = 13
    };
};
typedef whiteboard::TypedEnum<StructuresUnitTestTypesDef, StructuresUnitTestTypesDef::Type, uint8> UnitTestTypes;

struct WB_STRUCT_PACKED ComplexUnitTestStruct
{
    // Needed for structure type identification at compile time
    typedef int Structure;
    static const whiteboard::LocalDataTypeId DATA_TYPE_ID = 0x834A;
    static const whiteboard::StructureValueSerializer<ComplexUnitTestStruct> serializer;
    WB_WHEN_STRUCTURE_CLEANING_NEEDED(static const whiteboard::StructureValueCleaner<ComplexUnitTestStruct> cleaner;)

    WB_ALIGNED(whiteboard::WrapperFor32BitPointer<const char>) stringValue;
    WB_ALIGNED(bool) boolValue;
    WB_ALIGNED(int32) int32Value;
    WB_ALIGNED(float) floatValue;
    WB_ALIGNED(int8) int8Value;
    WB_ALIGNED(int64) int64Value;
    WB_ALIGNED(UnitTestTypes) typedEnumValue;
    WB_ALIGNED(whiteboard::Array<uint64>) arrayOfUint64Values;

    inline void visit(whiteboard::IStructureVisitor& rVisitor)
    {
        rVisitor.visit(stringValue)
            .visit(boolValue)
            .visit(int32Value)
            .visit(floatValue)
            .visit(int8Value)
            .visit(int64Value)
            .visit(typedEnumValue)
            .visit(arrayOfUint64Values);
    }
};

WB_STRUCT_PACK_END()

void fillStructure(ComplexUnitTestStruct& rStructure);
void checkSerializedData(const ComplexUnitTestStruct& rStructure, const uint8* pBuffer, size_t serializationLength);

whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const ComplexUnitTestStruct*);
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const ComplexUnitTestStruct& rReferenceData);
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const ComplexUnitTestStruct& rReferenceData);
