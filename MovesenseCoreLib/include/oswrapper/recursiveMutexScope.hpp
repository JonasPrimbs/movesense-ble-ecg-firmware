#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2012.
All rights reserved.

******************************************************************************/
#include "oswrapper/mutex.h"

namespace nea
{

/**
   Recursive mutex scope, lock mutex until this item has deleted
 */
class RecursiveMutexScope
{
public:
    explicit RecursiveMutexScope(MutexHandle handle) : mHandle(handle) { mutexLockRecursive(mHandle); }
    ~RecursiveMutexScope(void) { mutexUnlockRecursive(mHandle); }

private:
    RecursiveMutexScope(void);
    RecursiveMutexScope(const RecursiveMutexScope&);
    RecursiveMutexScope& operator=(const RecursiveMutexScope&);

private:
    MutexHandle mHandle;
};

} // namespace nea
