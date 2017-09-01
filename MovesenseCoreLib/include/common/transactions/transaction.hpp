#pragma once

#include "common/text/Text.hpp"
#include "filesystems/file.h"

namespace nea
{
static const uint32 INVALID_TRANSACTION_ID = 0xFFFFFFFF;

template <class T> class TransactionDatabase
{
    static const size_t NUM_OF_TRANSACTIONS = 5;

public:
    TransactionDatabase() {}
    ~TransactionDatabase() {}

    T* alloc()
    {
        for (size_t iter = 0; iter < NUM_OF_TRANSACTIONS; iter++)
        {
            if (m_transactions[iter].txnId() == INVALID_TRANSACTION_ID)
            {
                return &m_transactions[iter];
            }
        }

        return NULL;
    }

    T* get(const whiteboard::ClientId _clientId, const whiteboard::ResourceId _resourceId)
    {
        for (size_t iter = 0; iter < NUM_OF_TRANSACTIONS; iter++)
        {
            if (m_transactions[iter].m_clientId == _clientId && m_transactions[iter].m_resourceId == _resourceId)
            {
                return &m_transactions[iter];
            }
        }

        return NULL;
    }

private:
    T m_transactions[NUM_OF_TRANSACTIONS];
};

class TransactionInfo
{
    friend class TransactionDatabase<TransactionInfo>;

public:
    TransactionInfo() : m_txnId(INVALID_TRANSACTION_ID) {}
    TransactionInfo(const whiteboard::ClientId clientId, const whiteboard::ResourceId resourceId, const uint32 _txnId)
        : m_clientId(clientId), m_resourceId(resourceId), m_txnId(_txnId)
    {
    }

    uint32 txnId() const { return m_txnId; }

protected:
    whiteboard::ClientId m_clientId;
    whiteboard::ResourceId m_resourceId;
    uint32 m_txnId;
};

class FileTransactionInfo : public TransactionInfo
{
    friend class TransactionDatabase<FileTransactionInfo>;

public:
    static size_t const MAX_FILENAME_LENGTH = MAXPATHLEN;

    FileTransactionInfo() : m_fileOffset(0) {}
    FileTransactionInfo(const whiteboard::ClientId clientId,
                        const whiteboard::ResourceId resourceId,
                        const uint32 _txnId,
                        const uint64_t _fileOffset,
                        const char* const _filename)
        : TransactionInfo(clientId, resourceId, _txnId), m_fileOffset(_fileOffset), m_filename(_filename)
    {
    }

    uint64_t fileOffset() const { return m_fileOffset; }
    char const* filename() const { return m_filename.c_str(); }

protected:
    uint64_t m_fileOffset;
    nea::Text<MAX_FILENAME_LENGTH> m_filename;
};

class RouteTransactionInfo : public TransactionInfo
{
    friend class TransactionDatabase<RouteTransactionInfo>;

public:
    RouteTransactionInfo() : mNumberOfSentRoutePoints(0) {}
    RouteTransactionInfo(const whiteboard::ClientId clientId,
                         const whiteboard::ResourceId resourceId,
                         const uint32_t _txnId,
                         const uint16_t nrOfSentRoutePoints)
        : TransactionInfo(clientId, resourceId, _txnId), mNumberOfSentRoutePoints(nrOfSentRoutePoints)
    {
    }

    uint16_t numberOfSentRoutePoints() const { return mNumberOfSentRoutePoints; }

protected:
    uint16_t mNumberOfSentRoutePoints;
};
}
