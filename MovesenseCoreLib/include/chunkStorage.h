#ifndef CHUNK_STORAGE_H_
#define CHUNK_STORAGE_H_
/******************************************************************************

    Copyright (c) Suunto Oy 2014.
    All rights reserved.

    $Id: chunkStorage.h 69642 2014-10-22 07:36:46Z treyh $

******************************************************************************/
/**
    @file chunkStorage.h
    Interface class for a chunk storage.

    A chunk storage is a iterable ring buffer for variable length chunks.
    Oldest chunks are destroyed when overwritten by new chunks.

    @note
    Example for browsing through ChunkStorage chunks:
    
        ChunkStorage::Iterator iter;
        chunkStorage.selectFirst( iter );
        while( iter.isValid() )
        {
            uint32_t theTimeStamp = iter.timeStamp;
            uint8_t theData[10];
            chunkStorage.readData( iter, theData, 0, 10 );
            chunkStorage.selectNext( iter );
        }
*/

#include <stdint.h>
#include <stdbool.h>

#include "common/compiler/genDef.h"
#include "common/compiler/pack.h"


/**
    Header of a chunk
*/
struct ChunkHeader
{
    uint16_t   chunkType;       // 8 -> 16 so that WB ID's fit!
    uint8_t   chunkLen;        // includes header. 8 bit enough since max size 128 bytes
    uint8_t   flags;           
    uint32_t   timeStamp;
    uint32_t   duration;
    uint16_t   sessionId;
    uint8_t    headerChecksum;    // ChunkStorage calculates and checks
    uint8_t    dataChecksum;    // -"-
} PACKED;

#ifndef MAX_CHUNK_SIZE
    #define MAX_CHUNK_SIZE                    128
#endif
STATIC_VERIFY( MAX_CHUNK_SIZE >= sizeof(ChunkHeader) + sizeof( uint16_t ), MAX_CHUNK_SIZE_error );

#ifdef CHUNKSTORAGE_USE_SYNC_NOTIFIER
    #include "syncNotifier.h"
    struct ChunkStorageEvent
    {
        enum
        {
            POST_READ, POST_WRITE, PRE_DELETE
        } type;
        union
        {
            struct PostRead
            {
                uint32_t address;
                const ChunkHeader *header;
            } postRead;
            struct PostWrite
            {
                uint32_t address;
                const ChunkHeader *header;
            } postWrite;
            struct PreDelete
            {
                uint32_t address;
            } preDelete;
        } data;
    };
    class ChunkStorage : public SyncNotifier< const ChunkStorageEvent & >
#else
    class ChunkStorage
