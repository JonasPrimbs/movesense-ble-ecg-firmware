#pragma once
// Copyright (c) Suunto Oy 2014. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/Request.h"

namespace whiteboard
{

/**
* Queue for requests
*
* @tparam QUEUE_SIZE Maximum size of the queue
* @tparam ExtraDataType Type of caller specified extra data
*/
template <size_t QUEUE_SIZE, typename ExtraDataType> class RequestQueue
{
public:
    /**
     * Default constructor
     */
    RequestQueue();

    /**
    * Checks whether the queue is empty
    *
    * @return A value indicating whether the queue is empty
    */
    inline bool isEmpty() const;

    /**
    * Checks whether the queue is full
    *
    * @return A value indicating whether the queue is full
    */
    inline bool isFull() const;

    /**
    * Queues a new request
    *
    * @param rRequest Request information
    * @param rExtraData Caller specified extra data
    * @return True if request could be queued. False if the queue was full.
    */
    bool push(const Request& rRequest, const ExtraDataType& rExtraData);

    /**
    * Pops first request from the queue
    *
    * @param rRequest Request information
    * @param rExtraData On output contains caller specified extra data
    * @return True if the request was successfully dequeued. False if the queue was empty.
    */
    bool pop(Request& rRequest, ExtraDataType& rExtraData);

private:
    // Index of current read position
    size_t mReadPosition;

    // Number of requests in the queue
    size_t mQueueLength;

    // Array of requests
    Request mRequests[QUEUE_SIZE];

    // Array of extra data
    ExtraDataType mExtraData[QUEUE_SIZE];
};

/**
* Queue for requests
*
* @tparam QUEUE_SIZE Maximum size of the queue
* @tparam ExtraDataType Type of caller specified extra data
*/
template <size_t QUEUE_SIZE> class RequestQueue<QUEUE_SIZE, void>
{
public:
    /**
    * Default constructor
    */
    RequestQueue();

    /**
    * Checks whether the queue is empty
    *
    * @return A value indicating whether the queue is empty
    */
    bool isEmpty() const;

    /**
    * Checks whether the queue is full
    *
    * @return A value indicating whether the queue is full
    */
    bool isFull() const;

    /**
    * Queues a new request
    *
    * @param rRequest Request information
    * @return True if request could be queued. False if the queue was full.
    */
    bool push(const Request& rRequest);

    /**
    * Pops first request from a queue
    *
    * @param rRequest Request information
    * @return True if the request was successfully dequeued. False if the queue was empty.
    */
    bool pop(Request& rRequest);

private:
    // Index of current read position
    size_t mReadPosition;

    // Number of requests in the queue
    size_t mQueueLength;

    // Array of requests
    Request mRequests[QUEUE_SIZE];
};

template <size_t QUEUE_SIZE, typename ExtraDataType>
RequestQueue<QUEUE_SIZE, ExtraDataType>::RequestQueue() : mReadPosition(0), mQueueLength(0)
{
}

template <size_t QUEUE_SIZE, typename ExtraDataType> bool RequestQueue<QUEUE_SIZE, ExtraDataType>::isEmpty() const
{
    return mQueueLength == 0;
}

template <size_t QUEUE_SIZE, typename ExtraDataType> bool RequestQueue<QUEUE_SIZE, ExtraDataType>::isFull() const
{
    return mQueueLength == QUEUE_SIZE;
}

template <size_t QUEUE_SIZE, typename ExtraDataType>
bool RequestQueue<QUEUE_SIZE, ExtraDataType>::push(const Request& rRequest, const ExtraDataType& rExtraData)
{
    if (mQueueLength == QUEUE_SIZE)
    {
        return false;
    }

    size_t writePosition = (mReadPosition + mQueueLength) % QUEUE_SIZE;
    mRequests[writePosition] = rRequest;
    mExtraData[writePosition] = rExtraData;
    ++mQueueLength;
    return true;
}

template <size_t QUEUE_SIZE, typename ExtraDataType>
bool RequestQueue<QUEUE_SIZE, ExtraDataType>::pop(Request& rRequest, ExtraDataType& rExtraData)
{
    if (mQueueLength == 0)
    {
        return false;
    }

    rRequest = mRequests[mReadPosition];
    rExtraData = mExtraData[mReadPosition];
    mReadPosition = (mReadPosition + 1) % QUEUE_SIZE;
    --mQueueLength;
    return true;
}

template <size_t QUEUE_SIZE> RequestQueue<QUEUE_SIZE, void>::RequestQueue() : mReadPosition(0), mQueueLength(0)
{
}

template <size_t QUEUE_SIZE> bool RequestQueue<QUEUE_SIZE, void>::isEmpty() const
{
    return mQueueLength == 0;
}

template <size_t QUEUE_SIZE> bool RequestQueue<QUEUE_SIZE, void>::isFull() const
{
    return mQueueLength == QUEUE_SIZE;
}

template <size_t QUEUE_SIZE> bool RequestQueue<QUEUE_SIZE, void>::push(const Request& rRequest)
{
    if (mQueueLength == QUEUE_SIZE)
    {
        return false;
    }

    size_t writePosition = (mReadPosition + mQueueLength) % QUEUE_SIZE;
    mRequests[writePosition] = rRequest;
    ++mQueueLength;
    return true;
}

template <size_t QUEUE_SIZE> bool RequestQueue<QUEUE_SIZE, void>::pop(Request& rRequest)
{
    if (mQueueLength == 0)
    {
        return false;
    }

    rRequest = mRequests[mReadPosition];
    mReadPosition = (mReadPosition + 1) % QUEUE_SIZE;
    --mQueueLength;
    return true;
}

} // namespace whiteboard
