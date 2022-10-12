/******************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

******************************************************************************/
#ifndef PACK_H_
#define PACK_H_

#if defined(_MSC_VER)

#define PACK_BEGIN() __pragma(pack(push, 1))
#define PACK_END() __pragma(pack(pop))
#define PACKED

#define ALIGNED(x)  

#elif defined(__GNUC__) || defined(__clang__)

#define PACK_BEGIN() _Pragma("pack(push, 1)")
#define PACK_END() _Pragma("pack(pop)")
#define PACKED __attribute__((packed))

#define ALIGNED(x)  __attribute__((aligned(x)))

#elif defined(__IAR_SYSTEMS_ICC__)

#define PACK_BEGIN() _Pragma("pack(push, 1)")
#define PACK_END() _Pragma("pack(pop)")
#define PACKED

#else

#error Pack macros not defined

#define PACK_BEGIN()
#define PACK_END()
#define PACKED

#endif

#endif /* PACK_H_ */
