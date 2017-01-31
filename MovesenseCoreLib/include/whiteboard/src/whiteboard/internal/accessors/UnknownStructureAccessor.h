#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/builtinTypes/UnknownStructure.h"

namespace whiteboard
{

/** Static interface to provide a more priviledged access to UnknownStructure class */
class UnknownStructureAccessor
{
public:
    /** Creates a new instance of UnknownStructure class
    *
    * @param dataTypeId ID of the data type
    * @param protocolVersion Protocol version that was used to encode the structure
    * @param pData Encoded data
    * @param dataOwner Ownership of the data
    */
    static inline UnknownStructure makeUnknownStructure(
        LocalDataTypeId dataTypeId, ProtocolVersion protocolVersion, const void* pData, bool dataOwner);

    /** Gets data pointer of a structure data of UnknownStructure instance
     *
     * @param rStructure The structure to access
     * @return Pointer to unknown structure data
     */
    static inline const void* getData(const UnknownStructure& rStructure);
};

inline UnknownStructure UnknownStructureAccessor::makeUnknownStructure(
    LocalDataTypeId dataTypeId, ProtocolVersion protocolVersion, const void* pData, bool dataOwner)
{
    return UnknownStructure(dataTypeId, protocolVersion, pData, dataOwner);
}

inline const void* UnknownStructureAccessor::getData(const UnknownStructure& rStructure)
{
    return rStructure.mpData;
}

} // namespace whiteboard
