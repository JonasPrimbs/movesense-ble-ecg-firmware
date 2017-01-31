// Copyright (c) Suunto Oy 2014 - 2016. All rights reserved.

#include "whiteboard/metadata/MetadataStructures.h"
#include "whiteboard/metadata/MetadataMap.h"
#include "whiteboard/Value.h"
#include "quantity/quantities.h"

#define WB_DECLARATIONS_BEGIN()				using namespace whiteboard;
#define WB_DECLARATIONS_END()

#define WB_RESOURCES_BEGIN()				namespace wbresources {
#define WB_RESOURCES_END()						extern const MetadataMap sMetadataMap = { \
													StringMapSize, StringMap, \
													PropertyMapSize, PropertyMap, \
													PropertyListMapSize, PropertyListMap, \
													EnumerationItemListMapSize, EnumerationItemListMap, \
													DataTypeDataSize, DataTypeData, \
													sDataTypeSparseMapSize, sDataTypeSparseMap, \
													ParameterMapSize, ParameterMap, \
													ParameterListMapSize, ParameterListMap, \
													ResponseMapSize, ResponseMap, \
													ResponseListMapSize, ResponseListMap, \
													OperationMapSize, OperationMap, \
													OperationListMapSize, OperationListMap, \
													SecurityTagMapSize, SecurityTagMap }; \
												extern const metadata::ResourceTree sResourceTree = { \
													&sMetadataMap, \
													sPathDataSize, sPathData, \
													sPathSparseMapSize, sPathSparseMap, \
													0, NULL \
												}; \
											}

#define WB_EXECUTION_CONTEXT_IMPL_NAMESPACE_BEGIN(ns)			namespace ns {
#define WB_EXECUTION_CONTEXT_IMPL_NAMESPACE_END()				}

#define WB_EXECUTION_CONTEXT_IMPL_NOTIFICATION_FILTER_FWD_DECL(getFunctorImplementation)	\
    whiteboard::metadata::ExecutionContextNoticationFilter* getFunctorImplementation();
#define WB_EXECUTION_CONTEXT_IMPL_EVENT_PROCESSOR_FWD_DECL(getFunctorImplementation)	\
    whiteboard::metadata::ExecutionContextStateProcessor* getFunctorImplementation();
        
#define WB_EXECUTION_CONTEXT_MAP_BEGIN(size)            extern const metadata::ExecutionContextInfo sExecutionContextMap[size] = {
#define WB_EXECUTION_CONTEXT_DEF(id, name, nDpcs, nReqs, nResps, extThread, priority, stackSize, notificationFilter, eventProcessor) \
	{ name, INIT_EXEC_CTX_SETTINGS(nDpcs, nReqs, nResps, extThread, priority, stackSize, ALL_ACCESS), notificationFilter, eventProcessor },
#define WB_EXECUTION_CONTEXT_MAP_END()                  }; \
                                                        extern const size_t sExecutionContextMapSize = ELEMENTS(sExecutionContextMap);
#define WB_EXECUTION_CONTEXT_MAP_EMPTY()                extern const metadata::ExecutionContextInitInfo* sExecutionContextMap = NULL; \
                                                        extern const size_t sExecutionContextMapSize = 0;

#define WB_STRING_MAP_BEGIN(size)						const size_t StringMapSize = size; \
														const char* const StringMap = ""
#define WB_STRING_DEF(id, value)							value "\0"
#define WB_STRING_MAP_END()								;
#define WB_STRING_MAP_EMPTY()							WB_STRING_MAP_BEGIN(0) WB_STRING_MAP_END()

#define WB_SECURITY_TAG_MAP_BEGIN(size)					const metadata::SecurityTag SecurityTagMap[size] = {
#define WB_SECURITY_TAG_DEF(id, nameId)						{ nameId },
#define WB_SECURITY_TAG_MAP_END()						}; \
														const size_t SecurityTagMapSize = ELEMENTS(SecurityTagMap);
#define WB_SECURITY_TAG_MAP_EMPTY()						const metadata::SecurityTag* const SecurityTagMap = NULL; \
														const size_t SecurityTagMapSize = 0;

#define WB_TYPE_MAP_FWD_DECL(size)						extern const metadata::DataType DataTypeMap[size];
#define WB_TYPE_MAP_FWD_DECL_EMPTY()

#define WB_UINT8S_AS_UINT16(low, high) (static_cast<uint16>(low) | (static_cast<uint16>(high) << 8))
#define WB_UINT16_AS_UINT8S(value) ((value) & 0xff), (((value) >> 8) & 0xff)

WB_STATIC_VERIFY(sizeof(whiteboard::metadata::DataType) == 6, sizeOfDataTypeMetadataIsNotWhatExpected);

#define WB_DATATYPES_BEGIN(size)						const metadata::DataType DataTypeData[size] = {
#define WB_DATATYPES_END()								}; \
														const size_t DataTypeDataSize = ELEMENTS(DataTypeData);
#define WB_DATATYPES_EMPTY()							const metadata::DataType* const DataTypeData = NULL; \
														const size_t DataTypeDataSize = 0;

