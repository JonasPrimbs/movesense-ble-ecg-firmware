#ifndef MEAS_STORAGE_H_
#define MEAS_STORAGE_H_
/******************************************************************************

    Copyright (c) Suunto Oy 2014.
    All rights reserved.

    $Id: measStorage.h 69704 2014-10-22 15:25:45Z treyh $

******************************************************************************/
/**
    @file measStorage.h
    Measurement storage.

    @todo
    Recording masking should be implemented
*/

#include "common/core/debug.h"
#include "common/compiler/genDef.h"
#include "common/compiler/pack.h"

#include "chunkStorage.h"
#include <map>
#include <vector>
#include <stdlib.h>

#include "whiteboard/Identifiers.h"
#include "whiteboard/Value.h"

enum MeasurementType
{
    MT_NONE,
    MT_RR,
    MT_BATTERY_LEVEL,
    MT_ACCEL,
    MT_MAGN,
    COUNT_MeasurementType
};

#define MT_SHIFT    5
#define MT_MASK        (7<<MT_SHIFT)
STATIC_VERIFY( COUNT_MeasurementType <= 8, COUNT_MeasurementType_error );

// custom chunk types
enum ChunkType
{
    CT_DEBUG_DATA        = ChunkStorage::CT_CUSTOM_BASE + 0,
    CT_WB_MEASUREMENT_BASE = ChunkStorage::CT_CUSTOM_BASE + 1
};

enum ChunkFlag
{
    CH_FLAG_NONE = 0
};

// define chunk sizes
#ifndef MAX_WB_CHUNK_SIZE
    #define MAX_WB_CHUNK_SIZE                128
#endif

#define SERIALIZATION_BUFFER_LENGTH 96

class MeasStorage
#ifdef CHUNKSTORAGE_USE_NOTIFIER
    : public Sara::Listener< const ChunkStorage::PostReadNotification >
    , public Sara::Listener< const ChunkStorage::PostWriteNotification >
    , public Sara::Listener< const ChunkStorage::PreDeleteNotification >
