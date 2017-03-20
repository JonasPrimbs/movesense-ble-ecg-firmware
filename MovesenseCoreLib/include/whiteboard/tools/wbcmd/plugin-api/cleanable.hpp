#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <whiteboard/builtinTypes/ByteStream.h>

namespace wbcmd {

class ICleanable
{
public:
    virtual ~ICleanable() {}
};

template <typename Type> class Cleaner : public ICleanable
{
public:
    Cleaner(Type* pValue) : ICleanable(), mpValue(pValue)
    {
    }
    ~Cleaner() { delete mpValue; }
    Type* mpValue;
};

template <>
class Cleaner<char*> : public ICleanable
{
public:
    Cleaner(char* pValue) : ICleanable(), mpValue(pValue)
    {
    }
    ~Cleaner() { delete[] mpValue; }
    char* mpValue;
};

template <>
class Cleaner<wb::ByteStream*> : public ICleanable
{
public:
    Cleaner(wb::ByteStream* pValue)
        : ICleanable(), mpValue(pValue)
    {
    }
    ~Cleaner() { wb::ByteStream::deallocate(mpValue); }
    wb::ByteStream* mpValue;
};

}