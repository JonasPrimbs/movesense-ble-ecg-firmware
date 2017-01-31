/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

GTest based minimalistic test framework for embedded devices
******************************************************************************/

// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Authors: wan@google.com (Zhanyong Wan), eefacm@gmail.com (Sean Mcafee)
//
// The Google C++ Testing Framework (Google Test)

// This header file declares functions and macros used internally by
// Google Test.  They are subject to change without notice.

#ifndef MINGTEST_INCLUDE_GTEST_INTERNAL_GTEST_INTERNAL_H_
#define MINGTEST_INCLUDE_GTEST_INTERNAL_GTEST_INTERNAL_H_

#include "gtest/gtest-assertion-result.h"
#include <float.h>

namespace testing
{

// Forward declarations.

class Test; // Represents a test.

namespace internal
{

class UnitTestImpl; // Opaque implementation of UnitTest

// A secret type that Google Test users don't know about.  It has no
// definition on purpose.  Therefore it's impossible to create a
// Secret object, which is what we want.
class Secret;

// Two overloaded helpers for checking at compile time whether an
// expression is a null pointer literal (i.e. NULL or any 0-valued
// compile-time integral constant).  Their return values have
// different sizes, so we can use sizeof() to test which version is
// picked by the compiler.  These helpers have no implementations, as
// we only need their signatures.
//
// Given IsNullLiteralHelper(x), the compiler will pick the first
// version if x can be implicitly converted to Secret*, and pick the
// second version otherwise.  Since Secret is a secret and incomplete
// type, the only expression a user can write that has type Secret* is
// a null pointer literal.  Therefore, we know that x is a null
// pointer literal if and only if the first version is picked by the
// compiler.
char IsNullLiteralHelper(Secret* p);
char(&IsNullLiteralHelper(...))[2]; // NOLINT

// A compile-time bool constant that is true if and only if x is a
// null pointer literal (i.e. NULL or any 0-valued compile-time
// integral constant).
#ifdef MINGTEST_ELLIPSIS_NEEDS_POD_
// We lose support for NULL detection where the compiler doesn't like
// passing non-POD classes through ellipsis (...).
#define MINGTEST_IS_NULL_LITERAL_(x) false
#else
#define MINGTEST_IS_NULL_LITERAL_(x) (sizeof(::testing::internal::IsNullLiteralHelper(x)) == 1)
#endif // MINGTEST_ELLIPSIS_NEEDS_POD_

// Formats a comparison assertion (e.g. ASSERT_EQ, EXPECT_LT, and etc)
// operand to be used in a failure message.  The type (but not value)
// of the other operand may affect the format.  This allows us to
// print a char* as a raw pointer when it is compared against another
// char*, and print it as a C string when it is compared against an
// std::string object, for example.
//
// The default implementation ignores the type of the other operand.
// Some specialized versions are used to handle formatting wide or
// narrow C strings.
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
template <typename T1, typename T2>
internal::ValueFormatter FormatForComparisonFailureMessage(const T1& value,
                                                           const T2& /* other_operand */)
{
    // C++Builder compiles this incorrectly if the namespace isn't explicitly
    // given.
    return ::testing::internal::ValueFormatter(value);
}

// The next four overloads handle the case where the operand being
// printed is a char/wchar_t pointer and the other operand is not a
// string/wstring object.  In such cases, we just print the operand as
// a pointer to be safe.
#define GTEST_FORMAT_CHAR_PTR_IMPL_(CharType)                                                      \
    template <typename T>                                                                          \
    internal::ValueFormatter FormatForComparisonFailureMessage(CharType* p, const T&)              \
    {                                                                                              \
        return ::testing::internal::ValueFormatter(static_cast<const void*>(p));                   \
    }

GTEST_FORMAT_CHAR_PTR_IMPL_(char)
GTEST_FORMAT_CHAR_PTR_IMPL_(const char)

#undef GTEST_FORMAT_CHAR_PTR_IMPL_

// Constructs and returns the message for an equality assertion
// (e.g. ASSERT_EQ, EXPECT_STREQ, etc) failure.
//
// The first four parameters are the expressions used in the assertion
// and their values, as strings.  For example, for ASSERT_EQ(foo, bar)
// where foo is 5 and bar is 6, we have:
//
//   expected_expression: "foo"
//   actual_expression:   "bar"
//   expected_value:      "5"
//   actual_value:        "6"
//
// The ignoring_case parameter is true iff the assertion is a
// *_STRCASEEQ*.  When it's true, the string " (ignoring case)" will
// be inserted into the message.
template <typename T1, typename T2>
AssertionResult EQFailure(const char* expected_expression, const char* actual_expression,
                          const T1& expected_value, const T2& actual_value)
{
    return AssertionFailure() << "Value of: " << actual_expression << "\r\n  Actual: "
                              << FormatForComparisonFailureMessage(actual_value, expected_value)
                              << "\r\nExpected: " << expected_expression << "\r\nWhich is: "
                              << FormatForComparisonFailureMessage(expected_value, actual_value);
}

// Constructs and returns the message for an equality assertion
// (e.g. ASSERT_NE, EXPECT_STRNE, etc) failure.
//
// The first four parameters are the expressions used in the assertion
// and their values, as strings.  For example, for ASSERT_NE(foo, bar)
// where foo is 5 and bar is 6, we have:
//
//   expected_expression: "foo"
//   actual_expression:   "bar"
//   expected_value:      "5"
//   actual_value:        "6"
//
// The ignoring_case parameter is true iff the assertion is a
// *_STRCASEEQ*.  When it's true, the string " (ignoring case)" will
// be inserted into the message.
template <typename T1, typename T2>
AssertionResult NEFailure(const char* expected_expression, const char* actual_expression,
                          const T1& expected_value, const T2& actual_value)
{
    return AssertionFailure() << "\r\nExpected: (" << expected_expression << ") != ("
                              << actual_expression << ")"
                              << "\r\n  Actual: "
                              << FormatForComparisonFailureMessage(actual_value, expected_value)
                              << " vs "
                              << FormatForComparisonFailureMessage(expected_value, actual_value);
}

// Constructs a failure message for Boolean assertions such as EXPECT_TRUE.
AssertionResult GetBoolAssertionFailureMessage(const char* expression_text,
                                               const char* actual_predicate_value,
                                               const char* expected_predicate_value);

template <typename RawType> class digits
{
public:
    static const size_t count = 0;
};

template <> class digits<float>
{
public:
    static const size_t count = FLT_MANT_DIG;
};

template <> class digits<double>
{
public:
    static const size_t count = DBL_MANT_DIG;
};

// This template class represents an IEEE floating-point number
// (either single-precision or double-precision, depending on the
// template parameters).
//
// The purpose of this class is to do more sophisticated number
// comparison.  (Due to round-off error, etc, it's very unlikely that
// two floating-points will be equal exactly.  Hence a naive
// comparison by the == operation often doesn't work.)
//
// Format of IEEE floating-point:
//
//   The most-significant bit being the leftmost, an IEEE
//   floating-point looks like
//
//     sign_bit exponent_bits fraction_bits
//
//   Here, sign_bit is a single bit that designates the sign of the
//   number.
//
//   For float, there are 8 exponent bits and 23 fraction bits.
//
//   For double, there are 11 exponent bits and 52 fraction bits.
//
//   More details can be found at
//   http://en.wikipedia.org/wiki/IEEE_floating-point_standard.
//
// Template parameter:
//
//   RawType: the raw floating-point type (either float or double)
template <typename RawType> class FloatingPoint
{
public:
    // Defines the unsigned integer type that has the same size as the
    // floating point number.
    typedef typename TypeWithSize<sizeof(RawType)>::UInt Bits;

    // Constants.

    // # of bits in a number.
    static const size_t kBitCount = 8 * sizeof(RawType);

    // # of fraction bits in a number.
    static const size_t kFractionBitCount = digits<RawType>::count - 1;

    // # of exponent bits in a number.
    static const size_t kExponentBitCount = kBitCount - 1 - kFractionBitCount;

    // The mask for the sign bit.
    static const Bits kSignBitMask = static_cast<Bits>(1) << (kBitCount - 1);

    // The mask for the fraction bits.
    static const Bits kFractionBitMask = ~static_cast<Bits>(0) >> (kExponentBitCount + 1);

    // The mask for the exponent bits.
    static const Bits kExponentBitMask = ~(kSignBitMask | kFractionBitMask);

    // How many ULP's (Units in the Last Place) we want to tolerate when
    // comparing two numbers.  The larger the value, the more error we
    // allow.  A 0 value means that two numbers must be exactly the same
    // to be considered equal.
    //
    // The maximum error of a single floating-point operation is 0.5
    // units in the last place.  On Intel CPU's, all floating-point
    // calculations are done with 80-bit precision, while double has 64
    // bits.  Therefore, 4 should be enough for ordinary use.
    //
    // See the following article for more details on ULP:
    // http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm.
    static const size_t kMaxUlps = 4;

    // Constructs a FloatingPoint from a raw floating-point number.
    //
    // On an Intel CPU, passing a non-normalized NAN (Not a Number)
    // around may change its bits, although the new value is guaranteed
    // to be also a NAN.  Therefore, don't expect this constructor to
    // preserve the bits in x when x is a NAN.
    explicit FloatingPoint(const RawType& x) { u_.value_ = x; }

    // Static methods

    // Reinterprets a bit pattern as a floating-point number.
    //
    // This function is needed to test the AlmostEquals() method.
    static RawType ReinterpretBits(const Bits bits)
    {
        FloatingPoint fp(0);
        fp.u_.bits_ = bits;
        return fp.u_.value_;
    }

    // Returns the floating-point number that represent positive infinity.
    static RawType Infinity() { return ReinterpretBits(kExponentBitMask); }

    // Non-static methods

    // Returns the bits that represents this number.
    const Bits& bits() const { return u_.bits_; }

    // Returns the exponent bits of this number.
    Bits exponent_bits() const { return kExponentBitMask & u_.bits_; }

    // Returns the fraction bits of this number.
    Bits fraction_bits() const { return kFractionBitMask & u_.bits_; }

    // Returns the sign bit of this number.
    Bits sign_bit() const { return kSignBitMask & u_.bits_; }

    // Returns true iff this is NAN (not a number).
    bool is_nan() const
    {
        // It's a NAN if the exponent bits are all ones and the fraction
        // bits are not entirely zeros.
        return (exponent_bits() == kExponentBitMask) && (fraction_bits() != 0);
    }

    // Returns true iff this number is at most kMaxUlps ULP's away from
    // rhs.  In particular, this function:
    //
    //   - returns false if either number is (or both are) NAN.
    //   - treats really large numbers as almost equal to infinity.
    //   - thinks +0.0 and -0.0 are 0 DLP's apart.
    bool AlmostEquals(const FloatingPoint& rhs) const
    {
        // The IEEE standard says that any comparison operation involving
        // a NAN must return false.
        if (is_nan() || rhs.is_nan())
            return false;

        return DistanceBetweenSignAndMagnitudeNumbers(u_.bits_, rhs.u_.bits_) <= kMaxUlps;
    }

private:
    // The data type used to store the actual floating-point number.
    union FloatingPointUnion {
        RawType value_; // The raw floating-point number.
        Bits bits_;     // The bits that represent the number.
    };

    // Converts an integer from the sign-and-magnitude representation to
    // the biased representation.  More precisely, let N be 2 to the
    // power of (kBitCount - 1), an integer x is represented by the
    // unsigned number x + N.
    //
    // For instance,
    //
    //   -N + 1 (the most negative number representable using
    //          sign-and-magnitude) is represented by 1;
    //   0      is represented by N; and
    //   N - 1  (the biggest number representable using
    //          sign-and-magnitude) is represented by 2N - 1.
    //
    // Read http://en.wikipedia.org/wiki/Signed_number_representations
    // for more details on signed number representations.
    static Bits SignAndMagnitudeToBiased(const Bits& sam)
    {
        if (kSignBitMask & sam) {
            // sam represents a negative number.
            return ~sam + 1;
        } else {
            // sam represents a positive number.
            return kSignBitMask | sam;
        }
    }

    // Given two numbers in the sign-and-magnitude representation,
    // returns the distance between them as an unsigned number.
    static Bits DistanceBetweenSignAndMagnitudeNumbers(const Bits& sam1, const Bits& sam2)
    {
        const Bits biased1 = SignAndMagnitudeToBiased(sam1);
        const Bits biased2 = SignAndMagnitudeToBiased(sam2);
        return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
    }

    FloatingPointUnion u_;
};

// Typedefs the instances of the FloatingPoint template class that we
// care to use.
typedef FloatingPoint<float> Float;
typedef FloatingPoint<double> Double;

// Types of SetUpTestCase() and TearDownTestCase() functions.
typedef void (*SetUpTestCaseFunc)();
typedef void (*TearDownTestCaseFunc)();

// Helpers for suppressing warnings on unreachable code or constant
// condition.

// Always returns true.
bool AlwaysTrue();

// Always returns false.
inline bool AlwaysFalse()
{
    return !AlwaysTrue();
}

// EnableIf<condition>::type is void when 'Cond' is true, and
// undefined when 'Cond' is false.  To use SFINAE to make a function
// overload only apply when a particular expression is true, add
// "typename EnableIf<expression>::type* = 0" as the last parameter.
template <bool> struct EnableIf;
template <> struct EnableIf<true> {
    typedef void type;
}; // NOLINT

} // namespace internal
} // namespace testing

