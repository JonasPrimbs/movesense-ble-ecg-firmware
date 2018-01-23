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

# WB requires Unit test flag on DEBUG builds, so set them here. Otherwise init fails with repeating onGetRequest call recursion
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWB_UNITTEST_BUILD")
set(CMAKE_CXX_FLAGS_DEBUGWITHOUTDEBINFO "${CMAKE_CXX_FLAGS_DEBUGWITHOUTDEBINFO} -DWB_UNITTEST_BUILD")


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


# Setup link check address for application
# NOTE: Ajdust this when changing the softdevice!
set(LINK_STARTAT "0x1f000")

################################
# Exported variables
################################

# Variable that contains the filename of the softdevice hex-file that is supposed to be used with this version of Movesense Core-lib 
set(MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE "s132_nrf52_4.0.5_softdevice.hex")