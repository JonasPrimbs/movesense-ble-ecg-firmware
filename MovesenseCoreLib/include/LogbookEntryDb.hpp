#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

******************************************************************************/
/**
@file LogbookEntryDB.hpp
*/
#include <cstdint>
#include <cstddef>
typedef uint32_t FilePos;
typedef uint32_t LogEntryHandle;

class LogbookEntryDb
{
public:
    typedef uint32_t LogId;

    struct Entry {
        LogId id;
        FilePos summarySynced;
        FilePos valueSynced;
        FilePos foffset;
        uint16_t   flags;
        uint16_t   magic; // Used to indicate valid entries.
    };

    struct Header {
        uint32_t version;
        uint32_t invalidEntryAmount;
        uint32_t validEntryAmount;
    };

    enum Flags {
        SUMMARY_ALL_SYNCED = 1 << 0,
        VALUES_ALL_SYNCED  = 1 << 1,
        SYNC_CONTENT_ERROR = 1 << 2,
        IMPORTANT_LOG      = 1 << 3,
    };

    LogbookEntryDb();
    /** Destructor */
    virtual ~LogbookEntryDb();

    void init();

    /**
    *	Flush changes to fs. Call before poweroff
    */
    void flush();

    /**
    *	Add new item to db
    *	@return item
    */
    Entry addEntry(LogId logId);

    /**
    *	Get entry by id
    *	@return true when found
    */
    bool getEntry(LogId logId, Entry& entry) const;

    /**
    *	Get entry by index
    *	@return true when found
    */
    bool getEntryByIndex(uint32_t index, Entry& entry) const;

    /**
    *	Get oldest entry
    *	@return true when found
    */
    bool getFirstEntry(Entry& entry) const;

    /**
    *	Get newest entry
    *	@return true when found
    */
    bool getLastEntry(Entry& entry) const;

    /**
    *	Get next entry
    *	@return true when found - entry parameter is not modified if not found
    */
    bool getNextEntry(Entry& entry) const;

    /**
    *	Modify entry
    */
    void modifyEntry(const Entry& entry);

    /**
    *	Delete entry by id
    */
    void delEntry(LogId logId);

    /**
    *	Delete entry
    */
    void delEntry(Entry& entry);


    /**
    *	Return entry amount
    */
    uint32_t entryCount() const { return mHdr.validEntryAmount; }

    /**
     *  Number of entries requiring synchronization
     */
    size_t synchronizableLogCount() const;

    /**
     * Find first unsynchronized entry
     */
    bool firstSynchronizableEntry(Entry& entry) const
    {
        return nextSynchronizableEntry(entry, false);
    }

    /**
     * Find next unsynchronized log entry after parameter.
     * @param previous Entry from which search starts. Updated if entry is found.
     * @return true if unsynchronized entry was found
     */
    bool nextSynchronizableEntry(Entry& entry) const
    {
        return nextSynchronizableEntry(entry, true);
    }

    /**
     * Clear content error flags. This is intended to allow
     * re-uploading logs that have caused a content error at
     * synchronization.
     */
    void clearErrorFlags();

    /**
     * @brief Remove old dead entries from log
     * @param force forces packing. pack is filtered to happen only if enough invalid entrys in db
     */
    void pack(bool force = false);

private:
    friend class LogbookDbTest; // for unit testing

    /**
     * @brief count valid logs in db
     * @return
     */
    uint16_t getCountValid() const;
    /**
     * @brief try to verify that db is valid
     */
    void checkSystemIntegrity();
    LogEntryHandle openEntry() const;
    LogEntryHandle openEntry(const Entry& entry) const;
    LogEntryHandle openEnd() const;
    bool nextEntry(LogEntryHandle f, Entry& entry) const;
    bool nextSynchronizableEntry(Entry& entry, bool isPreviousValid) const;
    LogEntryHandle openHeader() const;
    void getHeader();
    void createHeader(LogEntryHandle f);
    void createHeader();
    void add(Entry& entry);
    void modify(const Entry& entry);

    Header mHdr;
};
