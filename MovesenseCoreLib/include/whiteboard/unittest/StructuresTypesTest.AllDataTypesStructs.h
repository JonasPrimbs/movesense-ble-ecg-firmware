#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2016.
All rights reserved.
******************************************************************************/

#include "StructuresTypesTest.Shared.h"
#include "wb-resources/resources.h"
#include "ut-resources/resources.h"

void fillStructure(WB_RES::TestStructure& rStructure);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::TestStructure*);
#ifdef WB_HAVE_UNKNOWN_STRUCTURES
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::TestStructure& rReferenceData);
#endif
#ifdef WB_HAVE_ALIEN_STRUCTURES
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::TestStructure& rReferenceData);
#endif

void fillStructure(WB_RES::AllPrimitiveAndBuiltinTypesInAStructure& rStructure);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::AllPrimitiveAndBuiltinTypesInAStructure*);
#ifdef WB_HAVE_UNKNOWN_STRUCTURES
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::AllPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
#endif
#ifdef WB_HAVE_ALIEN_STRUCTURES
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::AllPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
#endif

void fillStructure(WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure& rStructure);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure*);
#ifdef WB_HAVE_UNKNOWN_STRUCTURES
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
#endif
#ifdef WB_HAVE_ALIEN_STRUCTURES
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
#endif

void fillStructure(WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure& rStructure);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure*);
#ifdef WB_HAVE_UNKNOWN_STRUCTURES
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
#endif
#ifdef WB_HAVE_ALIEN_STRUCTURES
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
#endif

void fillStructure(WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure& rStructure);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure*);
#ifdef WB_HAVE_UNKNOWN_STRUCTURES
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
#endif
#ifdef WB_HAVE_ALIEN_STRUCTURES
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure& rReferenceData);
#endif

void fillStructure(WB_RES::StructureWithHashString& rStructure);
whiteboard::IDataTypeMetadataContainer* getMetadataContainer(const WB_RES::StructureWithHashString*);
#ifdef WB_HAVE_UNKNOWN_STRUCTURES
whiteboard::IStructureDataVisitor* getDynamicDeserializationValidator(const WB_RES::StructureWithHashString& rReferenceData);
#endif
#ifdef WB_HAVE_ALIEN_STRUCTURES
whiteboard::IStructureDataAccessor* getDynamicSerializationDataAccessor(const WB_RES::StructureWithHashString& rReferenceData);
#endif
