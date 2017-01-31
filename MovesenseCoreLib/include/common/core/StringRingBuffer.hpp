// Copyright (c) Suunto Oy 2016. All rights reserved.
#pragma once

namespace nea
{

/** Simple ring buffer for storing strings. */
class StringRingBuffer
{
public:
    /**
    Construct a string ring buffer using the given buffer memory. The given input buffer is zeroed.
    @param pBuffer Pointer to the data buffer to be used.
    @param bufferSize Byte size of the data buffer.
    @param maxStrLen Max length of an individual string (longer will be just cut).
    */
    StringRingBuffer(char* pBuffer, size_t bufferSize, size_t maxStrLen);

    /**
    Destructor.
    */
    ~StringRingBuffer();

    /**
    Write new string into the ring buffer. Not thread safe, if called from multiple
    thread contexts the caller must use e.g. a mutex.
    @param str Pointer to the string to be added.
    */
    void write(const char* str);

    /**
    Try to read forward from the given string position.
    Copies string to the destination buffer if a following string is found.
    @param current Pointer to a string within the ring buffer (NULL starts from the oldest write).
    @param destBuffer Reference to the buffer pointer in which to write.
    @param bytesRead Reference to which the amount of read bytes is written.
    @param limiter String length limiter, defaults to 0 which indicates maxStrLen from construction is used.
    @return Pointer to the string if found, NULL if no string found.
    */
    const char* findNextEntry(const char* current, char* destBuffer, size_t& bytesRead, size_t limiter = 0);

private:
    size_t mBufferSize;
    size_t mWriteHead;
    size_t mMaxStrLen;
    char* mpBuffer;
    bool mbWrapped;
};

} // namespace nea