#define WB_SCALAR_TYPE_WITH_UNIT_DEF(id, type, qId, uId)	{ metadata::DATATYPE_SCALAR, { WB_UINT8S_AS_UINT16(type, 0), WB_UINT8S_AS_UINT16(qId, uId) } },
#define WB_SCALAR_TYPE_DEF(id, type)						WB_SCALAR_TYPE_WITH_UNIT_DEF(id, type, quantity::NUMERIC, unit::BaseId)
#define WB_NULL_TYPE_DEF(id)								WB_SCALAR_TYPE_DEF(id, WB_TYPE_NONE)
#define WB_NAMED_TYPE_DEF(id, nameId, typeId)				{ metadata::DATATYPE_NAMED, { nameId, typeId } },
#define WB_ARRAY_TYPE_DEF(id, itemTypeId)					{ metadata::DATATYPE_ARRAY, { itemTypeId, 0 } },
#define WB_STRUCT_TYPE_DEF(id, propertyListId)				{ metadata::DATATYPE_STRUCTURE,{ propertyListId, 0 } },
#define WB_ENUM_TYPE_DEF(id, baseTypeId, itemListId)		{ metadata::DATATYPE_ENUMERATION, { baseTypeId, itemListId } },

#define WB_PROPERTY_MAP_BEGIN(size)						const metadata::Property PropertyMap[size] = {
#define WB_PROPERTY_DEF(id, nameId, typeId, required, inlineStorage) \
										 				{ nameId, typeId, required, inlineStorage },
#define WB_PROPERTY_MAP_END()							}; \
														const size_t PropertyMapSize = ELEMENTS(PropertyMap);
#define WB_PROPERTY_MAP_EMPTY()							const metadata::Property* const PropertyMap = NULL; \
														const size_t PropertyMapSize = 0;

#define WB_PROPERTY_LISTS_BEGIN(size)					const metadata::PropertyId PropertyListMap[] = {
#define WB_PROPERTY_LISTS_END()							}; \
														const size_t PropertyListMapSize = ELEMENTS(PropertyListMap);
#define WB_PROPERTY_LISTS_EMPTY()						const metadata::PropertyRef* const PropertyListMap = NULL; \
														const size_t PropertyListMapSize = 0;

#define WB_PROPERTY_LIST_BEGIN(id, size) 				
#define WB_PROPERTY_LIST_NODE_DEF(propertyId)				propertyId,
#define WB_PROPERTY_LIST_END() 								metadata::INVALID_PROPERTY,
#define WB_PROPERTY_LIST_EMPTY() 							metadata::INVALID_PROPERTY,

#define WB_ENUM_ITEM_LISTS_BEGIN(size)					const metadata::EnumerationItem EnumerationItemListMap[] = {
#define WB_ENUM_ITEM_LISTS_END()							}; \
														const size_t EnumerationItemListMapSize = ELEMENTS(EnumerationItemListMap);
#define WB_ENUM_ITEM_LISTS_EMPTY()						const metadata::EnumerationItem* const EnumerationItemListMap = NULL; \
														const size_t EnumerationItemListMapSize = 0;

#define WB_ENUM_ITEM_LIST_BEGIN(id, size) 				
#define WB_ENUM_ITEM_LIST_NODE_DEF(nameId, value)				{ nameId, value },
#define WB_ENUM_ITEM_LIST_END() 								{ metadata::INVALID_STRING, 0 },
#define WB_ENUM_ITEM_LIST_EMPTY() 								{ metadata::INVALID_STRING, 0 },

#define WB_PARAMETER_MAP_BEGIN(size)					const metadata::Parameter ParameterMap[size] = {
#define WB_PARAMETER_DEF(id, nameId, required, typeId) 		{ nameId, required, typeId },
#define WB_PARAMETER_MAP_END()							}; \
														const size_t ParameterMapSize = ELEMENTS(ParameterMap);
#define WB_PARAMETER_MAP_EMPTY()						const metadata::Parameter* const ParameterMap = NULL; \
														const size_t ParameterMapSize = 0;


#define WB_PARAMETER_LISTS_BEGIN(size)					const metadata::ParameterId ParameterListMap[] = {
#define WB_PARAMETER_LISTS_END()						}; \
														const size_t ParameterListMapSize = ELEMENTS(ParameterListMap);
#define WB_PARAMETER_LISTS_EMPTY()						const metadata::ParameterRef* const ParameterListMap = NULL; \
														const size_t ParameterListMapSize = 0;

#define WB_NO_PARAMETER									metadata::INVALID_PARAMETER

#define WB_PARAMETER_LIST_BEGIN(id, size)
#define WB_PARAMETER_LIST_NODE_DEF(parameterId) 			parameterId,
#define WB_PARAMETER_LIST_END()								WB_NO_PARAMETER,
#define WB_PARAMETER_LIST_EMPTY()


#define WB_VOID_RESPONSE metadata::INVALID_DATATYPE

