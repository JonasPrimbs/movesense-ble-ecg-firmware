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
// Author: wan@google.com (Zhanyong Wan)
//
// The Google C++ Testing Framework (Google Test)

#ifndef MINIMALISTIC_GTEST_GTEST_H_
#define MINIMALISTIC_GTEST_GTEST_H_

// Added by Markus Lindqvist, 05/2015 for forcelinking in Visual Studio build
#define FORCE_LINK_THIS(x) int force_link_##x = 0;
#define FORCE_LINK_THAT(x)                                                                         \
    do {                                                                                           \
        extern int force_link_##x;                                                                 \
        force_link_##x = 1;                                                                        \
    } while (0)

#define NO_STRING

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gtest/internal/gtest-port.h"
#include "gtest/internal/gtest-internal.h"
//#include "gtest/gtest-param-test.h"

namespace testing
{
namespace internal
{
class AssertHelper;
}

//////////////////////////////
// TEST
//////////////////////////////

// The result of a single Test.  This includes a list of
// TestPartResults, a list of TestProperties, a count of how many
// death tests there are in the Test, and how much time it took to run
// the Test.
//
// TestResult is not copyable.
class TestResult
{
public:
    typedef enum { PASSED, FAILURE, FATAL_FAILURE, SKIPPED } PartResult;

    // Creates an empty TestResult.
    TestResult();

    // D'tor.  Do not inherit from TestResult.
    ~TestResult();

    // Gets the number of all test parts.  This is the sum of the number
    // of successful test parts and the number of failed test parts.
    size_t total_part_count() const;

    // Returns true iff the test passed (i.e. no test part failed).
    bool Passed() const { return !Failed(); }

    // Returns true iff the test failed.
    bool Failed() const;

    // Returns true iff the test fatally failed.
    bool HasFatalFailure() const;

    // Returns true iff the test has a non-fatal failure.
    bool HasNonfatalFailure() const;

private:
    friend class TestInfo;
    friend class internal::AssertHelper;

    // Adds a test part result to the list.
    void AddTestPartResult(const PartResult test_part_result);

    // We disallow copying TestResult.
    MINGTEST_DISALLOW_COPY_AND_ASSIGN_(TestResult);

private:
    size_t mPassedParts;
    size_t mFailedParts;
    size_t mFatallyFailedParts;
    size_t mSkippedParts;
}; // class TestResult

// A TestInfo object stores the following information about a test:
//
//   Test case name
//   Test name
//   Whether the test should be run
//   A function pointer that creates the test object when invoked
//   Test result
//
// The constructor of TestInfo registers itself with the UnitTest
// singleton such that the RUN_ALL_TESTS() macro knows which tests to
// run.
class TestInfo
{
public:
    // Constructs a TestInfo object. The newly constructed instance assumes
    // ownership of the factory object.
    TestInfo(const char* test_case_name, const char* name, const char* a_type_param,
             const char* a_value_param);

    // Destructs a TestInfo object.
    virtual ~TestInfo();

    // Returns the test case name.
    const char* test_case_name() const { return test_case_name_; }

    // Returns the test name.
    const char* name() const { return name_; }

    // Returns the name of the parameter type, or NULL if this is not a typed
    // or a type-parameterized test.
    const char* type_param() const { return type_param_; }

    // Returns the text representation of the value parameter, or NULL if this
    // is not a value-parameterized test.
    const char* value_param() const { return value_param_; }

    // Returns true if this test should run, that is if the test is not disabled
    // (or it is disabled but the also_run_disabled_tests flag has been specified)
    // and its full name matches the user-specified filter.
    //
    // Google Test allows the user to filter the tests by their full names.
    // The full name of a test Bar in test case Foo is defined as
    // "Foo.Bar".  Only the tests that match the filter will run.
    //
    // A filter is a colon-separated list of glob (not regex) patterns,
    // optionally followed by a '-' and a colon-separated list of
    // negative patterns (tests to exclude).  A test is run if it
    // matches one of the positive patterns and does not match any of
    // the negative patterns.
    //
    // For example, *A*:Foo.* is a filter that matches any string that
    // contains the character 'A' or starts with "Foo.".
    bool should_run() const { return should_run_; }

    // Returns the result of the test.
    TestResult* result() { return &result_; }

    bool is_first_of_test_case() const { return is_first_of_test_case_; }
    bool is_last_of_test_case() const { return is_last_of_test_case_; }

protected:
    // Creates the test object, runs it, records its result, and then
    // deletes it.
    void Run();

    // Creates the test object
    virtual Test* CreateTest() = 0;

    virtual void SetUpTestCase() = 0;
    virtual void TearDownTestCase() = 0;

private:
    friend class UnitTest;

