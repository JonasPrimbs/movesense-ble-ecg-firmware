#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2016.
All rights reserved.
******************************************************************************/

#include "StructuresTypesTest.Shared.h"
#include "wb-resources/resources.h"
#include "ut-resources/resources.h"

void fillStructure(WB_RES::TestStructure& rStructure);
void checkSerializedData(const WB_RES::TestStructure& rStructure, const uint8* pBuffer, size_t serializationLength);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::TestStructure*);
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::TestStructure& rReferenceData);
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::TestStructure& rReferenceData);

void fillStructure(WB_RES::AllPrimitiveAndBuiltinTypesInAStructure& rStructure);
void checkSerializedData(const WB_RES::AllPrimitiveAndBuiltinTypesInAStructure& rStructure, const uint8* pBuffer, size_t serializationLength);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::AllPrimitiveAndBuiltinTypesInAStructure*);
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::AllPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::AllPrimitiveAndBuiltinTypesInAStructure& rReferenceData);

void fillStructure(WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure& rStructure);
void checkSerializedData(const WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure& rStructure, const uint8* pBuffer, size_t serializationLength);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure*);
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure& rReferenceData);

void fillStructure(WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure& rStructure);
void checkSerializedData(const WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure& rStructure, const uint8* pBuffer, size_t serializationLength);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure*);
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure& rReferenceData);

void fillStructure(WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure& rStructure);
void checkSerializedData(const WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure& rStructure, const uint8* pBuffer, size_t serializationLength);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure*);
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
