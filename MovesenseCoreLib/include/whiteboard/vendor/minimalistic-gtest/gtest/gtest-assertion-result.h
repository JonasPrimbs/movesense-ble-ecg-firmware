/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

GTest based minimalistic test framework for embedded devices
******************************************************************************/

#ifndef MINGTEST_INCLUDE_GTEST_GTEST_ASSERTION_RESULT_H_
#define MINGTEST_INCLUDE_GTEST_GTEST_ASSERTION_RESULT_H_

#include "gtest/internal/gtest-port.h"

namespace testing
{

namespace internal
{

class ValueFormatter
{
private:
    typedef enum {
        POINTER,
        BOOL,
        CHAR,
        UCHAR,
        SHORT,
        USHORT,
        INT,
        UINT,
        LONG,
        ULONG,
        LONGLONG,
        ULONGLONG,
        FLOAT,
        DOUBLE,
        OBJECT
    } ValueType;

    ValueType mValueType;
    union {
        const void* pointerValue;
        bool boolValue;
        char charValue;
        unsigned char ucharValue;
        short shortValue;
        unsigned short ushortValue;
        int intValue;
        unsigned int uintValue;
        long longValue;
        unsigned long ulongValue;
        long long longlongValue;
        unsigned long long ulonglongValue;
        float floatValue;
        double doubleValue;
        struct {
            size_t objectSize;
            const void* objectPointer;
        } objectValue;
    } mValue;

public:
    ValueFormatter(const void* value);
    ValueFormatter(const bool value);
    ValueFormatter(const char value);
    ValueFormatter(const unsigned char value);
    ValueFormatter(const short value);
    ValueFormatter(const unsigned short value);
    ValueFormatter(const int value);
    ValueFormatter(const unsigned int value);
    ValueFormatter(const long value);
    ValueFormatter(const unsigned long value);
    ValueFormatter(const long long value);
    ValueFormatter(const unsigned long long value);
    ValueFormatter(const float value);
    ValueFormatter(const double value);

    template <typename T> ValueFormatter(const T& value) : mValueType(OBJECT)
    {
        mValue.objectValue.objectSize = sizeof(T);
        mValue.objectValue.objectPointer = &value;
    }

    ~ValueFormatter();

    // Maximum lenght of formatted string including null terminator
    static const size_t MAX_LENGTH = 48;
    typedef char FormatBuffer[MAX_LENGTH];

    void format(FormatBuffer& buffer) const;
};

} // namespace internal

// A class for indicating whether an assertion was successful.  When
// the assertion wasn't successful, the AssertionResult object
// remembers a non-empty message that describes how it failed.
//
// To create an instance of this class, use one of the factory functions
// (AssertionSuccess() and AssertionFailure()).
//
// This class is useful for two purposes:
//   1. Defining predicate functions to be used with Boolean test assertions
//      EXPECT_TRUE/EXPECT_FALSE and their ASSERT_ counterparts
//   2. Defining predicate-format functions to be
//      used with predicate assertions (ASSERT_PRED_FORMAT*, etc).
//
// For example, if you define IsEven predicate:
//
//   testing::AssertionResult IsEven(int n) {
//     if ((n % 2) == 0)
//       return testing::AssertionSuccess();
//     else
//       return testing::AssertionFailure() << n << " is odd";
//   }
//
// Then the failed expectation EXPECT_TRUE(IsEven(Fib(5)))
// will print the message
//
//   Value of: IsEven(Fib(5))
//     Actual: false (5 is odd)
//   Expected: true
//
// instead of a more opaque
//
//   Value of: IsEven(Fib(5))
//     Actual: false
//   Expected: true
//
// in case IsEven is a simple Boolean predicate.
//
// If you expect your predicate to be reused and want to support informative
// messages in EXPECT_FALSE and ASSERT_FALSE (negative assertions show up
// about half as often as positive ones in our tests), supply messages for
// both success and failure cases:
//
//   testing::AssertionResult IsEven(int n) {
//     if ((n % 2) == 0)
//       return testing::AssertionSuccess() << n << " is even";
//     else
//       return testing::AssertionFailure() << n << " is odd";
//   }
//
// Then a statement EXPECT_FALSE(IsEven(Fib(6))) will print
//
//   Value of: IsEven(Fib(6))
//     Actual: true (8 is even)
//   Expected: false
//
// NB: Predicates that support negative Boolean assertions have reduced
// performance in positive ones so be careful not to use them in tests
// that have lots (tens of thousands) of positive Boolean assertions.
//
// To use this class with EXPECT_PRED_FORMAT assertions such as:
//
//   // Verifies that Foo() returns an even number.
//   EXPECT_PRED_FORMAT1(IsEven, Foo());
//
// you need to define:
//
//   testing::AssertionResult IsEven(const char* expr, int n) {
//     if ((n % 2) == 0)
//       return testing::AssertionSuccess();
//     else
//       return testing::AssertionFailure()
//         << "Expected: " << expr << " is even\n  Actual: it's " << n;
//   }
//
// If Foo() returns 5, you will see the following message:
//
//   Expected: Foo() is even
//     Actual: it's 5
//
class AssertionResult
{
public:
    // Copy constructor.
    // Used in EXPECT_TRUE/FALSE(assertion_result).
    AssertionResult(const AssertionResult& other);
    // Used in the EXPECT_TRUE/FALSE(bool_expression).
    explicit AssertionResult(bool success);

    // Returns true iff the assertion succeeded.
    operator bool() const { return mSuccess; } // NOLINT

    // Returns the text streamed into this AssertionResult. Test assertions
    // use it when they fail (i.e., the predicate's outcome doesn't match the
    // assertion's expectation). When nothing has been streamed into the
    // object, returns an empty string.
    const char* message() const { return mMessage; }

    AssertionResult& operator<<(const char* const value);
    AssertionResult& operator<<(char* const value);
    AssertionResult& operator<<(const internal::ValueFormatter& rFormatter);
    template <typename T> AssertionResult& operator<<(const T value)
    {
        return *this << internal::ValueFormatter(value);
    }

private:
    // Appends string to the assertion result message
    void AppendString(const char* const s);

    // Result of the test
    bool mSuccess;

    // Assertion message
    static char mMessage[256];
    char* mWriteCursor;

    MINGTEST_DISALLOW_ASSIGN_(AssertionResult);
};

// Makes a successful assertion result.
AssertionResult AssertionSuccess();

// Makes a failed assertion result.
AssertionResult AssertionFailure();

} // namespace testing

#endif // #ifndef MINGTEST_INCLUDE_GTEST_GTEST_ASSERTION_RESULT_H_