    static void Init();

private:
    // These fields are immutable properties of the test.
    const char* test_case_name_; // Test case name
    const char* name_;           // Test name
    // Name of the parameter type, or NULL if this is not a typed or a
    // type-parameterized test.
    const char* type_param_;
    // Text representation of the value parameter, or NULL if this is not a
    // value-parameterized test.
    const char* value_param_;
    bool should_run_;  // True iff this test should run
    bool is_disabled_; // True iff this test is disabled

    bool is_first_of_test_case_; // True iff this test is first of its test case
    bool is_last_of_test_case_;  // True iff this test is last of its test case

    // This field is mutable and needs to be reset before running the
    // test for the second time.
    TestResult result_;

    MINGTEST_DISALLOW_COPY_AND_ASSIGN_(TestInfo);

private:
    /** First registered test info */
    static TestInfo* spFirst;

    /** Next registered test info in linked list */
    TestInfo* mpNext;
};

template <class TestClass> class TestInfoImpl : public TestInfo
{
public:
    TestInfoImpl(const char* a_test_case_name, const char* a_name, const char* a_type_param,
                 const char* a_value_param)
        : TestInfo(a_test_case_name, a_name, a_type_param, a_value_param)
    {
    }

    virtual ~TestInfoImpl() {}

private:
    // Creates the test object
    Test* CreateTest() { return new TestClass(); }
    void SetUpTestCase() { TestClass::SetUpTestCase(); }
    void TearDownTestCase() { TestClass::TearDownTestCase(); }
};
// The abstract class that all tests inherit from.
//
// In Google Test, a unit test program contains one or many TestCases, and
// each TestCase contains one or many Tests.
//
// When you define a test using the TEST macro, you don't need to
// explicitly derive from Test - the TEST macro automatically does
// this for you.
//
// The only time you derive from Test is when defining a test fixture
// to be used a TEST_F.  For example:
//
//   class FooTest : public testing::Test {
//    protected:
//     virtual void SetUp() { ... }
//     virtual void TearDown() { ... }
//     ...
//   };
//
//   TEST_F(FooTest, Bar) { ... }
//   TEST_F(FooTest, Baz) { ... }
//
// Test is not copyable.
class Test
{
public:
    friend class TestInfo;

    // The d'tor is virtual as we intend to inherit from Test.
    virtual ~Test();

    // Sets up the stuff shared by all tests in this test case.
    //
    // Google Test will call Foo::SetUpTestCase() before running the first
    // test in test case Foo.  Hence a sub-class can define its own
    // SetUpTestCase() method to shadow the one defined in the super
    // class.
    static void SetUpTestCase() {}

    // Tears down the stuff shared by all tests in this test case.
    //
    // Google Test will call Foo::TearDownTestCase() after running the last
    // test in test case Foo.  Hence a sub-class can define its own
    // TearDownTestCase() method to shadow the one defined in the super
    // class.
    static void TearDownTestCase() {}

    // Returns true iff the current test has a fatal failure.
    static bool HasFatalFailure();

    // Returns true iff the current test has a non-fatal failure.
    static bool HasNonfatalFailure();

    // Returns true iff the current test has a (either fatal or
    // non-fatal) failure.
    static bool HasFailure() { return HasFatalFailure() || HasNonfatalFailure(); }

protected:
    template <typename> friend class TestInfoImpl;

    // Creates a Test object.
    Test();

    // Sets up the test fixture.
    virtual void SetUp() {}

    // Tears down the test fixture.
    virtual void TearDown() {}

private:
    // Runs the test after the test fixture has been set up.
    //
    // A sub-class must implement this to define the test logic.
    //
    // DO NOT OVERRIDE THIS FUNCTION DIRECTLY IN A USER PROGRAM.
    // Instead, use the TEST or TEST_F macro.
    virtual void TestBody() = 0;

    // Often a user mis-spells SetUp() as Setup() and spends a long time
    // wondering why it is never called by Google Test.  The declaration of
    // the following method is solely for catching such an error at
    // compile time:
    //
    //   - The return type is deliberately chosen to be not void, so it
    //   will be a conflict if a user declares void Setup() in his test
    //   fixture.
    //
    //   - This method is private, so it will be another compiler error
    //   if a user calls it from his test fixture.
    //
    // DO NOT OVERRIDE THIS FUNCTION.
    //
    // If you see an error about overriding the following function or
    // about it being private, you have mis-spelled SetUp() as Setup().
    struct Setup_should_be_spelled_SetUp {
    };
    virtual Setup_should_be_spelled_SetUp* Setup() { return NULL; }

