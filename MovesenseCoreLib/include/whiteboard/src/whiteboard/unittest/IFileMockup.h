// Copyright (c) Suunto Oy 2018. All rights reserved.
#pragma once

#include "whiteboard/elastic/IFile.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <vector>

#if WB_UNITTEST_BUILD

namespace whiteboard
{

class IFileMockup : public elastic::IFile
{
    using ExpectBoolPair = std::pair<std::string, bool>; // file name, return value
    using ExpectReadPair = std::pair<std::string, std::vector<uint8>*>; // file name, return data pointer
    using WriteCallback = std::function<bool(const std::vector<uint8>&)>;
    using ExpectWritePair = std::pair<std::string, WriteCallback>; // file name, callback
public:
    ~IFileMockup()
    {
        WB_ASSERT(!expectationsExist());
    }

    void setExpectExists(const std::string& fileName, const bool returnValue)
    {
        expectedExists.push_back(ExpectBoolPair(fileName, returnValue));
    }

    bool exists(const std::string& fileName) override
    {
        auto it = getIterator<std::list<ExpectBoolPair>::iterator>(fileName, expectedExists);

        const auto retVal = it->second;
        expectedExists.erase(it);
        return retVal;
    }

    void setExpectedRead(const std::string& fileName, std::vector<uint8>* data)
    {
        expectedRead.push_back(ExpectReadPair(fileName, data));
    }

    bool read(const std::string& fileName, std::vector<uint8>& rOutput) override
    {
        auto it = getIterator<std::list<ExpectReadPair>::iterator>(fileName, expectedRead);

        if (it->second)
        {
            copy(it->second->begin(), it->second->end(), back_inserter(rOutput));
            expectedRead.erase(it);
            return true;
        }
        else
        {
            expectedRead.erase(it);
            return false;
        }
    }

    void setExpectedWrite(const std::string& fileName, WriteCallback writeCallback)
    {
        expectedWrite.push_back(ExpectWritePair(fileName, writeCallback));
    }

    virtual bool write(const std::string& fileName, const std::vector<uint8>& input) override
    {
        auto it = getIterator<std::list<ExpectWritePair>::iterator>(fileName, expectedWrite);
        bool retVal = it->second(input);
        expectedWrite.erase(it);
        return retVal;
    }

private:
    template <typename I, typename L> static I getIterator(const std::string& fileName, L& list)
    {
        auto it = std::find_if(list.begin(), list.end(), [&fileName](const typename L::value_type& p) {
            return fileName == p.first;
        });
        WB_ASSERT(it != list.end());
        return it;
    }

    bool expectationsExist() const
    {
        return !expectedExists.empty() || !expectedRead.empty() || !expectedWrite.empty();
    }

    std::list<ExpectBoolPair> expectedExists;
    std::list<ExpectReadPair> expectedRead;
    std::list<ExpectWritePair> expectedWrite;
};

} // namespace whiteboard

#endif
