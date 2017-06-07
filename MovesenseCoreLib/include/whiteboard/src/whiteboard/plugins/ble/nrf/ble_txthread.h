// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "SerialFrame.hpp"
#include "TempLinkedList.h"
#include "BleApi.h"

namespace wbble
{
    struct BleCallbacks_t {
        void (*onSendCompleted)(const void* pData, bool success);
        void (*onDataReceived)(uint32 deviceId, const void* pData, uint32 size);
        void (*onConnected)(uint32 deviceId, bool success);
        void (*onDisconnected)(uint32 deviceId, bool success);
    };
    
class BleTxThread FINAL : public Protocol
{
public:
    BleTxThread(const BleCallbacks_t* pIfCb, SerialFrameSender* pSfSender);
    ~BleTxThread();

    SendResult_e send(uint32 deviceId, const void* pData, uint32 size);
    bool cancelSend(const void* pData);

    void reset();
    bool process(const void* pBuffer, uint32 size);

private:
    struct Item FINAL
    {
        const void* mpData;
        uint32 mSize;
        uint8 mChannel;
    };

    void txHandler();
    static void txThread(void* pUserData);

    TempLinkedList<Item> mList;
    WbThreadHandle mThread;
    WbSemaphoreHandle mListLockSema;
    WbSemaphoreHandle mListKickSema;
    const BleCallbacks_t* mpIfCb;
    SerialFrameSender* mpSfSender;
};

}