    // We disallow copying Tests.
    MINGTEST_DISALLOW_COPY_AND_ASSIGN_(Test);
};

///////////////////////////////////
// TESTING
///////////////////////////////////

namespace internal
{

// A macro for implementing the helper functions needed to implement
// ASSERT_EQ/NE and EXPECT_EQ/NE.  It is here just to avoid copy-and-paste
// of similar code.
//
// For each templatized helper function, we also define an overloaded
// version for BiggestInt in order to reduce code bloat and allow
// anonymous enums to be used with {ASSERT|EXPECT}_EQ/NE when compiled
// with gcc 4.
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
#define MINGTEST_IMPL_CMP_HELPER_(op_name, op)                                                                                                                                                                                                                                                                                                                                                                                              \
    \
template<typename T1, typename T2> \
AssertionResult CmpHelper##op_name(const char* expected_expression, const char* actual_expression,                                                                                                                                                                                                                                                                                                                                          \
                                   const T1& expected, const T2& actual)                                                                                                                                                                                                                                                                                                                                                                    \
    {                                                                                                                                                                                                                                                                                                                                                                                                                                       \
        if (expected op actual) {                                                                                                                                                                                                                                                                                                                                                                                                           \
            return AssertionSuccess();                                                                                                                                                                                                                                                                                                                                                                                                      \
        }                                                                                                                                                                                                                                                                                                                                                                                                                                   \
        return op_name##Failure(expected_expression, actual_expression, expected, actual);                                                                                                                                                                                                                                                                                                                                                  \
    \
}                                                                                                                                                                                                                                                                                                                                                                                                                                    \
    \
AssertionResult CmpHelper##op_name(const char* expected_expression, const char* actual_expression,                                                                                                                                                                                                                                                                                                                                          \
                                   BiggestInt expected, BiggestInt actual); \
\
/* The ??Helper is helper class for {ASSERT|EXPECT}_EQ/NE.  The template argument */ \
/* lhs_is_null_literal is true iff the first argument to ASSERT_EQ/NE() */ \
/* is a null pointer literal. The following default implementation is */ \
/* for lhs_is_null_literal being false. */                                                                                                                                                                                                                                                                                                                      \
    \
template<bool lhs_is_null_literal> \
class op_name##Helper{\
public : /* This templatized version is for the general case.*/                                                                                                                                                                                                                                                                                                                                                                             \
                          template <typename T1, typename T2> static AssertionResult Compare(                                                                                                                                                                                                                                                                                                                                               \
                              const char* expected_expression, const char* actual_expression,                                                                                                                                                                                                                                                                                                                                               \
                              const T1& expected, const T2& actual){return CmpHelper##op_name(                                                                                                                                                                                                                                                                                                                                              \
                              expected_expression, actual_expression, expected, actual);                                                                                                                                                                                                                                                                                                                                                    \
    }                                                                                                                                                                                                                                                                                                                                                                                                                                       \
                                                                                                                                                                                                                                                                                                                                                                                                                                            \
    /* With this overloaded version, we allow anonymous enums to be used */                                                                                                                                                                                                                                                                                                                                                                 \
    /* in {ASSERT|EXPECT}_EQ when compiled with gcc 4, as anonymous */                                                                                                                                                                                                                                                                                                                                                                      \
    /* enums can be implicitly cast to BiggestInt. */                                                                                                                                                                                                                                                                                                                                                                                       \
                                                                                                                                                                                                                                                                                                                                                                                                                                            \
    /* Even though its body looks the same as the above version, we */                                                                                                                                                                                                                                                                                                                                                                      \
    /* cannot merge the two, as it will make anonymous enums unhappy. */                                                                                                                                                                                                                                                                                                                                                                    \
    static AssertionResult Compare(const char* expected_expression, const char* actual_expression,                                                                                                                                                                                                                                                                                                                                          \
                                   BiggestInt expected, BiggestInt actual)                                                                                                                                                                                                                                                                                                                                                                  \
    {                                                                                                                                                                                                                                                                                                                                                                                                                                       \
        return CmpHelper##op_name(expected_expression, actual_expression, expected, actual);                                                                                                                                                                                                                                                                                                                                                \
    }                                                                                                                                                                                                                                                                                                                                                                                                                                       \
    \
}                                                                                                                                                                                                                                                                                                                                                                                                                                    \
    ; \
\
/* This specialization is used when the first argument to ASSERT_EQ/NE() */ \
/* is a null pointer literal, like NULL, false, or 0. */                                                                                                                                                                                                                                                                                                     \
    \