#endif
{
    const static uint32_t invalidTime = 0xFFFFFFFF;

public:
    typedef uint32_t (&GetTimeFunc)();

    #ifdef CHUNKSTORAGE_USE_NOTIFIER
        void notify( const ChunkStorage::PostReadNotification &ev );
        void notify( const ChunkStorage::PostWriteNotification &ev );
        void notify( const ChunkStorage::PreDeleteNotification &ev );
    #endif


    /**
        Creator.
        
        @param chunkStorage_    The storage object
        @param getTimeFunc        Function that returns current time (ms resolution).
    */
    MeasStorage( ChunkStorage &chunkStorage_, GetTimeFunc getTimeFunc );
    virtual ~MeasStorage() {}

    /**
        Initialize storage object. Should be called after creating object before calling any other member function.
    */
    void initialize();

    /**
        Set which kind of data will be recorded.

        @param mask            Recording mask value
    */
    void setRecording( uint32_t mask );

    /**
        Get current recording mask.

        @return mask value
    */
    uint32_t getRecording();

    /**
        Appends any WB measurement value to storage.

        @param timestamp_ms    Timestamp in ms
        @param measurement        WB measurement to store
        @note
        Appends Whiteboard measurement struct to storage.
    */
    void appendWBMeasurement( uint32_t timestamp_ms, whiteboard::ResourceId resourceId, const whiteboard::Value &measurement );

    /**
        Flushes cached measurement-values. 
    */
    void flushMeasurements(uint32_t measurementTypeID);

    /**
        If no RR-chunks has yet been saved, discards the buffer measurement.
    */
    void discardOrphanMeasurements();

    /**
        Get Measurement statistics.

        @param startTime_ms    destination for Measurement data starting time
        @param duration_ms    destination for Measurement duration
        @return                false if no Measurement data at all
    */
    bool getMeasStats( uint32 *startTime_ms, uint32 *duration_ms );

    /**
        Appends debug data to storage.

        @param data            points to the data
        @param len            data size (bytes)
    */
    void appendDebugData( const void *data, uint32_t len );

    /**
        Reads WB measurement values from storage.

        @param sessionId
                        Session to request values from
        @param reqStartTime_ms
                        Request values starting from this timestamp (ms)
        @param dest        Destination buffer for RR-values
        @param maxCount    Maximum count of values to read
        @param maxDuration_ms
                        Maximum duration of returned RR-value set (ms)
        @param startTime_ms
                        Destination of RR-value set starting time (ms)
        @param duration_ms
                        Destionation of RR-value set duration (ms)
        @return            Count of RR-values read

        @note
        Example:
            // save values in storage after creating it
            storage.appendRR( 10, 2 );
            storage.appendRR( 12, 2 );
            storage.appendRR( 14, 2 );
            storage.appendRR( 20, 2 );
            storage.appendRR( 22, 2 );
            storage.appendRR( 24, 2 );
            // now storage contains heart beat times: 10, 12, 14, 16, 20, 22, 24, 26
            uint32_t rrSetStartTime, rrSetDuration, rrSetCount;
            uint16_t destinationBuffer[100];
            rrSetCount = storage.readRR( storage.getSessionId(), 5, destinationBuffer,
                ELEMENTS(destinationBuffer), 1000, &rrSetStartTime, &rrSetDuration );
            // now: rrSetCount=7, dest={2,2,2,4,2,2,2}, rrSetStartTime=10, rrSetDuration=16
    */
    template <typename WBMeasType> uint32_t readMeasurements( uint16_t sessionId, uint32_t reqStartTime_ms, 
    std::map<whiteboard::LocalDataTypeId, std::vector<WBMeasType> > &measurementsOut,
    uint32_t maxDuration_ms, uint32_t *startTime_ms, uint32_t *duration_ms);

    /**
        Flushes all cached data. Usually executed before system shutdown.
    */
    void flush();

    /**
        Returns current session id.

        @return Session id.
    */
    uint16_t getSessionId();

    whiteboard::LocalResourceId getChunkWBResource(const ChunkStorage::Iterator iter) const;
    
protected:

private:
    #ifdef CHUNKSTORAGE_USE_SYNC_NOTIFIER
        struct ExtendedListener
        {
            ChunkStorage::Listener listener;
            MeasStorage *This;
        } extendedListener;
        static void onChunkStorageEvent( const ChunkStorage::Listener *storage, const ChunkStorage::Event &event );
    #elif !defined( CHUNKSTORAGE_USE_NOTIFIER )
        static void onPostRead( const void *userData, uint32_t address, const ChunkHeader *header );
        static void onPostWrite( const void *userData, uint32_t address, const ChunkHeader *header );
        static void onPreDelete( const void *userData, uint32_t address );
    #endif
    void cleanUp();

    ChunkStorage &chunkStorage;
    GetTimeFunc getTime;
    uint32_t recording;
    uint32_t measStart, measEnd;

    template <int size>
    struct MeasBuffer
    {
        ChunkHeader header;
        union
        {
            uint8_t data[size];
            uint16_t data_uint16[(size+1)/2];
        };
        uint32_t endTime_ms;
    };

    ChunkStorage::Iterator iter, lastAppendedMeas;
    
    typedef MeasBuffer<MAX_WB_CHUNK_SIZE-sizeof(ChunkHeader)> MeasurementBuffer;
    
    const static int MAX_BUFFER_COUNT=16;
    MeasurementBuffer measBuffers[MAX_BUFFER_COUNT];
    uint16_t measBufferIds[MAX_BUFFER_COUNT];

    MeasurementBuffer &getMeasBuffer(uint16_t measBufferId);
    int findMeasBuffer(uint16_t bufferId) const;

    // rrChunkTbl keeps track of any single rr chunk located in each slice of ring buffer.
    // Using these "bookmarks" we can faster locate the rr chunk we are looking for.
    static const uint32_t wbChunkSlices = 16;
    uint32_t chunkSpaceStart, chunkSpaceLength;
    uint16_t wbChunkTbl[wbChunkSlices];
    void updateWBChunkTbl( uint32_t address, const ChunkHeader *header, bool write );
    uint32_t inline getWBChunkSliceAddress( uint32_t sliceIndex )
    {
        return (wbChunkTbl[sliceIndex]&0x7FFF) + chunkSpaceStart + sliceIndex * chunkSpaceLength / wbChunkSlices;
    }
    
    
};

#endif
