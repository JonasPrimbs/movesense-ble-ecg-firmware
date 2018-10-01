#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/

#include <stdarg.h>
#include <stdint.h>
#include "hal/extflash/areas.h"

#ifdef BUILD_HAL_WANT_PERSISTENT_SYSTEMEVENT
#if defined(__cplusplus)
extern "C" {
#endif

// Block size in bytes used for persistent message logging
// This is the entire message including the header and the user-supplied string (payload)
#define SYSTEMEVENT_PERSISTENT_BLOCK_SIZE (64)

// Total number of available persistent message blocks
#define SYSTEMEVENT_PERSISTENT_BLOCK_COUNT (EXTFLASH_AREA_SYSEVENT_SIZE / SYSTEMEVENT_PERSISTENT_BLOCK_SIZE)

// Payload size within a block
#define SYSTEMEVENT_PAYLOAD_SIZE (SYSTEMEVENT_PERSISTENT_BLOCK_SIZE - sizeof(SystemEvent_Header))

// Header for SystemEvent
typedef struct
{
    uint32_t timeStamp;           // Timestamp.
    uint16_t severityAndModuleId; // Severity and Module ID combined.
    uint16_t systemState;         // System State.
} SystemEvent_Header;

// SystemEvent Block
typedef struct
{
    SystemEvent_Header header;                  // Header.
    char message[SYSTEMEVENT_PAYLOAD_SIZE + 1]; // Payload, extra space for zero termination
} SystemEvent_Block;

// Sequence number block
typedef struct
{
    uint32_t seqNum;
    uint32_t checkNum;
} SequenceNumber_Block;

typedef union {
    SequenceNumber_Block seqNumBlock;
    SystemEvent_Block systemEventBlock;
} SystemEvent_StorageItem;

// Persistent System Event Logging init function
// extflashAvailable controls whether extflash is used at all by the persistent layer
// or just ignored
void SystemEvent_initPersistent(bool extflashAvailable);

// Internal logging function - not for use by any other code!
void SystemEventPersistentV(uint16_t severityAndModuleId, const char* format, va_list args);

// Retrieval function for a persistent message, returns true if successful non-empty message retrieved
bool SystemEventPersistentGetMessage(const uint32_t index, uint32_t* seqNum, SystemEvent_Block* block);

bool SystemEventPersistentErase(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif // BUILD_HAL_WANT_PERSISTENT_SYSTEMEVENT
