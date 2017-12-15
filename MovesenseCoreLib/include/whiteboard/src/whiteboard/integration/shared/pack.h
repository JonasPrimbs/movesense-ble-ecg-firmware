#pragma once
// Copyright (c) Suunto Oy 2012. All rights reserved.

#include "whiteboard/integration/shared/macros.h"

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

// Structure member alignment macros
#if defined(WB_HAVE_CPLUSPLUS_11)
// Use standard version of alignment control
#define WB_ALIGN(align) alignas(align)
#elif defined(__GNUC__) || defined(__clang__)
// Enforce alignment with attribute aligned
#define WB_ALIGN(align) __attribute__((__aligned__((align))))
#else
// Do nothing works on most targets. Static verifys will catch if this is not ok.
#define WB_ALIGN(align)
#endif
