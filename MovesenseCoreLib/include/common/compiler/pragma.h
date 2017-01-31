#ifndef PRAGMA_H_
#define PRAGMA_H_

/******************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

        $Id$

******************************************************************************/

/** @file pragma.h
        Pragma related macros.
*/

/** @def MS_PRAGMA(x)
        Insert a msc pragma in code.

        @param x A pragma parameter.

        Example:

        @code

        class TextAdapterFixture : public ::testing::Test
        {
        public:
                TextAdapterFixture() :
                MS_PRAGMA(warning(suppress : 4355))
                mFun(*this, &TextAdapterFixture::getValue), mValue(0)
                {
                }
        //...
        };

        @endcode
*/
#ifdef _MSC_VER
#define MS_PRAGMA(x) __pragma(x)
#else
#define MS_PRAGMA(x)
#endif

/** @def MS_WARNING_SUPPRESS(x)
        Suppress a msc warning.

        @param x A warning number.

        Example:

        @code

        class TextAdapterFixture : public ::testing::Test
        {
        public:
                TextAdapterFixture() :
                MS_WARNING_SUPPRESS(4355)
                mFun(*this, &TextAdapterFixture::getValue), mValue(0)
                {
                }
        //...
        };

        @endcode
*/
#ifdef _MSC_VER
#define MS_WARNING_SUPPRESS(x) __pragma(warning(suppress : x))
#else
#define MS_WARNING_SUPPRESS(x)
#endif

#endif // #ifndef PRAGMA_H_