#define MINGTEST_MESSAGE_AT_(file, line, message, result_type)                                     \
    ::testing::internal::AssertHelper(result_type, file, line) = message

#define MINGTEST_MESSAGE_(message, result_type)                                                    \
    MINGTEST_MESSAGE_AT_(__FILE__, __LINE__, message, result_type)

#define MINGTEST_FATAL_FAILURE_(message)                                                           \
    return MINGTEST_MESSAGE_(message, ::testing::TestResult::FATAL_FAILURE)

#define MINGTEST_NONFATAL_FAILURE_(message)                                                        \
    MINGTEST_MESSAGE_(message, ::testing::TestResult::FAILURE)

#define MINGTEST_SUCCESS_(message) MINGTEST_MESSAGE_(message, ::testing::TestResult::PASSED)

// Implements Boolean test assertions such as EXPECT_TRUE. expression can be
// either a bool expression or an AssertionResult. text is a textual
// represenation of expression as it was passed into the EXPECT_TRUE.
#define MINGTEST_TEST_BOOLEAN_(expression, text, actual, expected, on_failure)                     \
    MINGTEST_AMBIGUOUS_ELSE_BLOCKER_                                                               \
    if (::testing::AssertionResult(expression))                                                    \
        ;                                                                                          \
    else                                                                                           \
    on_failure(::testing::internal::GetBoolAssertionFailureMessage(text, #actual, #expected))

// Expands to the name of the class that implements the given test.
#define MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name) test_case_name##_##test_name##_Test

// Helper macro for defining tests.
#if 0
#define MINGTEST_TEST_(test_case_name, test_name, parent_class)                                    \
    \
class MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)                                         \
        : public parent_class                                                                      \
    {                                                                                              \
    public:                                                                                        \
        MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)() {}                                  \
        static MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name) Instance;                      \
                                                                                                   \
    private:                                                                                       \
        virtual void TestBody();                                                                   \
        static ::testing::TestInfo const test_info_ MINGTEST_ATTRIBUTE_UNUSED_;                    \
        MINGTEST_DISALLOW_COPY_AND_ASSIGN_(MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name));  \
    \
};                                                                                                 \
    \
