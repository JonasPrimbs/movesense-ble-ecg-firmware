#pragma once
// Copyright (c) Suunto Oy 2018. All rights reserved.

#ifdef WB_HAVE_CRASH_DUMP

/** In certain ports, where it is not possible to write system events inside CriticalSection, extra memory
area is needed that survices over the system restart.

Note: In ports where system event writing is possible inside critical section, use the default implementation for this
function, then Whiteboard crashdump functionality will directly write crashdump as systemevents.

@param crashDumpMemorySize [out] The size of the returned crash dump memory area in bytes. 1kt is required for the crash dump.

@return Pointer to the memory are that shall be available after reboot. NULL if not implemented in the port.
*/
WB_API void* WbGetCrashDumpMemoryArea(size_t& crashDumpMemorySize);

#endif // #ifdef WB_HAVE_CRASH_DUMP