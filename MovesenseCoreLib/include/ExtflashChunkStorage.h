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

#include "common/compiler/genDef.h"
#include "chunkStorage.h"

// Header area is the first 256 bytes. The storage is a rolling storage that starts with all 0xff and header is then stored to
// offset "0".
// then header offset is incremented until all header space is taken. Then starts from 0 again
#define STORAGE_HEADER_SIZE 256
#define TWO_CC 0xEFC5
class ExtflashChunkStorage : public ChunkStorage
{
private:
    PACK_BEGIN()
    struct StorageHeader
    {
        uint16_t two_cc; // 0xEFC5;
        uint32_t firstChunk;
        uint32_t cursor; // next position to insert data into
        uint16_t sessionId;
    } PACKED;
    PACK_END()
    int currentHeaderOffset;

public:
    void initialize(uint32_t startOffset, uint32_t size);

#ifdef CHUNKSTORAGE_USE_NOTIFIER
    virtual Sara::Notifier<const ChunkStorage::PostReadNotification>& getPostReadNotifier();
    virtual Sara::Notifier<const ChunkStorage::PostWriteNotification>& getPostWriteNotifier();
    virtual Sara::Notifier<const ChunkStorage::PreDeleteNotification>& getPreDeleteNotifier();
#elif !defined(CHUNKSTORAGE_USE_SYNC_NOTIFIER)
    virtual void setListenerUserData(const void* userData);
    virtual void setPostReadListener(void (*listenerFunc)(const void* userData, uint32_t address, const ChunkHeader* header));
    virtual void setPostWriteListener(void (*listenerFunc)(const void* userData, uint32_t address, const ChunkHeader* header));
    virtual void setPreDeleteListener(void (*listenerFunc)(const void* userData, uint32_t address));
#endif

    virtual uint16_t getSessionId();
    virtual uint16_t incrementSessionId();

    virtual void getAddressSpace(uint32_t* startAddress, uint32_t* length);
    virtual bool getFirstAddress(uint32_t* dest);

    virtual bool append(const ChunkHeader* chunkHeader, const void* data);
    virtual bool append(const ChunkHeader* chunkHeader, const void* data, Iterator& iter);
    virtual void flush();
    virtual bool eraseAll();

    virtual bool selectFirst(Iterator& iter);
    virtual bool selectNext(Iterator& iter);
    virtual bool selectByAddress(Iterator& iter, uint32_t address);
    virtual uint32_t readData(Iterator& iter, void* dest, uint32_t startOffset, uint32_t len);

    int32_t getFreeSpace() const;

private:
    bool write(uint32_t addr, const ChunkHeader* chunkHeader, const void* data);
    bool appendPadding(uint32_t len);
    void onChunkDelete(uint32_t addr);
    bool append(const ChunkHeader* chunkHeader, const void* data, uint32_t* destAddr);
    void headerModified();
    bool isValidChunkAddress(uint32_t addr) const;
    bool eraseStorageHeader();

    bool readStorageHeader(StorageHeader& header);
    bool writeStorageHeader(const StorageHeader& header);

    static void invalidateIfOutsideBoundaries(Iterator* iter, const void* userArg);
    void invalidateIfOutsideBoundaries(Iterator* iter);

    StorageHeader header;

    uint32_t mStartAddress;
    uint32_t mStorageSize;

    inline uint32_t dataAreaStart() const { return mStartAddress + STORAGE_HEADER_SIZE; }
    inline uint32_t dataAreaEnd() const { return mStartAddress + mStorageSize; }

    bool headerUpToDate;

#ifdef CHUNKSTORAGE_USE_NOTIFIER
    Sara::Notifier<const ChunkStorage::PostReadNotification> postReadNotifier;
    Sara::Notifier<const ChunkStorage::PostWriteNotification> postWriteNotifier;
    Sara::Notifier<const ChunkStorage::PreDeleteNotification> preDeleteNotifier;
#elif !defined(CHUNKSTORAGE_USE_SYNC_NOTIFIER)
    const void* listenerUserData;
    void (*onPostRead)(const void* userData, uint32_t address, const ChunkHeader* chunkHeader);
    void (*onPostWrite)(const void* userData, uint32_t address, const ChunkHeader* chunkHeader);
    void (*onPreDelete)(const void* userData, uint32_t address);
#endif
};

#endif
