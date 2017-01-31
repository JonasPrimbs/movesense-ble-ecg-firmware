#*******************************************************************************
#	Copyright (c) Suunto Oy 2016.
#	All rights reserved.
#*******************************************************************************


################################
# Configuration
################################
#include(${CMAKE_CURRENT_LIST_DIR}/toolchain/misc/set-generated-paths.cmake)

################################
# Options
################################

#set(WB_UNITTEST_BUILD OFF CACHE BOOL "Enable this to link against Whiteboard library that has extended support for unit tests. This has a minor performance penalty and additional ROM usage.")

################################
# Headers
################################

# Add include directories
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Build nice file list for Visual Studio and 
set(MOVESENSE_INCLUDE_DIRECTORIES
    whiteboard
    whiteboard/builtinTypes
    whiteboard/containers
    whiteboard/metadata
    whiteboard/devicediscovery
    whiteboard/integration
    whiteboard/integration/bsp
    whiteboard/integration/bsp/android
    whiteboard/integration/bsp/external
    whiteboard/integration/bsp/ios
    whiteboard/integration/bsp/linux
    whiteboard/integration/bsp/macosx
    whiteboard/integration/bsp/nea
    whiteboard/integration/bsp/nrf
    whiteboard/integration/bsp/shared
    whiteboard/integration/bsp/windows
    whiteboard/integration/os
    whiteboard/integration/os/android
    whiteboard/integration/os/freertos
    whiteboard/integration/os/ios
    whiteboard/integration/os/linux
    whiteboard/integration/os/macosx
    whiteboard/integration/os/nea
    whiteboard/integration/os/windows
    whiteboard/integration/shared
    whiteboard/services
    whiteboard/unittest
)

foreach(SUBDIR ${WB_INCLUDE_DIRECTORIES})
  file(GLOB SUBDIR_HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/${SUBDIR}/*.h")
  string(REPLACE "/" "\\" SUBDIR_NAME ${SUBDIR})
  source_group("Whiteboard Files\\${SUBDIR_NAME}" FILES ${SUBDIR_HEADERS})
  set(WB_HEADERS ${WB_HEADERS} ${SUBDIR_HEADERS})
endforeach(SUBDIR)

################################
# Libraries
################################

add_library(movesense-core STATIC IMPORTED)
set_property(TARGET movesense-core PROPERTY IMPORTED_LOCATION_DEBUG          ${CMAKE_CURRENT_LIST_DIR}/lib/${COMPILER}/${CMAKE_STATIC_LIBRARY_PREFIX}movesense-coreD${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET movesense-core PROPERTY IMPORTED_LOCATION_MINSIZEREL     ${CMAKE_CURRENT_LIST_DIR}/lib/${COMPILER}/${CMAKE_STATIC_LIBRARY_PREFIX}movesense-coreS${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET movesense-core PROPERTY IMPORTED_LOCATION_RELWITHDEBINFO ${CMAKE_CURRENT_LIST_DIR}/lib/${COMPILER}/${CMAKE_STATIC_LIBRARY_PREFIX}movesense-coreRD${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET movesense-core PROPERTY IMPORTED_LOCATION_RELEASE        ${CMAKE_CURRENT_LIST_DIR}/lib/${COMPILER}/${CMAKE_STATIC_LIBRARY_PREFIX}movesense-core${CMAKE_STATIC_LIBRARY_SUFFIX})

include(${CMAKE_CURRENT_LIST_DIR}/tools/wbres/resources.cmake)