template<> \
class op_name##Helper<true>{\
public : /* We define two overloaded versions of Compare().  The first */ /* version will be                                                                                                                                                                                                                                                                                                                                                \
                                                                             picked when the                                                                                                                                                                                                                                                                                                                                                \
                                                                             second argument to                                                                                                                                                                                                                                                                                                                                             \
                                                                             ASSERT_EQ/NE() is */ /* NOT a pointer, e.g. ASSERT_EQ/NE(0, AnIntFunction()) or */ /* EXPECT_EQ/NE(false, a_bool). */                                                                                                                                                                                                                                          \
                                                                              template <                                                                                                                                                                                                                                                                                                                                                    \
                                                                                  typename T1,                                                                                                                                                                                                                                                                                                                                              \
                                                                                  typename T2>                                                                                                                                                                                                                                                                                                                                              \
                                                                              static AssertionResult Compare(                                                                                                                                                                                                                                                                                                                               \
                                                                                  const char*                                                                                                                                                                                                                                                                                                                                               \
                                                                                      expected_expression,                                                                                                                                                                                                                                                                                                                                  \
                                                                                  const char*                                                                                                                                                                                                                                                                                                                                               \
                                                                                      actual_expression,                                                                                                                                                                                                                                                                                                                                    \
                                                                                  const T1&                                                                                                                                                                                                                                                                                                                                                 \
                                                                                      expected,                                                                                                                                                                                                                                                                                                                                             \
                                                                                  const T2&                                                                                                                                                                                                                                                                                                                                                 \
                                                                                      actual, /* The                                                                                                                                                                                                                                                                                                                                        \
                                                                                                 following line prevents this overload from being considered if T2 */ /* is not a pointer type.  We need this because ASSERT_EQ(NULL, my_ptr) */ /* expands to Compare("", "", NULL, my_ptr), which requires a conversion */ /* to match the Secret* in the other overload, which would otherwise make */ /* this template match better. */ \
                                                                                  typename EnableIf<                                                                                                                                                                                                                                                                                                                                        \
                                                                                      !is_pointer<                                                                                                                                                                                                                                                                                                                                          \
                                                                                          T2>::                                                                                                                                                                                                                                                                                                                                             \
                                                                                          value>::                                                                                                                                                                                                                                                                                                                                          \
                                                                                      type* = 0){                                                                                                                                                                                                                                                                                                                                           \
                                                                                  return CmpHelper##op_name(                                                                                                                                                                                                                                                                                                                                \
                                                                                      expected_expression,                                                                                                                                                                                                                                                                                                                                  \
                                                                                      actual_expression,                                                                                                                                                                                                                                                                                                                                    \
                                                                                      expected,                                                                                                                                                                                                                                                                                                                                             \
                                                                                      actual);                                                                                                                                                                                                                                                                                                                                              \
    }                                                                                                                                                                                                                                                                                                                                                                                                                                       \
                                                                                                                                                                                                                                                                                                                                                                                                                                            \
    /* This version will be picked when the second argument to ASSERT_EQ/NE() is a */                                                                                                                                                                                                                                                                                                                                                       \
    /* pointer, e.g. ASSERT_EQ/NE(NULL, a_pointer). */                                                                                                                                                                                                                                                                                                                                                                                      \
    template <typename T>                                                                                                                                                                                                                                                                                                                                                                                                                   \
    static AssertionResult Compare(                                                                                                                                                                                                                                                                                                                                                                                                         \
        const char* expected_expression,                                                                                                                                                                                                                                                                                                                                                                                                    \
        const char* actual_expression, /* We used to have a second template parameter instead of                                                                                                                                                                                                                                                                                                                                            \
                                          Secret*.  That */ /* template parameter would deduce to                                                                                                                                                                                                                                                                                                                                           \
                                                               'long', making this a better match                                                                                                                                                                                                                                                                                                                                           \
                                                               */ /* than the first overload even                                                                                                                                                                                                                                                                                                                                           \
                                                                     without the first overload's                                                                                                                                                                                                                                                                                                                                           \
                                                                     EnableIf. */ /* Unfortunately,                                                                                                                                                                                                                                                                                                                                         \
                                                                                     gcc with                                                                                                                                                                                                                                                                                                                                               \
                                                                                     -Wconversion-null                                                                                                                                                                                                                                                                                                                                      \
                                                                                     warns when                                                                                                                                                                                                                                                                                                                                             \
                                                                                     "passing NULL                                                                                                                                                                                                                                                                                                                                          \
                                                                                     to */ /* non-pointer                                                                                                                                                                                                                                                                                                                                   \
                                                                                              argument"                                                                                                                                                                                                                                                                                                                                     \
                                                                                              (even                                                                                                                                                                                                                                                                                                                                         \
                                                                                              a                                                                                                                                                                                                                                                                                                                                             \
                                                                                              deduced                                                                                                                                                                                                                                                                                                                                       \
                                                                                              integral                                                                                                                                                                                                                                                                                                                                      \
                                                                                              argument),                                                                                                                                                                                                                                                                                                                                    \
                                                                                              so                                                                                                                                                                                                                                                                                                                                            \
                                                                                              the                                                                                                                                                                                                                                                                                                                                           \
                                                                                              old                                                                                                                                                                                                                                                                                                                                           \
                                                                                              */ /* implementation caused warnings in user code. */                                                                                                                                                                                                                                                                                         \
        Secret* /* expected (NULL) */,                                                                                                                                                                                                                                                                                                                                                                                                      \
        T* actual)                                                                                                                                                                                                                                                                                                                                                                                                                          \
    {                                                                                                                                                                                                                                                                                                                                                                                                                                       \
        /* We already know that 'expected' is a null pointer.*/                                                                                                                                                                                                                                                                                                                                                                             \
        return CmpHelper##op_name(expected_expression, actual_expression, static_cast<T*>(NULL),                                                                                                                                                                                                                                                                                                                                            \
                                  actual);                                                                                                                                                                                                                                                                                                                                                                                                  \
    }                                                                                                                                                                                                                                                                                                                                                                                                                                       \
    \
}                                                                                                                                                                                                                                                                                                                                                                                                                                    \
    ;

