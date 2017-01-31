#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

******************************************************************************/
/**
@file LogbookDB.hpp
*/
#include <cstdint>
#include <oswrapper/mutex.h>
#include "common/text/Text.hpp"
#include "filesystems/file.h"
#include "LogbookEntryDb.hpp"
#include <whiteboard/ResourceClient.h>

class Logbook;

static const LogbookEntryDb::LogId INVALID_ENTRY_ID = static_cast<LogbookEntryDb::LogId>(-1);

/** Class to handle Logbook file operations. */
class LogbookDb
{
public:
    typedef LogbookEntryDb::LogId LogId;
    static const uint16_t DIR_NAME_LEN = 32;
    typedef nea::Text<DIR_NAME_LEN> Filename;

    /** Constructor */
    LogbookDb();

    /** Destructor */
    ~LogbookDb();

    /**
    *   Open new logfile if no log file is open.
    *   @return true if successful.
    */
//    bool open();

    /**
    *   Close open log files
    */
    void close() {}

    /**
    *   Are log files open.
    *   @return true if log files are open.
    */
    bool isOpen() const {return false;}

    /**
    *   Creates 'logs' directory, initializes LogbookEntryDb
    */
    void init();

    /**
    *   Function for flushing data before poweroff
    */
//    void flushAll();

    /*
     * Get uploaded amount of log values data
     */
    uint32 getValueSync(LogId logId);

    /*
     *  Get all log ids.
     */
    uint16_t getLogIds(LogId* idArray /*out*/, uint16_t arraySize, bool fromEnd = true);

    /*
     *  Search n-th log. 0 is oldest
     *  return true if found
     */
    bool getLogIdIndex(LogId &logId, uint32 index);

    /*
     *  Convert negative indexes to logbook entry indexes and check index validity against number of logs.
     *  @param index index value to be checked and converter
     *  @return true on success and false if index is out of arange.
     */
    bool convertLogIndex(int32 &index);

    /**
    *   Function to get open logfile name.
    *
    *   @param summaryFile Set true for summary file name and false for value filename.
    */
   // static void getLogFilename(nea::BText& name, uint32 logId, bool summaryFile);

    uint32 entryCount() const { return mLogbookEntryDb.entryCount(); }
 
  /**
    *   return last log create/delete time.
    */
    uint32 last() const { return mLastLogId; }

    /**
    *   return logging state
    */
    bool isLogFull() const { return mLogFull; }

    LogbookEntryDb& getEntryDb() { return mLogbookEntryDb; }

    /**
     * Set summary synchronization amount for a log
     */
    void setSummarySync(LogbookEntryDb::Entry& entry, uint32 syncAmount);
    /**
     * Set values synchronization amount for a log
     */
    void setValueSync(LogbookEntryDb::Entry& entry, uint32 syncAmount);

    /**
     * @brief Logbook uploader can set log to be busy.
     */
     void setSyncingLog(LogbookEntryDb::LogId syncingLogId) { mSyncingLogId = syncingLogId; }

    /**
     * @return amount of unsynced log
     */
    uint16_t getUnsynchronised();

private:

    friend class LogbookEntryDb;

    /**
    *   Creates new directory for logging and opens log files.
    */
//    void initiateLogDirectory();

    /**
    *   Opens log file and writes SBEM header at the beginning of log file.
    *
    *   @param filename Log file name to open.
    *   @return Returns filehandle to open log file.
    */
//    FileHandle initiateLog(const nea::BText& filename);

    /**
    *   Function to add new entro to log file list.
    *   @param entry New log entry name.
    */
//    void addEntry(const nea::BText& entry);

//    friend class LogbookDbTest; // for unit testing
//    friend class LogbookUploadTest;

    /**
    *   Delete certain log
    *   @param entry is deleted log.
    *   @return true when success
    */
    bool deleteLog(LogbookEntryDb::Entry &entry);

    /**
    *   Delete oldest not important log. Does not delete open log
    *   @return true when success
    */
    bool deleteOldest();

    /**
    *   Filter directory tree scanning
    *   @param  data is length of stream written to log or new filter reload value
    *   @param  reload tell to set filter limit
    *   @return true if no need for checking diretory tree size
    */
//    bool shouldCheckSpace(uint32 data, bool reload = false);

    /**
    *   Calculate free space for logging
    *   @return free space
    */
    uint32 getFreeSpace();

    /**
    *   Does filesystem have space for logging
    *   @param  size is written data amount
    *   @param  summaryFile is data sammary or value data. There is some extra left for lap summaries
    *   @return true if is enough space for data
    */
//    bool isSpaceEnough(uint16_t size, bool summaryfile);

    /**
    *   Remove log directories which are not in etries database
    */
//    void cleanUnknownDirs() const;

    /**
    *   Check that app data duration is != 0. If duration is 0 then delete log
    */
    void cleanUnfinishedLogs();

    /**
    *   set logging full state
    *   @param  full is true if no space for value items
    *   @param  critical is true if no space for lap data
    */
    void setLogFull(bool full, bool critical = false);

    LogbookEntryDb mLogbookEntryDb;
    uint32 mLogId;
    uint32 mLastLogId;
    MutexHandle mMutex;

    int32 mSizeCheckfilter;
    bool  mLogFull;
    bool  mLogFullCritical;
    LogbookEntryDb::LogId mSyncingLogId;
    uint32 mBytesWritten;
    uint32 mInitialLogbookSize;

};
