#pragma once

#include <whiteboard/ResourceClient.h>

typedef uint32_t timestamp_t;


template<class T>
class SeriesBuffer
{
public:
    /**
     * @brief Constructs a new Series Buffer.
     */
    SeriesBuffer();
    /**
     * @brief Constructs a new Series Buffer.
     * 
     * @param bufferLength Number of samples in one buffer.
     * @param bufferCount Number of buffers. Must be >= 2!
     */
    SeriesBuffer(size_t bufferLength, size_t bufferCount);
    /**
     * @brief Destructs the Series Buffer.
     */
    ~SeriesBuffer();

    /**
     * @brief Gets the size of a buffer in bytes.
     * 
     * @return size_t Size of a single buffer in bytes.
     */
    size_t getSingleBufferSize();
    /**
     * @brief Gets a pointer to the buffer at a given index i.
     * 
     * @param i Index of buffer to get.
     * @return uint8_t* Pointer to the start of the buffer of index i.
     */
    uint8_t* getBuffer(size_t i);
    /**
     * @brief Gets a pointer to the current buffer.
     * 
     * @return uint8_t* Pointer to the start of the current buffer.
     */
    uint8_t* getCurrentBuffer();
    /**
     * @brief Reset the current buffer.
     */
    void resetCurrentBuffer();
    /**
     * @brief Switches to the next buffer and starts filling out the samples at the beginning.
     */
    void switchBuffer();
    /**
     * @brief Gets whether a new sample can be added to the current buffer.
     * 
     * @return true Buffer is not full. New samples can be added.
     * @return false Buffer is full. New samples cannot be added.
     */
    bool canAddSample();
    /**
     * @brief Adds a new sample to the current buffer if not full.
     * 
     * @param sample Sample to add.
     * @return true New sample was added successfully.
     * @return false New sample was not added since current buffer ist full. Call switchBuffer() to switch to the next buffer and call this method again.
     */
    bool addSample(T sample);
    /**
     * @brief Sets the timestamp to the current sample.
     * 
     * @param timestamp Timestamp to write to the current sample.
     */
    void setTimestamp(timestamp_t t);
    /**
     * @brief Sets the length of one buffer. This will clear all buffers!
     * 
     * @param length New number of samples in one buffer.
     */
    void setLength(size_t len);
    /**
     * @brief Gets the number of samples in one buffer.
     */
    size_t getLength();

private:
    /** The actual buffer. */
    uint8_t* buffer;
    /** The number of samples in one buffer. */
    size_t length;
    /** The number of buffers. Must be >= 2! */
    size_t count;
    /** The index of the current buffer. */
    size_t currentIndex;
    /** The number of buffered samples in the current buffer. */
    size_t bufferedSamples;
};


template<class T>
SeriesBuffer<T>::SeriesBuffer() :
    length(16),
    count(2),
    currentIndex(0),
    bufferedSamples(0)
{
    auto size = this->getSingleBufferSize();
    this->buffer = new uint8_t[size * this->count] { 0 };
}

template <class T>
SeriesBuffer<T>::SeriesBuffer(size_t bufferLength, size_t bufferCount) :
    length(bufferLength),
    count(bufferCount),
    currentIndex(0),
    bufferedSamples(0)
{
    auto size = this->getSingleBufferSize();
    this->buffer = new uint8_t[size * this->count] { 0 };
}

template <class T>
SeriesBuffer<T>::~SeriesBuffer()
{
    delete[] this->buffer;
}

template <class T>
size_t SeriesBuffer<T>::getSingleBufferSize()
{
    return sizeof(timestamp_t) + (this->length * sizeof(T));
}

template <class T>
uint8_t* SeriesBuffer<T>::getBuffer(size_t i)
{
    size_t size = this->getSingleBufferSize();
    return &this->buffer[size * i];
}

template <class T>
uint8_t* SeriesBuffer<T>::getCurrentBuffer()
{
    return this->getBuffer(this->currentIndex);
}

template <class T>
void SeriesBuffer<T>::resetCurrentBuffer()
{
    this->bufferedSamples = 0;
}

template <class T>
void SeriesBuffer<T>::switchBuffer()
{
    this->currentIndex = (this->currentIndex + 1) % this->count;
    this->resetCurrentBuffer();
}

template <class T>
bool SeriesBuffer<T>::canAddSample()
{
    return (this->bufferedSamples + 1) <= this->length;
}

template <class T>
bool SeriesBuffer<T>::addSample(T sample)
{
    // Verify that new sample still fits into current buffer.
    if (!this->canAddSample())
    {
        return false;
    }

    // Get a pointer to the start of the current buffer.
    uint8_t* currentBuffer = this->getCurrentBuffer();
    // Compute start index of new sample in current buffer.
    size_t offset = sizeof(timestamp_t) + (sizeof(T) * this->bufferedSamples);
    // Cast sample to bytes.
    uint8_t* sampleBytes = (uint8_t*)&sample;

    // Copy bytes of sample to buffer.
    memcpy(currentBuffer + offset, sampleBytes, sizeof(T));
    // Increase number of buffered samples.
    this->bufferedSamples++;

    return true;
}

template <class T>
void SeriesBuffer<T>::setTimestamp(timestamp_t t)
{
    // Get a pointer to the start of the current buffer.
    uint8_t* currentBuffer = this->getCurrentBuffer();

    // Cast timestamp to bytes.
    uint8_t* timestampBytes = (uint8_t*)&t;

    // Copy bytes of timestamp to start of current buffer.
    memcpy(currentBuffer, timestampBytes, sizeof(timestamp_t));
}

template <class T>
void SeriesBuffer<T>::setLength(size_t len)
{
    // Delete current buffer.
    delete[] this->buffer;

    // Set new length.
    this->length = len;

    // Compute new single buffer size.
    size_t size = this->getSingleBufferSize();
    // Create new buffer with new size.
    this->buffer = new uint8_t[size * this->count] { 0 };

    // Reset current index.
    this->currentIndex = 0;
    // Reset buffered samples.
    this->bufferedSamples = 0;
}

template <class T>
size_t SeriesBuffer<T>::getLength()
{
    return this->length;
}
