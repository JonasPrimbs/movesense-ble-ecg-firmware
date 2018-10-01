#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

******************************************************************************/
/**
@file Logbook.hpp
*/

#include "whiteboard/ResourceProvider.h"
#include "whiteboard/ResourceClient.h"
#include "whiteboard/LaunchableModule.h"

#include "LogbookDb.hpp"
#include "MeasStorage.h"
#include "ExtflashChunkStorage.h"
#include "common/core/dbgAssert.h"

void getLogbookMemoryArea(uint32_t &offset, uint32_t &size);

#define _LOGBOOK_BYTESTREAM_OVERHEAD_MAX (10)
#define _LOGBOOK_STREAM_PART_SIZE (256)

/** Class to provide log file names to loggers. */
class Logbook : 
    private whiteboard::ResourceClient,
    private whiteboard::ResourceProvider,
    public  whiteboard::LaunchableModule
{
    
public:
    static const char* const LAUNCHABLE_NAME;
   
    Logbook(LogbookDb &logbookDb, ExtflashChunkStorage &chunkStorage, MeasStorage &measStorage);
    virtual ~Logbook();
    
private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }

private:

    using ResourceProvider::onTimer;
    using ResourceProvider::startTimer;
    using ResourceProvider::stopTimer;

    /** @see whiteboard::ResourceProvider::onGetRequest */
    virtual void onGetRequest(const wb::Request& request,
                              const wb::ParameterList& parameterList) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onPutRequest */
    virtual void onPutRequest(const wb::Request& request,
                              const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onPostRequest */
    virtual void onPostRequest(const wb::Request& request,
                               const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onSubscribe */
    virtual void onSubscribe(const wb::Request& request,
                             const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onUnsubscribe */
    virtual void onUnsubscribe(const wb::Request& request,
                               const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onDeleteRequest */
    virtual void onDeleteRequest(const whiteboard::Request& rRequest,
                         const whiteboard::ParameterList& parameters) OVERRIDE;

    void onSubscribeResult(wb::RequestId requestId, wb::ResourceId resourceId,
                     wb::Result resultCode, const wb::Value& resultData) OVERRIDE { DEBUG_ASSERT(resultCode == wb::HTTP_CODE_OK); }


    void onTimer(wb::TimerId timerId) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onRemoteWhiteboardDisconnected */
    virtual void onRemoteWhiteboardDisconnected(whiteboard::WhiteboardId whiteboardId) OVERRIDE;
private:

    ExtflashChunkStorage &mChunkStorage;
    MeasStorage &mMeasStorage;


    friend class LogbookTest; // Used for testing
    /**
    *   Close open, but unused logbookDb and open logbookDb when it is in use.
    */
    void updateLogbookState();
    bool eraseFilesystem();

    void handleGetDescriptors(const wb::Request& request, const wb::ParameterList& parameterList);
    void handleGetData(const wb::Request& request, const wb::ParameterList& parameterList);

    LogbookDb &mLogbookDb;

    uint16 mIsLogging;
    bool mLogbookFull;
    wb::TimerId mTimer;
    bool mShutdown;

    void resetGetLogDataState();
    void resetGetLogDescriptorsState();
    uint32_t startNewLog();

    uint16_t mCurrentDescriptorItemIndex;
    uint16_t mCurrentDescriptorGroupIndex;
    uint32_t mLastDescriptorReadPos;

    uint32_t mCurrentDataChunkAddress;
    uint32_t mCurrentDataChunkOffset;
    uint32_t mCurrentDataLength;
    uint32_t mCurrentDataRead;
    uint32_t mLastDataReadPos;

    inline size_t freeBytesInBuffer(size_t currentPosInStm) const {return _LOGBOOK_STREAM_PART_SIZE - currentPosInStm;}
    inline uint32_t dataBytesLeftToRead() const {return mCurrentDataLength - mCurrentDataRead;}
    bool selectNextChunkInSession(uint16_t sessionId, ChunkStorage::Iterator &iter);

    uint16_t mDescriptorizedResourceIds[16];

    bool isResourceDescriptorized(wb::LocalResourceId resourceId, uint16_t & nextFreeIndex) const;
};
