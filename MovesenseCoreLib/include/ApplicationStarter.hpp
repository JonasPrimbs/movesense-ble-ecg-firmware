#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

/// Helper class for doing application initialization
///
/// @tparam TApplication       The concrete application class.
template<class TApplication>
class ApplicationStarter FINAL
{
public:
    /** Run application initialization. */
    ApplicationStarter() :
        mApplication()
    {
        mApplication.applicationContextInit();
    }

private:
    /// The concrete application instance.
    TApplication mApplication;
};
