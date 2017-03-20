#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#if WB_UNITTEST_BUILD

namespace whiteboard
{

/**
 * RAII class for managing singleton instances of Whiteboard resource trees.
 *
 * Creating an instance of this class stores all resource tree
 * instance pointers and sets them to NULL. Deleting the instance
 * restores the instance pointers.  For most of test cases,
 * EmptyTestResourceScope does the same and initializes an empty set
 * of these. WbResTestResourceScope under unittestHelpers.h does the
 * same but leaves WBRES-generated objects the way they were.
 */
class TestResourceScope
{
public:
    virtual ~TestResourceScope();

protected:
    TestResourceScope();
};

/**
 * RAII class to create empty resource trees for running Whiteboard
 * tests and restoring the former state
 */
class EmptyTestResourceScope : private TestResourceScope
{
public:
    // The members are constructed only after TestResourceScope
    // constructor has saved the global instance pointers. Similarly,
    // its destructor restores the state after destructors for members
    // below have been run.
    EmptyTestResourceScope();
    virtual ~EmptyTestResourceScope();

private:
    void* const mpPrevResourceSubtree;
};

/**
* RAII class to create empty resource trees for running Nea
* tests and restoring the former state
*/
class WbResTestResourceScope : private TestResourceScope
{
public:
    // The members are constructed only after TestResourceScope
    // constructor has saved the global instance pointers. Similarly,
    // its destructor restores the state after destructors for members
    // below have been run.
    WbResTestResourceScope();
};

} // namespace whiteboard

#endif
