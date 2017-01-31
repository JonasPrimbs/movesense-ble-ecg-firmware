#ifndef EEPROM_CHUNK_STORAGE_H_
#define EEPROM_CHUNK_STORAGE_H_
/******************************************************************************

    Copyright (c) Suunto Oy 2014.
    All rights reserved.

    $Id: eepromChunkStorage.h 69642 2014-10-22 07:36:46Z treyh $

******************************************************************************/
/**
    @file eepromChunkStorage.h
    Eeprom ChunkStorage implementation.
*/


#include <stdint.h>
#include <stdbool.h>

#include "common/compiler/genDef.h"
#include "chunkStorage.h"

class ExtflashChunkStorage : public ChunkStorage
{
private:
    struct Header
    {
        uint32_t    firstChunk;
        uint32_t    cursor;        // next position to insert data into
        uint16_t    sessionId;
    };

public:
    static const size_t overhead = sizeof( ChunkHeader ) + sizeof( Header );

    void initialize( uint32_t startOffset, uint32_t size );
    void eraseAll();

    #ifdef CHUNKSTORAGE_USE_NOTIFIER
        virtual Sara::Notifier< const ChunkStorage::PostReadNotification > &getPostReadNotifier();
        virtual Sara::Notifier< const ChunkStorage::PostWriteNotification > &getPostWriteNotifier();
        virtual Sara::Notifier< const ChunkStorage::PreDeleteNotification > &getPreDeleteNotifier();
    #elif !defined( CHUNKSTORAGE_USE_SYNC_NOTIFIER )
        virtual void setListenerUserData( const void *userData );
        virtual void setPostReadListener( void (*listenerFunc)( const void *userData, uint32_t address, const ChunkHeader *header ) );
        virtual void setPostWriteListener( void (*listenerFunc)( const void *userData, uint32_t address, const ChunkHeader *header ) );
        virtual void setPreDeleteListener( void (*listenerFunc)( const void *userData, uint32_t address ) );
    #endif

private:
    virtual uint16_t getSessionId();
    virtual void getAddressSpace( uint32_t *startAddress, uint32_t *length );
    virtual bool getFirstAddress( uint32_t *dest );

    virtual bool append( const ChunkHeader *chunkHeader, const void *data );
    virtual bool append( const ChunkHeader *chunkHeader, const void *data, Iterator &iter );
    virtual void flush();

    virtual bool selectFirst( Iterator &iter );
    virtual bool selectNext( Iterator &iter );
    virtual bool selectByAddress( Iterator &iter, uint32_t address );
    virtual uint32_t readData( Iterator &iter, void *dest, uint32_t startOffset, uint32_t len );

    bool write( uint32_t addr, const ChunkHeader *chunkHeader, const void *data );
    bool appendPadding( uint32_t len );
    void onChunkDelete( uint32_t addr );
    bool append( const ChunkHeader *chunkHeader, const void *data, uint32_t *destAddr );
    void headerModified();
    bool isValidChunkAddress( uint32_t addr ) const;

    static void invalidateIfOutsideBoundaries( Iterator *iter, const void *userArg );
    void invalidateIfOutsideBoundaries( Iterator *iter );

    Header header;
    uint32_t ringStart, ringEnd;
    bool headerUpToDate;

    #ifdef CHUNKSTORAGE_USE_NOTIFIER
        Sara::Notifier< const ChunkStorage::PostReadNotification > postReadNotifier;
        Sara::Notifier< const ChunkStorage::PostWriteNotification > postWriteNotifier;
        Sara::Notifier< const ChunkStorage::PreDeleteNotification > preDeleteNotifier;
    #elif !defined( CHUNKSTORAGE_USE_SYNC_NOTIFIER )
        const void *listenerUserData;
        void (*onPostRead)( const void *userData, uint32_t address, const ChunkHeader *header );
        void (*onPostWrite)( const void *userData, uint32_t address, const ChunkHeader *header );
        void (*onPreDelete)( const void *userData, uint32_t address );
    #endif
};

#endif