#endif
{
public:
    #ifdef CHUNKSTORAGE_USE_SYNC_NOTIFIER
        typedef ChunkStorageEvent Event;
        typedef SyncNotifier< const Event & >::Listener Listener;
    #endif

    /**
        Chunk types handled by ChunkStorage
    */
    enum ChunkType
    {
        CT_NONE,            // single byte padding
        CT_PADDING,            // padding (length 3..65535 bytes)
        CT_STORAGE_HEADER,    // permanent for storage status and settings etc. always @ 0x0000
        CT_CUSTOM_BASE        // base value for custom chunk types
    };

    /**
        Object used to iterate through storage chunks.

        @note
        If the chunk that the iterator points at is destroyed (overwritten),
        the iterator gets invalidated.
    */
    class Iterator
    {
    public:
        static void invalidate( uint32_t addr );
        static void invalidateAll();
        static void visitAll( void (*callBack)( Iterator *iter, const void *userArg ), const void *userArg );

        Iterator();
        ~Iterator();
        void invalidate();
        bool isValid() const;
        void set( Iterator &iter );

        ChunkHeader header;
        uint32_t address;

    private:
        Iterator *prev, *next;
        static Iterator *first;
    };

    /**
        Returns current session id.

        @return Session id.
    */
    virtual uint16_t getSessionId() = 0;

    /**
        increments current session id by 1.
        @return new Session id.
    */
    virtual uint16_t incrementSessionId() = 0;

    /**
        Appends a chunk to storage ring buffer.

        @param chunkHeader    Points to chunk header. Checksum values are updated
            during function execution.
        @param data    Points to data. Value not used if chunk does not have any
            data ( chunkHeader->chunkLen equals sizeof(ChunkHeader) ).
        @return true if succeeded
    */
    virtual bool append( const ChunkHeader *chunkHeader, const void *data ) = 0;

    /**
        Appends a chunk to storage ring buffer.

        @param chunkHeader    Points to chunk header. Checksum values are updated
            during function execution.
        @param data    Points to data. Value not used if chunk does not have any
            data ( chunkHeader->chunkLen equals sizeof(ChunkHeader) )
        @param iter    After successful execution, points to the appended item.
        @return true if succeeded
    */
    virtual bool append( const ChunkHeader *chunkHeader, const void *data, Iterator &iter ) = 0;

    /**
        Flushes cached data. Usually executed before system shutdown.
    */
    virtual void flush() = 0;

    /**
        Updates iterator to point at first item in storage.

        @param iter    Iterator object to update. If function fails,
            iterator will be invalidated.
        @return true if succeeded
    */
    virtual bool selectFirst( Iterator &iter ) = 0;

    /**
        Updates iterator to point at next item in storage.

        @param iter    Iterator object to update. If function fails,
            iterator will be invalidated.
        @return true if succeeded
    */
    virtual bool selectNext( Iterator &iter ) = 0;

    /**
        Updates iterator to point at item in given address of storage.

        @param iter    Iterator object to update. If function fails,
            iterator will be invalidated.
        @param address    Address of the item.
        @return true if succeeded
    */
    virtual bool selectByAddress( Iterator &iter, uint32_t address ) = 0;

    /**
        Read chunk data.

        @param iter    Iterator object pointing to the chunk.
        @param dest    Points to destination buffer.
        @param startOffset    Start offset of chunk data to start reading at.
            ( use value 0 to read from start of the data )
        @param len    Length of data to read.
        @return Number of bytes read.
    */
    virtual uint32_t readData( Iterator &iter, void *dest, uint32_t startOffset, uint32_t len ) = 0;

    #ifdef CHUNKSTORAGE_USE_NOTIFIER
        struct PostReadNotification
        {
            uint32_t address;
            const ChunkHeader *header;
        };
        virtual Sara::Notifier< const PostReadNotification > &getPostReadNotifier() = 0;
        struct PostWriteNotification
        {
            uint32_t address;
            const ChunkHeader *header;
        };
        virtual Sara::Notifier< const PostWriteNotification > &getPostWriteNotifier() = 0;
        struct PreDeleteNotification
        {
            uint32_t address;
        };
        virtual Sara::Notifier< const PreDeleteNotification > &getPreDeleteNotifier() = 0;
    #elif !defined( CHUNKSTORAGE_USE_SYNC_NOTIFIER )
        /**
            Set user data for listener functions.

            @param userData    Data pointer to be forwarded to listening callback functions.
        */
        virtual void setListenerUserData( const void *userData ) = 0;

        /**
            Set listener functions.

            @param listenerFunc    Listener function callback.
        */
        virtual void setPostReadListener( void (*listenerFunc)( const void *userData, uint32_t address, const ChunkHeader *header ) ) = 0;
        virtual void setPostWriteListener( void (*listenerFunc)( const void *userData, uint32_t address, const ChunkHeader *header ) ) = 0;
        virtual void setPreDeleteListener( void (*listenerFunc)( const void *userData, uint32_t address ) ) = 0;
    #endif

    /**
        Get address space used for chunk addresses.

        @param startAddress    Destination of first address.
        @param length    Destination of address space length.
    */
    virtual void getAddressSpace( uint32_t *startAddress, uint32_t *length ) = 0;

    /**
        Get address of first chunk.

        @param dest    Destination of address.
        @return true if succeeded
    */
    virtual bool getFirstAddress( uint32_t *dest ) = 0;

    virtual ~ChunkStorage() { }
};

#endif
