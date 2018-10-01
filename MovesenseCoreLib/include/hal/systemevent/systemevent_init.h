#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

// Internal System Event Logging init function
// persistanceAccessible is propagated down to the persistance layer (if any)
// and will enable/inhibit access to extflash there
void SystemEvent_init(bool persistanceAccessible);

/// Deinitialize the System Event module
void SystemEvent_deinit(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
