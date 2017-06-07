#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/Result.h"
#include "whiteboard/Value.h"

#if WB_UNITTEST_BUILD

#if _MSC_VER
#define strdup _strdup
#endif

namespace whiteboard
{

/** Base class that stores client request results */
class TestResultBase
{
public:
    TestResultBase()
        : mResourceId(ID_INVALID_RESOURCE),
          mCallResultCode(HTTP_CODE_INTERNAL_SERVER_ERROR),
          mValidResponse(false),
          mReturnResultCode(HTTP_CODE_INTERNAL_SERVER_ERROR),
          mValueDataTypeId(0),
          mHasValue(false)
    {
    }

    inline operator bool() const
    {
        return RETURN_OK(mCallResultCode) && mValidResponse && RETURN_OK(mReturnResultCode);
    }

    inline bool checkGetResourceResult(const ResourceId expectedResourceId) const
    {
        return *this && (expectedResourceId == mResourceId);
    }

    inline bool checkAsyncCallResult(Result expectedCallResult) const
    {
        return expectedCallResult == mCallResultCode;
    }

    inline bool checkResult(Result expectedReturnResult) const
    {
        return mValidResponse && RETURN_OK(mCallResultCode) && (expectedReturnResult == mReturnResultCode);
    }

    inline bool checkValueType(ValueType expectedValueType) const
    {
        return mValidResponse && RETURN_OK(mCallResultCode) && (expectedValueType == Value::getType(mValueDataTypeId));
    }

    virtual void setValue(const whiteboard::Value& rValue) = 0;

    ResourceId mResourceId;
    Result mCallResultCode;
    bool mValidResponse;
    Result mReturnResultCode;
    LocalDataTypeId mValueDataTypeId;
    bool mHasValue;
};

/** Class that stores client request results */
template <typename R>
class TestResult : public TestResultBase
{
public:
    TestResult()
    {
    }

    inline TestResult<R> operator=(const TestResult<R>& rResult)
    {
        mResourceId = rResult.mResourceId;
        mCallResultCode = rResult.mCallResultCode;
        mValidResponse = rResult.mValidResponse;
        mReturnResultCode = rResult.mReturnResultCode;
        mValueDataTypeId = rResult.mValueDataTypeId;
        mHasValue = rResult.mHasValue;
        mValue = rResult.mValue;
        return *this;
    }

    inline bool checkValue(const R& expectedValue) const
    {
        return mValidResponse && RETURN_OK(mCallResultCode) && mHasValue && (expectedValue == mValue);
    }

    inline void setValue(const typename Value::ConvertResult<R>::type rValue)
    {
        mHasValue = true;
        mValue = rValue;
    }

    virtual void setValue(const whiteboard::Value& rValue) OVERRIDE
    {
        if (mValueDataTypeId == Value::DataTypeId<R>::value)
        {
            setValue(rValue.convertTo<R>());
        }
    }

    R mValue;
};

/** Base class for specialized versions that allocates memory for string results */
class StringTestResultBase : public TestResultBase
{
public:
    StringTestResultBase()
        : mValue(NULL)
    {
    }

    virtual ~StringTestResultBase()
    {
        if (mValue)
        {
            free(mValue);
        }
    }

    inline bool checkValue(const char* expectedValue) const
    {
        if (!mValidResponse || !RETURN_OK(mCallResultCode) || !mHasValue) return false;
        if (expectedValue == NULL) return mValue == NULL;
        if (mValue == NULL) return false;
        return !strcmp(expectedValue, mValue);
    }

    inline void setValue(const char* rValue)
    {
        mHasValue = true;
        if (rValue != NULL)
        {
            mValue = strdup(rValue);
        }
    }

    virtual void setValue(const whiteboard::Value& rValue) OVERRIDE
    {
        if (mValueDataTypeId == Value::DataTypeId<const char*>::value)
        {
            setValue(rValue.convertTo<const char*>());
        }
    }

    char* mValue;
};

template <>
class TestResult<char*> : public StringTestResultBase
{
public:
    inline TestResult()
    {
    }

    inline TestResult(const TestResult<char*>& other)
    {
        *this = other;
    }

