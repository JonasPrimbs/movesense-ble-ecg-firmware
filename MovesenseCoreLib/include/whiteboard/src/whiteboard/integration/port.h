#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#if defined(_MSC_VER)
  #if defined(WB_DECLARE_API_EXPORT)
    #define WB_API __declspec(dllexport)
    #define WB_INTERNAL
  #elif defined(WB_DECLARE_API_IMPORT)
    #define WB_API __declspec(dllimport)
    #define WB_INTERNAL
  #endif
#elif defined(__GNUC__) || defined(__clang__)
  #define WB_API __attribute__((visibility("default")))
  #define WB_INTERNAL __attribute__((visibility("hidden")))
#endif

#ifndef WB_API
  #define WB_API
  #define WB_INTERNAL
#endif

#include "whiteboard/integration/shared/types.h"
#include "whiteboard/integration/shared/infinite.h"

// Define this in your make files / build system
#include WB_BSP

#include "whiteboard/integration/bsp/bsp.h"

#include "whiteboard/integration/os/os.h"
