#pragma once

// Detect whether doing a debug or release version. With msbuild, we want to use
// the per build setting, while on others we'll want to obey the
// CMAKE_BUILD_TYPE setting (indirectly)
#if defined(_MSC_VER)
// If _DEBUG is set with MSC, then debugs are enabled (don't use NDEBUG,
// which is used to control asserts and is portable).
#if defined(_DEBUG)
#ifndef BUILD_WITH_DEBUG
#define BUILD_WITH_DEBUG
#endif
#elif defined(BUILD_WITH_DEBUG)
// Undo the define from cmake
#undef BUILD_WITH_DEBUG
#endif
#else
// BUILD_WITH_DEBUG set by cmake is obeyed on other build systems
#endif