#define WB_RESPONSE_MAP_BEGIN(size)						const metadata::Response ResponseMap[size] = {
#define WB_RESPONSE_DEF(id, code, typeId) 					{ static_cast<Result>(code), typeId },
#define WB_RESPONSE_MAP_END()							}; \
														const size_t ResponseMapSize = ELEMENTS(ResponseMap);
#define WB_RESPONSE_MAP_EMPTY() 						const metadata::Response* const ResponseMap = NULL; \
														const size_t ResponseMapSize = 0;


#define WB_RESPONSE_LISTS_BEGIN(size)					const metadata::ResponseId ResponseListMap[] = { 
#define WB_RESPONSE_LISTS_END()							}; \
														const size_t ResponseListMapSize = ELEMENTS(ResponseListMap);
#define WB_RESPONSE_LISTS_EMPTY() 						const metadata::ResponseRef* const ResponseListMap = NULL; \
														const size_t ResponseListMapSize = 0;

#define WB_RESPONSE_LIST_BEGIN(id, size)				
#define WB_RESPONSE_LIST_NODE_DEF(responseId) 				responseId,
#define WB_RESPONSE_LIST_END()								metadata::INVALID_RESPONSE,
#define WB_RESPONSE_LIST_EMPTY()							metadata::INVALID_RESPONSE,


#define WB_OPERATION_MAP_BEGIN(size)					const metadata::Operation OperationMap[size] = {
#define WB_OPERATION_DEF(id, type, parameterListId, responseListId, securityMask) \
															{ type, parameterListId, responseListId, securityMask },
#define WB_OPERATION_MAP_END()							}; \
														const size_t OperationMapSize = ELEMENTS(OperationMap);
#define WB_OPERATION_MAP_EMPTY() 						const metadata::Operation* const OperationMap = NULL; \
														const size_t OperationMapSize = 0;

#define WB_NO_OPERATION metadata::INVALID_OPERATION

#define WB_OPERATION_LISTS_BEGIN(size)					const metadata::OperationList OperationListMap[size] = {
#define WB_OPERATION_LIST(id, getId, putId, postId, deleteId, subscribeId, unsubscribeId) \
															{ { getId, putId, postId, deleteId, subscribeId, unsubscribeId } },
#define WB_OPERATION_LISTS_END()						}; \
														const size_t OperationListMapSize = ELEMENTS(OperationListMap);
#define WB_OPERATION_LISTS_EMPTY() 						const metadata::OperationList* const OperationListMap = NULL; \
														const size_t OperationListMapSize = 0;

#define WB_END_OF_PATH static_cast<LocalResourceId>(-1)

#define WB_PATHS_BEGIN(size)				const metadata::ResourceTreeNode sPathData[size] = { 
#define WB_PATHS_END()						}; \
											extern const size_t sPathDataSize = ELEMENTS(sPathData);
#define WB_PATHS_EMPTY()					const metadata::ResourceTreeNode* const sPathData = NULL; \
											extern const size_t sPathDataSize = 0;

#define WB_PATH_FLAGS(executionContextId, pathParameterCount) \
	((static_cast<uint16>(executionContextId) << 1) | \
	 (static_cast<uint16>(pathParameterCount) << 5))

#define WB_PATH_DEF(id, name, executionContextId, pathParameterCount, operationListId, pathVariableId, parentId, nextSiblingId, firstChildId) \
											{ { \
												name, \
												WB_PATH_FLAGS(executionContextId, pathParameterCount), \
												parentId, \
												nextSiblingId, \
												firstChildId, \
												operationListId, \
												pathVariableId \
											} },

#define WB_LINK_DEF(id, name, pathType, parentId, nextSiblingId, linkedResourceId) \
											{ { \
												name, \
												pathType, \
												parentId, \
												nextSiblingId, \
												linkedResourceId, \
												0, \
												0 \
											} },

#define WB_SPARSE_MAP_ENTRY(id, index)		index,

#define WB_DATATYPE_SPARSE_MAP_BEGIN(size)	const metadata::DataTypeId sDataTypeSparseMap[size] = { 
#define WB_DATATYPE_SPARSE_MAP_END()		}; \
											const size_t sDataTypeSparseMapSize = ELEMENTS(sDataTypeSparseMap);
#define WB_DATATYPE_SPARSE_MAP_EMPTY() 		metadata::DataTypeId* const sDataTypeSparseMap = NULL; \
											const size_t sDataTypeSparseMapSize = 0;

#define WB_PATH_SPARSE_MAP_BEGIN(size)		const LocalResourceId sPathSparseMap[size] = { 
#define WB_PATH_SPARSE_MAP_END()			}; \
											const size_t sPathSparseMapSize = ELEMENTS(sPathSparseMap);
#define WB_PATH_SPARSE_MAP_EMPTY() 			LocalResourceId* const sPathSparseMap = NULL; \
											const size_t sPathSparseMapSize = 0;

/** Helper function for force linking of the library
* (Visual C will ignore whole library if none of the symbols are referenced)
*/
void pullInWbResources()
{
}

