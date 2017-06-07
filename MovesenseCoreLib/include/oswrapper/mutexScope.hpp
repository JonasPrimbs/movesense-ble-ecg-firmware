#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2012.
All rights reserved.

******************************************************************************/
#include "oswrapper/mutex.h"

namespace nea
{

/**
   Mutex scope, lock mutex until this item has deleted
 */
class MutexScope
{
public:
    MutexScope(MutexHandle handle) : mHandle(handle) { mutexLock(mHandle); }
    ~MutexScope(void) { mutexUnlock(mHandle); }

private:
    MutexScope(void);
    MutexScope(const MutexScope&);
    MutexScope& operator=(const MutexScope&);

private:
    MutexHandle mHandle;
};

} // namespace nea
