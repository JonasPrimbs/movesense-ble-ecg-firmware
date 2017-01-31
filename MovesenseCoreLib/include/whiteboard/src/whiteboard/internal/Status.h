#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

namespace whiteboard
{

/** Whiteboard internal status flags */
enum StatusFlags
{
    /** No Whiteboard instance */
    STATUS_NO_INSTANCE = 1 << 0,

    /** Whiteboard main mutex is locked */
    STATUS_MAIN_MUTEX_LOCKED = 1 << 1,

    /** Buffer pool has run out of small buffers */
    STATUS_BUFFER_POOL_SMALL_EMPTY = 1 << 2,

    /** Buffer pool has run out of medium sized buffers */
    STATUS_BUFFER_POOL_MEDIUM_EMPTY = 1 << 3,

    /** Buffer pool has run out of large buffers */
    STATUS_BUFFER_POOL_LARGE_EMPTY = 1 << 4,

    /** Buffer pool has run out of maximum length buffers */
    STATUS_BUFFER_POOL_MAXLEN_EMPTY = 1 << 5,

    /** Buffer pool has run out of internal communication buffers */
    STATUS_BUFFER_POOL_COMMINT_EMPTY = 1 << 6,

    /** Buffer pool has run out of receive communication buffers */
    STATUS_BUFFER_POOL_COMMRECV_EMPTY = 1 << 7,

    /** Communication mutex is locked */
    STATUS_COMM_MUTEX_LOCKED = 1 << 8,

    /** Routing table is full */
    STATUS_ROUTING_TABLE_FULL = 1 << 9,

    /** Send request pool is empty */
    STATUS_COMM_SEND_REQUEST_POOL_EMPTY = 1 << 10,

    /** Receive request semaphore is locked */
    STATUS_COMM_RECV_REQUEST_LOCKED = 1 << 11,
};

// Timeout to use when probing status
#define PROBE_STATUS_TIMEOUT_MS 25

} // namespace whiteboard
