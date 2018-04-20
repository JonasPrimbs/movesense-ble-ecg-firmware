// Copyright (c) Suunto Oy 2018. All rights reserved.
#pragma once
#include "system_debug/resources.h"


class DebugMessageBuffer
{
public:
    virtual bool store(const WB_RES::DebugMessage& msg) = 0;
    virtual bool get(WB_RES::DebugMessage& msg, uint8_t index) = 0;
    virtual void reset() = 0;
    virtual uint8_t getStoredMessagesCount() = 0;
    virtual void getSkippedMessagesInfo(uint8_t& count, uint8_t& level) = 0;

    struct Iterator
    {
        DebugMessageBuffer* buffer;
        uint8_t index;
        Iterator(DebugMessageBuffer* buffer, uint8_t index) : buffer(buffer), index(index) {}

        WB_RES::DebugMessage operator*() const
        {
            WB_RES::DebugMessage msg;
            buffer->get(msg, index);
            return msg;
        }
        void operator++() { index++; }
        bool operator==(Iterator& that) { return this->index == that.index; }
        bool operator!=(Iterator& that) { return !(*this == that); }
    };

    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, getStoredMessagesCount()); }
};


/**
 * @param H max number of debug messages available for storage
 * @param S size of buffer for messages, in bytes
 */
template <size_t H, size_t S>
class StaticDebugMessageBuffer : public DebugMessageBuffer
{
    struct Header
    {
        uint32_t timestamp;
        const char* tag;
        uint8_t length : 7;
        bool trimmed : 1;
        uint8_t level : 4;
    };

    Header headers[H];
    char buffer[S];
    uint8_t storedCount;
    uint8_t skippedCount;
    uint8_t skippedLevel;
    static constexpr uint8_t LEVEL_INVALID = 0xFF;

    uint8_t getNextMessageOffset()
    {
        return getMessageOffset(storedCount);
    }

    uint8_t getMessageOffset(uint8_t msgNum)
    {
        uint8_t index = 0;
        for (int i=0; i < msgNum; ++i)
        {
            index += headers[i].length + 1;
        }
        return index;
    }

    bool isSpaceSufficient(uint8_t headIndex, uint8_t newMsgLen)
    {
        return (headIndex + newMsgLen) < S;
    }

public:

    StaticDebugMessageBuffer() :
        storedCount(0),
        skippedCount(0),
        skippedLevel(LEVEL_INVALID)
    {
    }

    bool store(const WB_RES::DebugMessage& msg) override
    {
        if (storedCount >= H)
        {
            skippedCount++;
            if (msg.level < skippedLevel) skippedLevel = msg.level;
            return false;
        }

        uint8_t len = strlen(msg.message);
        uint8_t head = getNextMessageOffset();

        Header* hdr = &headers[storedCount];
        hdr->timestamp = msg.timestamp;
        hdr->tag = msg.tag;
        hdr->level = msg.level;
        hdr->length = len;

        if (isSpaceSufficient(head, len))
        {
            strcpy(&buffer[head], msg.message);
            hdr->trimmed = 0;
        }
        else
            hdr->trimmed = 1;

        storedCount++;
        return true;
    }

    bool get(WB_RES::DebugMessage& msg, uint8_t index) override
    {
        if (index >= storedCount) return false;

        Header* hdr = &headers[index];
        msg.timestamp = hdr->timestamp;
        msg.tag = hdr->tag;
        msg.level = static_cast<WB_RES::DebugLevelValues::Type>(hdr->level);
        msg.message = !hdr->trimmed
            ? &buffer[getMessageOffset(index)]
            : "...";

        return true;
    }

    void reset() override
    {
        storedCount = 0;
        skippedCount = 0;
        skippedLevel = LEVEL_INVALID;
    }

    uint8_t getStoredMessagesCount() override
    {
        return storedCount;
    }
    void getSkippedMessagesInfo(uint8_t& count, uint8_t& level) override
    {
        count = skippedCount;
        level = skippedLevel;
    }
};