    inline TestResult<char*>& operator=(const TestResult<char*>& other)
    {
        *static_cast<TestResultBase*>(this) = static_cast<const TestResultBase&>(other);

        if (mValue)
        {
            free(mValue);
            mValue = NULL;
        }

        if (other.mValue != NULL)
        {
            mValue = strdup(other.mValue);
        }

        return *this;
    }
};

template <>
class TestResult<const char*> : public StringTestResultBase
{
public:
    inline TestResult()
    {
    }

    inline TestResult(const TestResult<const char*>& other)
    {
        *this = other;
    }

    inline TestResult<const char*>& operator=(const TestResult<const char*>& other)
    {
        *static_cast<TestResultBase*>(this) = static_cast<const TestResultBase&>(other);

        if (mValue)
        {
            free(mValue);
            mValue = NULL;
        }

        if (other.mValue != NULL)
        {
            mValue = strdup(other.mValue);
        }

        return *this;
    }
};

/** Class that stores client's asynchronous request results */
template <typename R>
class AsyncTestResult
{
public:
    AsyncTestResult()
    {
        mpResult = New<ReferenceCountedResult>();
    }

    AsyncTestResult(const AsyncTestResult& rOther)
    {
        mpResult = rOther.mpResult;
        ++mpResult->mReferenceCount;
    }

    ~AsyncTestResult()
    {
        WB_ASSERT(mpResult->mReferenceCount > 0);
        if (--mpResult->mReferenceCount == 0)
        {
            Delete(mpResult);
        }
    }

    inline operator bool() const
    {
        return *mpResult;
    }

    inline bool checkGetResourceResult(const ResourceId expectedResourceId) const
    {
        return mpResult->checkGetResourceResult(expectedResourceId);
    }

    inline bool checkAsyncCallResult(Result expectedCallResult) const
    {
        return mpResult->checkAsyncCallResult(expectedCallResult);
    }

    inline bool checkResult(Result expectedReturnResult) const
    {
        return mpResult->checkResult(expectedReturnResult);
    }

    inline bool checkValueType(ValueType expectedValueType) const
    {
        return mpResult->checkValueType(expectedValueType);
    }

    inline AsyncTestResult<R>& operator=(const AsyncTestResult<R>& rResult)
    {
        WB_ASSERT(mpResult->mReferenceCount > 0);
        if (--mpResult->mReferenceCount == 0)
        {
            Delete(mpResult);
        }

        mpResult = rResult.mpResult;
        ++mpResult->mReferenceCount;
        return *this;
    }

    inline bool checkValue(const R& expectedValue) const
    {
        return mpResult->checkValue(expectedValue);
    }

    inline void setValue(const typename Value::ConvertResult<R>::type rValue)
    {
        mpResult->setValue(rValue);
    }

    inline operator TestResult<R>&()
    {
        return *mpResult;
    }

private:
    class ReferenceCountedResult : public TestResult<R>
    {
    public:
        ReferenceCountedResult()
            : mReferenceCount(1)
        {
        }

        size_t mReferenceCount;
    };

    ReferenceCountedResult* mpResult;
};

/** GTEST helpers */
#define EXPECT_WBGETRESOURCERESULT(expectedResId, testResult)  EXPECT_TRUE(testResult.checkGetResourceResult(expectedResId))
#define EXPECT_WBCALLRESULT(expectedCallResult, asyncTestResult)  EXPECT_TRUE(asyncTestResult.checkAsyncCallResult(expectedCallResult))
#define EXPECT_WBRESULT(expectedReturnResult, testResult)  EXPECT_TRUE(testResult.checkResult(expectedReturnResult))
#define EXPECT_WBVALUETYPE(expectedValueType, testResult)  EXPECT_TRUE(testResult.checkValueType(expectedValueType))
#define EXPECT_WBVALUE(expectedValue, testResult)  EXPECT_TRUE(testResult.checkValue(expectedValue))
#define EXPECT_WBVALUESTR(expectedValue, testResult)  EXPECT_TRUE(testResult.checkValue(expectedValue))

} // namespace whiteboard

#endif
