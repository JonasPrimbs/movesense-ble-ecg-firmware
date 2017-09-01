/******************************************************************************

        Copyright (c) Suunto Oy 2015
        All rights reserved.

******************************************************************************/

#ifndef INCLUDE_SUUNTO_LOWMEM_H
#define INCLUDE_SUUNTO_LOWMEM_H

/**
 * Portable support for LOWMEM define that allows placing data into low memory
 * region that is available on kinetis target. In other environments the keyword
 * will evaluate to nothing (regular allocation)
 *
 * The keyword places the allocation into a section called ".lowmem", which must
 * be declared for the linker in the respective linker scripts in order to work.
 *
 * Example:
 *
 *   #include "common/compiler/lowmem.h"
 *
 *   // globally visible array allocated in lowmem
 *   LOWMEM uint32_t foo[32];
 *
 *   // locally visible data allocated in lowmem (keyword must come first due to
 *   // IAR limitation)
 *   LOWMEM static uint64_t foobar;
 *
 *   // using (nothing special is required)
 *   foo[0] += 2; printf("%p", &foo[2]);
 *
 * Accessing LOWMEM allocated symbols externally should work without requiring
 * the LOWMEM define.
 */

// TODO: This should be much stronger and depend on K65 instead
#ifdef ARCH_CORTEX
#ifdef __IAR_SYSTEMS_ICC__
#define LOWMEM _Pragma("location=\".lowmem\"")
#else
// TODO: Test this
#define LOWMEM __attribute__((section(".lowmem")))
#endif
#else
// on targets that don't have lower memory region, we won't do anything
#define LOWMEM
#endif

#endif // ifndef INCLUDE...
