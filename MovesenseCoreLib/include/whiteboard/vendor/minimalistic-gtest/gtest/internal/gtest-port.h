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
// Authors: wan@google.com (Zhanyong Wan)
//
// The Google C++ Testing Framework (Google Test)

//
// Low-level types and utilities for porting Google Test to various
// platforms.  They are subject to change without notice.  DO NOT USE
// THEM IN USER CODE.

#ifndef MINGTEST_INCLUDE_MINGTEST_INTERNAL_MINGTEST_PORT_H_
#define MINGTEST_INCLUDE_MINGTEST_INTERNAL_MINGTEST_PORT_H_

// GTEST feature support

//   GTEST_HAS_COMBINE      - the Combine() function (for value-parameterized
//                            tests)
//   GTEST_HAS_DEATH_TEST   - death tests
//   GTEST_HAS_PARAM_TEST   - value-parameterized tests
//   GTEST_HAS_TYPED_TEST   - typed tests
//   GTEST_HAS_TYPED_TEST_P - type-parameterized tests
//   GTEST_USES_POSIX_RE    - enhanced POSIX regex is used. Do not confuse with
//                            GTEST_HAS_POSIX_RE (see above) which users can
//                            define themselves.
//   GTEST_USES_SIMPLE_RE   - our own simple regex is used;
//                            the above two are mutually exclusive.
//   GTEST_CAN_COMPARE_NULL - accepts untyped NULL in EXPECT_EQ().

#undef GTEST_HAS_COMBINE
#undef GTEST_HAS_DEATH_TEST
#undef GTEST_HAS_PARAM_TEST
#undef GTEST_HAS_TYPED_TEST
#undef GTEST_HAS_TYPED_TEST_P
#undef GTEST_USES_POSIX_RE
#undef GTEST_USES_SIMPLE_RE
#define GTEST_CAN_COMPARE_NULL 1

#include <stdlib.h>

// Defines some utility macros.

// The GNU compiler emits a warning if nested "if" statements are followed by
// an "else" statement and braces are not used to explicitly disambiguate the
// "else" binding.  This leads to problems with code like:
//
//   if (gate)
//     ASSERT_*(condition) << "Some message";
//
// The "switch (0) case 0:" idiom is used to suppress this.
#ifdef __INTEL_COMPILER
#define MINGTEST_AMBIGUOUS_ELSE_BLOCKER_
#else
#define MINGTEST_AMBIGUOUS_ELSE_BLOCKER_                                                           \
    switch (0)                                                                                     \
    case 0:                                                                                        \
    default: // NOLINT
#endif

// Use this annotation at the end of a struct/class definition to
// prevent the compiler from optimizing away instances that are never
// used.  This is useful when all interesting logic happens inside the
// c'tor and / or d'tor.  Example:
//
//   struct Foo {
//     Foo() { ... }
//   } MINGTEST_ATTRIBUTE_UNUSED_;
//
// Also use it after a variable or parameter declaration to tell the
// compiler the variable/parameter does not have to be used.
#if defined(__GNUC__) && !defined(COMPILER_ICC)
#define MINGTEST_ATTRIBUTE_UNUSED_ __attribute__((unused))
#else
#define MINGTEST_ATTRIBUTE_UNUSED_
#endif

// A macro to disallow operator=
// This should be used in the private: declarations for a class.
#define MINGTEST_DISALLOW_ASSIGN_(type) void operator=(type const&)

// A macro to disallow copy constructor and operator=
// This should be used in the private: declarations for a class.
#define MINGTEST_DISALLOW_COPY_AND_ASSIGN_(type)                                                   \
    type(type const&);                                                                             \
    MINGTEST_DISALLOW_ASSIGN_(type)

namespace testing
{
namespace internal
{

typedef long long BiggestInt;

template <bool bool_value> struct bool_constant {
    typedef bool_constant<bool_value> type;
    static const bool value = bool_value;
};
template <bool bool_value> const bool bool_constant<bool_value>::value;

typedef bool_constant<false> false_type;
typedef bool_constant<true> true_type;

template <typename T> struct is_pointer : public false_type {
};

template <typename T> struct is_pointer<T*> : public true_type {
};

// This template class serves as a compile-time function from size to
// type.  It maps a size in bytes to a primitive type with that
// size. e.g.
//
//   TypeWithSize<4>::UInt
//
// is typedef-ed to be unsigned int (unsigned integer made up of 4
// bytes).
//
// Such functionality should belong to STL, but I cannot find it
// there.
//
// Google Test uses this class in the implementation of floating-point
// comparison.
//
// For now it only handles UInt (unsigned int) as that's all Google Test
// needs.  Other types can be easily added in the future if need
// arises.
template <size_t size> class TypeWithSize
{
public:
    // This prevents the user from using TypeWithSize<N> with incorrect
    // values of N.
    typedef void UInt;
};

// The specialization for size 4.
template <> class TypeWithSize<4>
{
public:
    // unsigned int has size 4 in both gcc and MSVC.
    //
    // As base/basictypes.h doesn't compile on Windows, we cannot use
    // uint32, uint64, and etc here.
    typedef int Int;
    typedef unsigned int UInt;
};

// The specialization for size 8.
template <> class TypeWithSize<8>
{
public:
    typedef long long Int;           // NOLINT
    typedef unsigned long long UInt; // NOLINT
};

// Integer types of known sizes.
typedef TypeWithSize<4>::Int Int32;
typedef TypeWithSize<4>::UInt UInt32;
typedef TypeWithSize<8>::Int Int64;
typedef TypeWithSize<8>::UInt UInt64;

} // namespace internal
} // namespace testing

#endif // MINGTEST_INCLUDE_MINGTEST_INTERNAL_MINGTEST_PORT_H_
