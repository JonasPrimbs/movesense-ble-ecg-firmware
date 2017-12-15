#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/metadata/MetadataStructures.h"

/* Whiteboard structures use following data packing and alignment. Most
* compilers produce same kind of structures natively. When porting
* some compile time verifys may break, if compiler has some different
* data packing and alignment. For these cases check if data alignment
* can be modified using command line parameters. An alternative is
* to modify wbres to produce compiler dependent code that insert padding
* between problematic members.
*/

/** Helper macro for getting structure member alignment at compile time.
*
* @note Use this to get what the alignment should be and WB_TYPE_ALIGNMENT
* to get what alignment actually is.
* @note I tried also template specialization, but GCC cannot reference
*       integer constants (static const int x = 1) in attribute(aligned)
*       parameter.
*/
#define WB_STRUCTURE_MEMBER_ALIGNMENT(type) \
    ((::whiteboard::IsSame<type, int64>::value || \
      ::whiteboard::IsSame<type, uint64>::value || \
      ::whiteboard::IsSame<type, double>::value) ? 8 : WB_TYPE_ALIGNMENT(type))

// Shorthand for data type definitions
#define WB_ALIGNED(type) WB_ALIGN(WB_STRUCTURE_MEMBER_ALIGNMENT(type)) type

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

} // namespace whiteboard