#ifdef _MSC_VER
#pragma warning(push)           // Saves the current warning state.
#pragma warning(disable : 4389) // Temporarily disables warning on
                                // signed/unsigned mismatch.
#endif

// Implements the helper function for {ASSERT|EXPECT}_EQ
MINGTEST_IMPL_CMP_HELPER_(EQ, == );
// Implements the helper function for {ASSERT|EXPECT}_NE
MINGTEST_IMPL_CMP_HELPER_(NE, != );

#ifdef _MSC_VER
#pragma warning(pop) // Restores the warning state.
#endif

#undef MINGTEST_IMPL_CMP_HELPER_

// A macro for implementing the helper functions needed to implement
// ASSERT_?? and EXPECT_??.  It is here just to avoid copy-and-paste
// of similar code.
//
// For each templatized helper function, we also define an overloaded
// version for BiggestInt in order to reduce code bloat and allow
// anonymous enums to be used with {ASSERT|EXPECT}_?? when compiled
// with gcc 4.
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
#define MINGTEST_IMPL_CMP_HELPER_(op_name, op)                                                     \
    \
template<typename T1, typename T2> \
AssertionResult CmpHelper##op_name(const char* expr1, const char* expr2, const T1& val1,           \
                                   const T2& val2)                                                 \
    {                                                                                              \
        if (val1 op val2) {                                                                        \
            return AssertionSuccess();                                                             \
        } else {                                                                                   \
            return AssertionFailure()                                                              \
                   << "Expected: (" << expr1 << ") " #op " (" << expr2                             \
                   << "), actual: " << FormatForComparisonFailureMessage(val1, val2) << " vs "     \
                   << FormatForComparisonFailureMessage(val2, val1);                               \
        }                                                                                          \
    \
}                                                                                           \
    \
AssertionResult CmpHelper##op_name(const char* expr1, const char* expr2, BiggestInt val1,          \
                                   BiggestInt val2)

// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.

// Implements the helper function for {ASSERT|EXPECT}_NE
// MINGTEST_IMPL_CMP_HELPER_(NE, != );
// Implements the helper function for {ASSERT|EXPECT}_LE
MINGTEST_IMPL_CMP_HELPER_(LE, <= );
// Implements the helper function for {ASSERT|EXPECT}_LT
MINGTEST_IMPL_CMP_HELPER_(LT, < );
// Implements the helper function for {ASSERT|EXPECT}_GE
MINGTEST_IMPL_CMP_HELPER_(GE, >= );
// Implements the helper function for {ASSERT|EXPECT}_GT
MINGTEST_IMPL_CMP_HELPER_(GT, > );

#undef MINGTEST_IMPL_CMP_HELPER_

// The helper function for {ASSERT|EXPECT}_STREQ.
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
AssertionResult CmpHelperSTREQ(const char* expected_expression, const char* actual_expression,
                               const char* expected, const char* actual);

// The helper function for {ASSERT|EXPECT}_STRCASEEQ.
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
AssertionResult CmpHelperSTRCASEEQ(const char* expected_expression, const char* actual_expression,
                                   const char* expected, const char* actual);

// The helper function for {ASSERT|EXPECT}_STRNE.
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
AssertionResult CmpHelperSTRNE(const char* s1_expression, const char* s2_expression, const char* s1,
                               const char* s2);

// The helper function for {ASSERT|EXPECT}_STRCASENE.
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
AssertionResult CmpHelperSTRCASENE(const char* s1_expression, const char* s2_expression,
                                   const char* s1, const char* s2);

