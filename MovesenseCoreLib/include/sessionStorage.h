#ifndef SESSION_STORAGE_H_
#define SESSION_STORAGE_H_
/******************************************************************************

    Copyright (c) Suunto Oy 2014.
    All rights reserved.

    $Id: sessionStorage.h 69633 2014-10-21 15:47:49Z treyh $

******************************************************************************/
/**
    @file sessionStorage.h
*/

#include "common/compiler/genDef.h"
#include "common/compiler/pack.h"
#include "chunkStorage.h"

#ifndef MIN_SESSION_LEN
    #define MIN_SESSION_LEN_ms        30000    // forget shorter sessions
#endif
#ifndef MAX_SESSIONS
    #define MAX_SESSIONS            50
#endif

class SessionStorage
{
public:
    union SessionFlags
    {
        uint16_t value;
        struct 
        {
            uint8_t isValid : 1;
            uint8_t absoluteTime : 1;
            uint8_t relativeTime : 1;
            uint8_t containsRr : 1;
        } PACKED bits;
    } PACKED;

    struct SessionHeader
    {
        uint16_t id;
        SessionFlags flags;
        uint32_t firstChunk;
        uint64_t startTime;
        uint32_t duration;
    } PACKED;

    static const uint32_t slots = MAX_SESSIONS;
    static const size_t slotSize = sizeof(SessionHeader); //EepromChecksumedAccessor::overhead + sizeof(SessionHeader);
    static const size_t size = slots * slotSize;

    SessionStorage( uint32_t eepromStartAddress, ChunkStorage &measChunkStorage_ );
    bool getSessionInfo( uint32_t slotIndex, SessionHeader *dest );
    bool saveSessionInfo( uint32_t slotIndex, SessionHeader *session );

    // Update offset at power up and whenever it changes (value 0 = unknown)
    void setAbsoluteTimeOffset( uint64_t absoluteTimeOffset_ );
    void relativeTimeLost();

    bool startSession( uint16_t sessionId, uint64_t startTimeRelative, uint32_t firstChunk );
    bool endSession( uint32_t duration, bool containsRr );
    bool addSession( uint16_t sessionId, uint64_t startTimeRelative, uint32_t firstChunk, uint32_t duration, bool containsRr );

private:
    static const uint32_t INVALID_SLOT = 0xFF;

    // Updates sessions validity info
    void updateSessionsValidity();

    // Next session starting chunk to be destroyed by overwriting. As long as
    // this is valid, the session list is also valid.
    ChunkStorage::Iterator firstSessionStart;    

    ChunkStorage &measChunkStorage;
    uint32_t address;
    uint64_t absoluteTimeOffset;
    uint8_t curSessionSlot;
    STATIC_VERIFY( MAX_SESSIONS <= 255, TOO_HIGH_MAX_SESSIONS_FOR_curSessionSlot );

    struct Bit
    {
        uint8_t value : 1;
    } PACKED;
    struct
    {
        uint8_t initialized : 1;
        Bit isValid[MAX(1,MAX_SESSIONS)];
    } PACKED sessionSlotValidity;
};

#endif
