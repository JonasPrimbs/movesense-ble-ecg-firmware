#pragma once

/// C++11 override specifier, in portable way. Replace
/// all instances with just 'override' when all compilers support it.
#if defined(__IAR_SYSTEMS_ICC__)
#define OVERRIDE /**/
#define FINAL /**/
#define DELETED /**/
#else
#define OVERRIDE override
#define FINAL final
#define DELETED = delete
#endif