// Helper template function for comparing floating-points.
//
// Template parameter:
//
//   RawType: the raw floating-point type (either float or double)
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
template <typename RawType>
AssertionResult CmpHelperFloatingPointEQ(const char* expected_expression,
                                         const char* actual_expression, RawType expected,
                                         RawType actual)
{
    const FloatingPoint<RawType> lhs(expected), rhs(actual);

    if (lhs.AlmostEquals(rhs)) {
        return AssertionSuccess();
    }

    return EQFailure(expected_expression, actual_expression, expected, actual);
}

// Helper function for implementing ASSERT_NEAR.
//
// INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
AssertionResult DoubleNearPredFormat(const char* expr1, const char* expr2,
                                     const char* abs_error_expr, double val1, double val2,
                                     double abs_error);

// INTERNAL IMPLEMENTATION - DO NOT USE IN USER CODE.
// A class that enables one to stream messages to assertion macros
class AssertHelper
{
public:
    // Constructor.
    AssertHelper(TestResult::PartResult type, const char* file, int line);
    ~AssertHelper();

    // Assertion result assignment is a semantic trick to enable assertion
    // streaming; see the GTEST_MESSAGE_ macro below.
    void operator=(const AssertionResult& ar) const;

private:
    MINGTEST_DISALLOW_COPY_AND_ASSIGN_(AssertHelper);

    TestResult::PartResult type_;
    const char* file_;
    int line_;
};

} // namespace internal

// Boolean assertions. Condition can be either a Boolean expression or an
// AssertionResult. For more information on how to use AssertionResult with
// these macros see comments on that class.
#define EXPECT_TRUE(condition)                                                                     \
    MINGTEST_TEST_BOOLEAN_(condition, #condition, false, true, MINGTEST_NONFATAL_FAILURE_)
#define EXPECT_FALSE(condition)                                                                    \
    MINGTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, MINGTEST_NONFATAL_FAILURE_)
#define ASSERT_TRUE(condition)                                                                     \
    MINGTEST_TEST_BOOLEAN_(condition, #condition, false, true, MINGTEST_FATAL_FAILURE_)
#define ASSERT_FALSE(condition)                                                                    \
    MINGTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, MINGTEST_FATAL_FAILURE_)

// Includes the auto-generated header that implements a family of
// generic predicate assertion macros.
#include "gtest/gtest_pred_impl.h"

// Macros for testing equalities and inequalities.
//
//    * {ASSERT|EXPECT}_EQ(expected, actual): Tests that expected == actual
//    * {ASSERT|EXPECT}_NE(v1, v2):           Tests that v1 != v2
//    * {ASSERT|EXPECT}_LT(v1, v2):           Tests that v1 < v2
//    * {ASSERT|EXPECT}_LE(v1, v2):           Tests that v1 <= v2
//    * {ASSERT|EXPECT}_GT(v1, v2):           Tests that v1 > v2
//    * {ASSERT|EXPECT}_GE(v1, v2):           Tests that v1 >= v2
//
// When they are not, Google Test prints both the tested expressions and
// their actual values.  The values must be compatible built-in types,
// or you will get a compiler error.  By "compatible" we mean that the
// values can be compared by the respective operator.
//
// Note:
//
//   1. It is possible to make a user-defined type work with
//   {ASSERT|EXPECT}_??(), but that requires overloading the
//   comparison operators and is thus discouraged by the Google C++
//   Usage Guide.  Therefore, you are advised to use the
//   {ASSERT|EXPECT}_TRUE() macro to assert that two objects are
//   equal.
//
//   2. The {ASSERT|EXPECT}_??() macros do pointer comparisons on
//   pointers (in particular, C strings).  Therefore, if you use it
//   with two C strings, you are testing how their locations in memory
//   are related, not how their content is related.  To compare two C
//   strings by content, use {ASSERT|EXPECT}_STR*().
//
//   3. {ASSERT|EXPECT}_EQ(expected, actual) is preferred to
//   {ASSERT|EXPECT}_TRUE(expected == actual), as the former tells you
//   what the actual value is when it fails, and similarly for the
//   other comparisons.
//
//   4. Do not depend on the order in which {ASSERT|EXPECT}_??()
//   evaluate their arguments, which is undefined.
//
//   5. These macros evaluate their arguments exactly once.
//
// Examples:
//
//   EXPECT_NE(5, Foo());
//   EXPECT_EQ(NULL, a_pointer);
//   ASSERT_LT(i, array_size);
//   ASSERT_GT(records.size(), 0) << "There is no record left.";

#define EXPECT_EQ(expected, actual)                                                                \
    EXPECT_PRED_FORMAT2(                                                                           \
        ::testing::internal::EQHelper<MINGTEST_IS_NULL_LITERAL_(expected)>::Compare, expected,     \
        actual)
#define EXPECT_NE(expected, actual)                                                                \
    EXPECT_PRED_FORMAT2(                                                                           \
        ::testing::internal::NEHelper<MINGTEST_IS_NULL_LITERAL_(expected)>::Compare, expected,     \
        actual)
