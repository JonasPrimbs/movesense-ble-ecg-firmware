#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "../integration/port.h"

/* Whiteboard structures use following data packing and alignment. Most
 * compilers produce same kind of structures natively. When porting
 * some compile time verifys may break, if compiler has some different
 * data packing and alignment. For these cases check if data alignment
 * can be modified using command line parameters. An alternative is
 * to modify wbres to produce compiler dependent code that insert padding
 * between problematic members.
 */

/** Structure members use native packing */

// Structure pack macros
#define WB_STRUCT_PACK_BEGIN()
#define WB_STRUCT_PACK_END()
#define WB_STRUCT_PACKED

#ifdef WB_STRUCTS_SUPPORT_BITFIELDS
#define WB_STRUCT_BITFIELD_PACK_BEGIN() WB_PACK_BEGIN()
#define WB_STRUCT_BITFIELD_PACK_END() WB_PACK_END()
#define WB_STRUCT_BITFIELD_PACKED WB_PACKED
#else
#define WB_STRUCT_BITFIELD_PACK_BEGIN() WB_STRUCT_PACK_BEGIN()
#define WB_STRUCT_BITFIELD_PACK_END() WB_STRUCT_PACK_END()
#define WB_STRUCT_BITFIELD_PACKED WB_STRUCT_PACKED
#endif

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
