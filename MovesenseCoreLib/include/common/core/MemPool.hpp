/******************************************************************************

        Copyright (c) Suunto Oy 2016.
        All rights reserved.

******************************************************************************/
#pragma once

#include "common/collections/ListNode.hpp"
#include "common/compiler/genType.h"
#include "oswrapper/semaphore.h"
#include <new>

class MemPool
{
public:
    MemPool(void* pData, size_t size, size_t count);
    ~MemPool();

    void* reserve();
    void release(void* pData);

private:
    void releaseInternal(void* pData);

    ListNode mFree;
    SemaphoreHandle mLock;
    uint8* mpData;
#ifdef BUILD_WITH_DEBUG
    size_t mSize;
    size_t mCount;
#endif
};

template <unsigned SZ, typename T> class ObjPool : private MemPool
{
public:
    ObjPool() : mBuffer({0}), MemPool(mBuffer, sizeof(uint32) * ((MAX(sizeof(ListNode), sizeof(T)) + 3) >> 2), SZ) {}

    T* reserve()
    {
        T* pObj = static_cast<T*>(MemPool::reserve());
        new (pObj) T();
        return pObj;
    }

    void release(T* pObj)
    {
        if (pObj)
        {
            pObj->~T();
            MemPool::release(pObj);
        }
    }

private:
    uint32 mBuffer[SZ * ((MAX(sizeof(ListNode), sizeof(T)) + 3) >> 2)];
};