#if 0
#define EXPECT_NE(expected, actual)                                                                \
    EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperNE, expected, actual)
#endif
#define EXPECT_LE(val1, val2) EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperLE, val1, val2)
#define EXPECT_LT(val1, val2) EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperLT, val1, val2)
#define EXPECT_GE(val1, val2) EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperGE, val1, val2)
#define EXPECT_GT(val1, val2) EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperGT, val1, val2)

#define MINGTEST_ASSERT_EQ(expected, actual)                                                       \
    ASSERT_PRED_FORMAT2(                                                                           \
        ::testing::internal::EQHelper<MINGTEST_IS_NULL_LITERAL_(expected)>::Compare, expected,     \
        actual)
#define MINGTEST_ASSERT_NE(expected, actual)                                                       \
    ASSERT_PRED_FORMAT2(                                                                           \
        ::testing::internal::NEHelper<MINGTEST_IS_NULL_LITERAL_(expected)>::Compare, expected,     \
        actual)
#if 0
#define MINGTEST_ASSERT_NE(val1, val2)                                                             \
    ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperNE, val1, val2)
#endif
#define MINGTEST_ASSERT_LE(val1, val2)                                                             \
    ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperLE, val1, val2)
#define MINGTEST_ASSERT_LT(val1, val2)                                                             \
    ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperLT, val1, val2)
#define MINGTEST_ASSERT_GE(val1, val2)                                                             \
    ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperGE, val1, val2)
#define MINGTEST_ASSERT_GT(val1, val2)                                                             \
    ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperGT, val1, val2)

// Define macro MINGTEST_DONT_DEFINE_ASSERT_XY to 1 to omit the definition of
// ASSERT_XY(), which clashes with some users' own code.

#define ASSERT_EQ(val1, val2) MINGTEST_ASSERT_EQ(val1, val2)

#define ASSERT_NE(val1, val2) MINGTEST_ASSERT_NE(val1, val2)

#define ASSERT_LE(val1, val2) MINGTEST_ASSERT_LE(val1, val2)

#define ASSERT_LT(val1, val2) MINGTEST_ASSERT_LT(val1, val2)

#define ASSERT_GE(val1, val2) MINGTEST_ASSERT_GE(val1, val2)

#define ASSERT_GT(val1, val2) MINGTEST_ASSERT_GT(val1, val2)

// C String Comparisons.  All tests treat NULL and any non-NULL string
// as different.  Two NULLs are equal.
//
//    * {ASSERT|EXPECT}_STREQ(s1, s2):     Tests that s1 == s2
//    * {ASSERT|EXPECT}_STRNE(s1, s2):     Tests that s1 != s2
//    * {ASSERT|EXPECT}_STRCASEEQ(s1, s2): Tests that s1 == s2, ignoring case
//    * {ASSERT|EXPECT}_STRCASENE(s1, s2): Tests that s1 != s2, ignoring case
//
// For wide or narrow string objects, you can use the
// {ASSERT|EXPECT}_??() macros.
//
// Don't depend on the order in which the arguments are evaluated,
// which is undefined.
//
// These macros evaluate their arguments exactly once.

#define EXPECT_STREQ(expected, actual)                                                             \
    \
EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperSTREQ, expected, actual)
#define EXPECT_STRNE(s1, s2)                                                                       \
    \
EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperSTRNE, s1, s2)
#define EXPECT_STRCASEEQ(expected, actual)                                                         \
    \
EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperSTRCASEEQ, expected, actual)
#define EXPECT_STRCASENE(s1, s2)                                                                   \
    \
EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperSTRCASENE, s1, s2)

#define ASSERT_STREQ(expected, actual)                                                             \
    \
ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperSTREQ, expected, actual)
#define ASSERT_STRNE(s1, s2)                                                                       \
    \
ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperSTRNE, s1, s2)
#define ASSERT_STRCASEEQ(expected, actual)                                                         \
    \
ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperSTRCASEEQ, expected, actual)
#define ASSERT_STRCASENE(s1, s2)                                                                   \
    ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperSTRCASENE, s1, s2)

// Macros for comparing floating-point numbers.
//
//    * {ASSERT|EXPECT}_FLOAT_EQ(expected, actual):
//         Tests that two float values are almost equal.
//    * {ASSERT|EXPECT}_DOUBLE_EQ(expected, actual):
//         Tests that two double values are almost equal.
//    * {ASSERT|EXPECT}_NEAR(v1, v2, abs_error):
//         Tests that v1 and v2 are within the given distance to each other.
//
// Google Test uses ULP-based comparison to automatically pick a default
// error bound that is appropriate for the operands.  See the
// FloatingPoint template class in gtest-internal.h if you are
// interested in the implementation details.

