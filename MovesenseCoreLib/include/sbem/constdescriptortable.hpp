#pragma once

#include "common/compiler/genType.h"

namespace nea
{
namespace sbem
{

struct ConstDescriptor
{
    uint16 const m_id;
    uint16 const m_aliasId;
    uint16 const m_enumListHead;
    uint16 const m_plusDescriptor; /**< Descriptor opened by plus descriptor */
    uint8 const m_type;
    uint8 const m_format;
    uint8 const m_numOfDescriptors;
    bool const m_isGroup;
    char const* const m_name;
    char const* const m_parentAlias;
    uint16 const* const listOfDescriptors;
};

struct ConstEnum
{
    uint16 const m_next;
    int16 const m_value;
    char const* const m_string;
};

struct ConstAlias
{
    char const* const m_string;
    uint16 const m_to;
};

struct ConstDescriptorTable
{
    ConstAlias const* const m_aliases;
    ConstDescriptor const* const m_descriptors;
    ConstEnum const* const m_enums;
    uint16 const m_numOfAliases;
    uint16 const m_numOfDescriptors;
    uint16 const m_numOfEnums;
};
}
}
