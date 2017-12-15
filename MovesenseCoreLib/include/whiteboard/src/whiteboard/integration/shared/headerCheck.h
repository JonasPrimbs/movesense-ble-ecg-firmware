#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

/*
  A macro for making link-time checks that defines used in header files are defined the same
  way as in the code file. Use this macro in the global namespace of the header file.
 
  @param def Name of the define that should be checked

  Example (header file):
  @code

  WB_HEADER_CHECK_DEFINE(NUMBER_OF_OBJECTS);

  @endcode
*/
#define WB_HEADER_CHECK_DEFINE(def)     \
    namespace _ ## def { \
        extern WB_API uint8 WB_EVAL_AND_CONCATENATE(__header_check_failed_, def); \
        const int WB_ATTRIBUTE_UNUSED WB_EVAL_AND_CONCATENATE(header_checker_, __LINE__) = WB_EVAL_AND_CONCATENATE(__header_check_failed_, def); \
    }

/**
  Counterpart for WB_HEADER_CHECK_DEFINE macro. Use this macro in the global namespace of the code file.

  Example (code file):
  @code

  WB_HEADER_CHECK_CONFIRM(NUMBER_OF_OBJECTS);

  @endcode
*/
#define WB_HEADER_CHECK_CONFIRM(def) \
    namespace _ ## def { uint8 WB_ATTRIBUTE_UNUSED WB_EVAL_AND_CONCATENATE(__header_check_failed_, def); }