#define EXPECT_FLOAT_EQ(expected, actual)                                                          \
    EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<float>, \
expected,                                                                                          \
                        actual)

#define EXPECT_DOUBLE_EQ(expected, actual)                                                         \
    EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<double>, \
expected,                                                                                          \
                        actual)

#define ASSERT_FLOAT_EQ(expected, actual)                                                          \
    \
ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<float>, \
expected,                                                                                          \
                    actual)

#define ASSERT_DOUBLE_EQ(expected, actual)                                                         \
    \
ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<double>, \
expected,                                                                                          \
                    actual)

#define EXPECT_NEAR(val1, val2, abs_error)                                                         \
    \
EXPECT_PRED_FORMAT3(::testing::internal::DoubleNearPredFormat, \
val1,                                                                                              \
                    val2, abs_error)

#define ASSERT_NEAR(val1, val2, abs_error)                                                         \
    \
ASSERT_PRED_FORMAT3(::testing::internal::DoubleNearPredFormat, \
val1,                                                                                              \
                    val2, abs_error)

////////////////////////////////
// DEATH TESTS

// This macro is used for implementing macros such as
// EXPECT_DEATH_IF_SUPPORTED and ASSERT_DEATH_IF_SUPPORTED on systems where
// death tests are not supported. Those macros must compile on such systems
// iff EXPECT_DEATH and ASSERT_DEATH compile with the same parameters on
// systems that support death tests. This allows one to write such a macro
// on a system that does not support death tests and be sure that it will
// compile on a death-test supporting system.
//
// Parameters:
//   statement -  A statement that a macro such as EXPECT_DEATH would test
//                for program termination. This macro has to make sure this
//                statement is compiled but not executed, to ensure that
//                EXPECT_DEATH_IF_SUPPORTED compiles with a certain
//                parameter iff EXPECT_DEATH compiles with it.
//   regex     -  A regex that a macro such as EXPECT_DEATH would use to test
//                the output of statement.  This parameter has to be
//                compiled but not evaluated by this macro, to ensure that
//                this macro only accepts expressions that a macro such as
//                EXPECT_DEATH would accept.
//   terminator - Must be an empty statement for EXPECT_DEATH_IF_SUPPORTED
//                and a return statement for ASSERT_DEATH_IF_SUPPORTED.
//                This ensures that ASSERT_DEATH_IF_SUPPORTED will not
//                compile inside functions where ASSERT_DEATH doesn't
//                compile.
//
//  The branch that has an always false condition is used to ensure that
//  statement and regex are compiled (and thus syntactically correct) but
//  never executed. The unreachable code macro protects the terminator
//  statement from generating an 'unreachable code' warning in case
//  statement unconditionally returns or throws. The Message constructor at
//  the end allows the syntax of streaming additional messages into the
//  macro, for compilational compatibility with EXPECT_DEATH/ASSERT_DEATH.
#define MINGTEST_UNSUPPORTED_DEATH_TEST_(statement, regex, terminator)                             \
    MINGTEST_AMBIGUOUS_ELSE_BLOCKER_                                                               \
    if (true) {                                                                                    \
        ::testing::AssertionResult gtest_ar =                                                      \
            ::testing::AssertionFailure() << "Death tests are not supported on this platform.\r\n" \
                                             "Statement '" #statement "' cannot be verified.";     \
        MINGTEST_MESSAGE_(gtest_ar, ::testing::TestResult::SKIPPED);                               \
        terminator;                                                                                \
    }

#define EXPECT_DEATH_IF_SUPPORTED(statement, regex)                                                \
    MINGTEST_UNSUPPORTED_DEATH_TEST_(statement, regex, )
#define ASSERT_DEATH_IF_SUPPORTED(statement, regex)                                                \
    MINGTEST_UNSUPPORTED_DEATH_TEST_(statement, regex, return )

////////////////////////////////
// TEST RUNNER
////////////////////////////////

class UnitTest
{
public:
    static UnitTest* GetInstance();

    int Run();

    // void Report(const char* file, int line, const char* message, TestResult::PartResult
    // resultType);

    // Returns the TestResult for the test that's currently running, or
    // the TestResult for the ad hoc test if no test is running.
    TestResult* current_test_result();

private:
    UnitTest();
    ~UnitTest() {}

    // We disallow copying Tests.
    MINGTEST_DISALLOW_COPY_AND_ASSIGN_(UnitTest);

private:
    /** Current test result */
    TestResult* current_test_result_;
};

void InitGoogleTest(int* argc, char** argv);

#define RUN_ALL_TESTS() (::testing::UnitTest::GetInstance()->Run())

} // namespace testing

#endif // #ifndef MINIMALISTIC_GTEST_GTEST_H_