\
::testing::TestInfo const MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)::test_info_(        \
        #test_case_name, #test_name, NULL, NULL, parent_class::,                                   \
        ::testing::internal::TestFactoryImpl < GTEST_TEST_CLASS_NAME_(test_case_name, test_name)); \
    \
MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)                                               \
    MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)::Instance;                                \
    \
void MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()
#endif

#define MINGTEST_TEST_(test_case_name, test_name, parent_class)                                    \
    \
class MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)                                         \
        : public parent_class                                                                      \
    {                                                                                              \
    public:                                                                                        \
        MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)() {}                                  \
    private:                                                                                       \
        virtual void TestBody();                                                                   \
        static ::testing::TestInfoImpl<MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)> const \
            test_info_ MINGTEST_ATTRIBUTE_UNUSED_;                                                 \
        MINGTEST_DISALLOW_COPY_AND_ASSIGN_(MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name));  \
    \
};                                                                                                 \
    \
\
::testing::TestInfoImpl<MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)> const                \
        MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)::test_info_(#test_case_name,          \
                                                                         #test_name, NULL, NULL);  \
    \
void MINGTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()

// Note that we call GetTestTypeId() instead of GetTypeId<
// ::testing::Test>() here to get the type ID of testing::Test.  This
// is to work around a suspected linker bug when using Google Test as
// a framework on Mac OS X.  The bug causes GetTypeId<
// ::testing::Test>() to return different values depending on whether
// the call is from the Google Test framework itself or from user test
// code.  GetTestTypeId() is guaranteed to always return the same
// value, as it always calls GetTypeId<>() from the Google Test
// framework.
#define MINGTEST_TEST(test_case_name, test_name)                                                   \
    MINGTEST_TEST_(test_case_name, test_name, ::testing::Test)

#define TEST(test_case_name, test_name) MINGTEST_TEST(test_case_name, test_name)

// Defines a test that uses a test fixture.
//
// The first parameter is the name of the test fixture class, which
// also doubles as the test case name.  The second parameter is the
// name of the test within the test case.
//
// A test fixture class must be declared earlier.  The user should put
// his test code between braces after using this macro.  Example:
//
//   class FooTest : public testing::Test {
//    protected:
//     virtual void SetUp() { b_.AddElement(3); }
//
//     Foo a_;
//     Foo b_;
//   };
//
//   TEST_F(FooTest, InitializesCorrectly) {
//     EXPECT_TRUE(a_.StatusIsOK());
//   }
//
//   TEST_F(FooTest, ReturnsElementCountCorrectly) {
//     EXPECT_EQ(0, a_.size());
//     EXPECT_EQ(1, b_.size());
//   }

#define TEST_F(test_fixture, test_name) MINGTEST_TEST_(test_fixture, test_name, test_fixture)

#endif // MINGTEST_INCLUDE_GTEST_INTERNAL_GTEST_INTERNAL_H_
