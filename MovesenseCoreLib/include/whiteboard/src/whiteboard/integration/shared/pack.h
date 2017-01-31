#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2012.
All rights reserved.
******************************************************************************/

// Structure packing macros

#if defined(_MSC_VER)

#define WB_PACK_BEGIN() __pragma(pack(push, 1))
#define WB_PACK_END() __pragma(pack(pop))
#define WB_PACKED

#define WB_PACK_BEGIN_4() __pragma(pack(push, 4))
#define WB_PACKED_4

#elif defined(__GNUC__) || defined(__clang__) || defined(__IAR_SYSTEMS_ICC__)

#define WB_PACK_BEGIN() _Pragma("pack(push, 1)")
#define WB_PACK_END() _Pragma("pack(pop)")
#define WB_PACKED

#define WB_PACK_BEGIN_4() _Pragma("pack(push, 4)")
#define WB_PACKED_4

#else

#error Pack macros not defined

#endif

// Helper macro for disabling warnings about unaligned data access that
// we know are safe
#if defined(__IAR_SYSTEMS_ICC__)

#define WB_SAFE_UNALIGNED_ACCESS_BEGIN() _Pragma("diag_suppress = Pa039")
#define WB_SAFE_UNALIGNED_ACCESS_END() _Pragma("diag_default = Pa039")

#else

#define WB_SAFE_UNALIGNED_ACCESS_BEGIN()
#define WB_SAFE_UNALIGNED_ACCESS_END()

#endif

// Structure member alignment macros
#if defined(__GNUC__) || defined(__clang__)
// Enforce alignment with attribute aligned
#define WB_ALIGN(align) __attribute__((__aligned__((align))))
#else
// Do nothing works on most targets. Static verifys will catch if this is not ok.
#define WB_ALIGN(align)
#endif
