#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/Result.h"

// We are using CRC checks always
#define WB_SERIAL_FRAME_WITH_CRC

/**
   Serial communication protocol.

   Frame format:
     StartMark = 0x7e

        Payload bytes as they are, except 0x7e and 0x7d are
        converted to 0x7d followed by [byte xor 0x20].

        Example:
            0x7e converted to = 0x7d 0x5e
            0x7d converted to = 0x7d 0x5d

        So character 0x7e will never be found in the frame.

     EndMark = 0x7e
*/

namespace whiteboard {

class WB_API Protocol
{
public:
    Protocol() : mpNext(NULL) {}
    virtual ~Protocol() {}

    void setNext(Protocol* pProtocol) { mpNext = pProtocol; }
    Protocol* getNext() const { return mpNext; }

    virtual void reset() {}
    virtual Result process(const void* pBuffer, uint32 size) = 0;

private:
    Protocol* mpNext;
};

class WB_API SerialFrameReceiver : public Protocol
{
public:
    SerialFrameReceiver(uint8* pBuf, uint32 size)
        : mpBuffer(pBuf), mBufferSize(size), mPos(0), mState(STATE_RESYNC)
    {
    }

    void reset() OVERRIDE;
    Result process(const void* pBuffer, uint32 size) OVERRIDE;
    
private:
    Result processByte(uint8 data);

    enum State_e
    {
        STATE_START,
        STATE_NORMAL,
        STATE_CONTROL,
        STATE_RESYNC
    };

    void logError(const char* pMsg);

    uint8* mpBuffer;
    const uint32 mBufferSize;
    uint32 mPos;
    State_e mState;
};

class WB_API SerialFrameSender : public Protocol
{
public:
    SerialFrameSender(uint8* pBuf, uint32 size) 
        : mpBuffer(pBuf), mBufferSize(size) {}

    void reset() OVERRIDE;
    Result process(const void* pBuffer, uint32 size) OVERRIDE;

private:
    struct EncState_t
    {
        uint8* dstBuf;
        uint32 dstPos;
        const uint8* srcBuf;
        uint32 srcCnt;
        bool control;
    };

    Result bufferProcess(EncState_t& es);
    void bufferEncode(EncState_t& es);
    Result bufferSend(EncState_t& es);

    uint8* mpBuffer;
    const uint32 mBufferSize;
};

template <uint32 SZ> 
class WB_API SerialRecv : public SerialFrameReceiver
{
public:
    SerialRecv() : SerialFrameReceiver(mBuffer, SZ) {}

private:
    uint8 mBuffer[SZ];
};

template <uint32 SZ> 
class WB_API SerialSend : public SerialFrameSender
{
public:
    SerialSend() : SerialFrameSender(mBuffer, SZ) {}

private:
    uint8 mBuffer[SZ];
};

} // namespace whiteboard
