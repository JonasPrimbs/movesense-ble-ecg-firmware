// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include <whiteboard/ResourceProvider.h>
#include <whiteboard/DpcFunctor.h>

namespace nea
{

class DebugProvider FINAL : private wb::ResourceProvider
{
public:
    DebugProvider();
    virtual ~DebugProvider();

    // called from the launchable module
    bool wbRegister();
    void wbUnregister();

private:
    void onGetRequest(const wb::Request& request, const wb::ParameterList& parameters) OVERRIDE;
    void onPutRequest(const wb::Request& request, const wb::ParameterList& parameters) OVERRIDE;

    void onSubscribe(const wb::Request& request, const wb::ParameterList& parameters) OVERRIDE;
    void onUnsubscribe(const wb::Request& request, const wb::ParameterList& parameters) OVERRIDE;

    void sendUpdate();

public:
    bool allowDebugOutString(const char* outString);

private:
    wb::DpcFunctor mNotificationDpc;

    static const uint16 FILTER_BUF_SIZE = 32;
    static const uint16 MAX_DEBUG_STR_LEN = 128;

    char mOutputFilter[FILTER_BUF_SIZE];
    char mDebugOutBuffer[MAX_DEBUG_STR_LEN];

    // Thread of the provider
    WbThreadHandle mProviderThread;

    // A value indicating whether debug stream is actively listened
    bool mDebugStreamActive;

    // Flag that is used for synchronization
    volatile bool mDebugOutBufferFilled;
#if (DEBUGLOG_TAIL_BORROW_FROM_DUKTAPE_HEAP_AMOUNT != 0)
    wb::ClientId mTailClientId;
    size_t mDebugTailStreamIndex;
    WbTimestamp mLastTailRequest;
    const char* mLastFoundDebugPrint;
#endif
};

} // namespace nea
