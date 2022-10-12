#*******************************************************************************
#	Copyright (c) Suunto Oy 2016.
#	All rights reserved.
#*******************************************************************************


################################
# Build Setup
################################

set(APPLICATION "MovesenseApplication")

set(EXECUTABLE_NAME Movesense)
add_definitions(-DAPP_SS2_APPLICATION=1)
set(BUILD_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/app-build)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message(WARNING "Defaulting build type to 'Debug'")
endif()

set(OPTIMIZATIONS HIGH)

if(NOT DEFINED LINKER_SCRIPT_NAME)
    set(LINKER_SCRIPT_NAME appflash)
endif()


include(${BUILD_CONFIG_PATH}/toolchain-setup.cmake)
include(${BUILD_CONFIG_PATH}/prolog.cmake)

include(${BUILD_CONFIG_PATH}/platform/${BSP}.cmake)
include(${BUILD_CONFIG_PATH}/compiler/${COMPILER}.cmake)


set(PATH_GENERATED_ROOT ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(WB_DIRECTORY "${MOVESENSE_CORE_LIBRARY}/include/whiteboard")

# Simulator config
INIT_SIMULATOR_ENVIRONMENT()



#include(${CMAKE_CURRENT_LIST_DIR}/toolchain/misc/set-generated-paths.cmake)

################################
# Options
################################

#set(WB_UNITTEST_BUILD OFF CACHE BOOL "Enable this to link against Whiteboard library that has extended support for unit tests. This has a minor performance penalty and additional ROM usage.")

################################
# Headers
################################

# Add include directories
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

include_directories(${MOVESENSE_CORE_LIBRARY}/include/whiteboard/src)
include_directories(${MOVESENSE_CORE_LIBRARY}/include/whiteboard)
include_directories(${MOVESENSE_CORE_LIBRARY}/include/movesense_core)
include_directories(${MOVESENSE_CORE_LIBRARY}/generated/${COMPILER}/${CMAKE_BUILD_TYPE})
include_directories(${MOVESENSE_CORE_LIBRARY}/generated/${COMPILER}/${CMAKE_BUILD_TYPE}/wb-resources)
include_directories(${MOVESENSE_CORE_LIBRARY}/generated)
include_directories(${MOVESENSE_CORE_LIBRARY}/generated/wb-resources)
include_directories(${PATH_GENERATED_ROOT})

include_directories(${CMAKE_BINARY_DIR})



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

# Set up WB_BSP & WB_PORT
set(WB_BSP nea CACHE STRING "WB_BSP")
set(WB_PORT nea CACHE STRING "WB_PORT")

add_definitions(-DWB_BSP="bsp/${WB_BSP}/bsp.h")
add_definitions(-DWB_PORT="os/${WB_PORT}/port.h")

add_definitions(-DWB_HAVE_UNKNOWN_STRUCTURES=1)

# WB requires Unit test flag on DEBUG builds, so set them here. Otherwise init fails with repeating onGetRequest call recursion
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWB_UNITTEST_BUILD")
set(CMAKE_CXX_FLAGS_DEBUGWITHOUTDEBINFO "${CMAKE_CXX_FLAGS_DEBUGWITHOUTDEBINFO} -DWB_UNITTEST_BUILD")

# Logbook etc require WB_HAVE_DEPRECATED_BYTE_STREAM
add_definitions(-DWB_HAVE_DEPRECATED_BYTE_STREAM=1)

# Whiteboard timers are enabled by default (TODO: error caused if the static lib does not include the feature)
if(NOT DEFINED WB_HAVE_TIMERS)
    set(WB_HAVE_TIMERS true)
    add_definitions(-DWB_HAVE_TIMERS=1)
endif()

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
set_property(TARGET movesense-core PROPERTY IMPORTED_LOCATION_DEBUG          ${CMAKE_CURRENT_LIST_DIR}/lib/${COMPILER}/${HWCONFIG}/${CMAKE_STATIC_LIBRARY_PREFIX}movesense-coreD${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET movesense-core PROPERTY IMPORTED_LOCATION_MINSIZEREL     ${CMAKE_CURRENT_LIST_DIR}/lib/${COMPILER}/${HWCONFIG}/${CMAKE_STATIC_LIBRARY_PREFIX}movesense-coreS${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET movesense-core PROPERTY IMPORTED_LOCATION_RELWITHDEBINFO ${CMAKE_CURRENT_LIST_DIR}/lib/${COMPILER}/${HWCONFIG}/${CMAKE_STATIC_LIBRARY_PREFIX}movesense-coreRD${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET movesense-core PROPERTY IMPORTED_LOCATION_RELEASE        ${CMAKE_CURRENT_LIST_DIR}/lib/${COMPILER}/${HWCONFIG}/${CMAKE_STATIC_LIBRARY_PREFIX}movesense-core${CMAKE_STATIC_LIBRARY_SUFFIX})

include(${CMAKE_CURRENT_LIST_DIR}/tools/wbres/resources.cmake)


# Setup link check address for application
# NOTE: Ajdust this when changing the softdevice!
set(LINK_STARTAT "0x26000")

################################
# Exported variables
################################

# Variable that contains the filename of the softdevice hex-file that is supposed to be used with this version of Movesense Core-lib 
set(MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE "s132_nrf52_6.1.1_softdevice.hex")

####################################
# Prepare external modules
####################################
include(${BUILD_CONFIG_PATH}/modules.cmake)


################################
# Compile WB yaml files
################################

# The first one contains real application yaml with execution context definitions
#generate_wb_resources(
#    app_execution_contexts app_execution_contexts_RESOURCE_SOURCES
#    SOURCE_GROUP app_execution_contexts  ${CMAKE_CURRENT_SOURCE_DIR}/*_root.yaml
#    GENERATE C CPP LIB
#    CPP_DEPENDS wb-resources)

# find all yaml files and collect them
file(GLOB APP_YAML_FILES_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/*.yaml)
unset(APP_YAML_FILES_WB_RSC)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/wbresources)
    file(GLOB APP_YAML_FILES_WB_RSC ${CMAKE_CURRENT_SOURCE_DIR}/wbresources/*.yaml)
endif()
unset(APP_CUSTOM_PATH_YAML_FILES)
# allow app developer to specify additional directories relative to module directory
foreach(APP_CUSTOM_YAML_PATH ${CUSTOM_YAML_PATHS})
    unset(YAML_FILES_TMP)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${APP_CUSTOM_YAML_PATH})
        file(GLOB YAML_FILES_TMP ${CMAKE_CURRENT_SOURCE_DIR}/${APP_CUSTOM_YAML_PATH}/*.yaml)
    else()
        message("Warning, specified custom yaml directory not found: " ${CMAKE_CURRENT_SOURCE_DIR}/${APP_CUSTOM_YAML_PATH})
    endif()
    set(APP_CUSTOM_PATH_YAML_FILES ${APP_CUSTOM_PATH_YAML_FILES} ${YAML_FILES_TMP})
endforeach(APP_CUSTOM_YAML_PATH)

set(MOVESENSE_APP_YAML_FILES ${APP_YAML_FILES_MAIN} ${APP_YAML_FILES_WB_RSC} ${APP_CUSTOM_PATH_YAML_FILES})

# Generate application resources
if(MOVESENSE_APP_YAML_FILES)
    set(APP_RESOURCES ${CMAKE_BINARY_DIR}/app-resources.wbo)
    generate_wb_resources(
        app-resources APP_RESOURCE_SOURCES
        INCLUDE_DIRECTORIES ${MOVESENSE_CORE_LIBRARY}/resources/whiteboard/builtinTypes
        INCLUDE_DIRECTORIES ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api
        INCLUDE_DIRECTORIES ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/comm
        INCLUDE_DIRECTORIES ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/component
        INCLUDE_DIRECTORIES ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/meas
        INCLUDE_DIRECTORIES ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/mem
        INCLUDE_DIRECTORIES ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/misc
        INCLUDE_DIRECTORIES ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/system
        INCLUDE_DIRECTORIES ${MOVESENSE_CORE_LIBRARY}/resources/movesense-api/ui
        SOURCE_GROUP app ${MOVESENSE_APP_YAML_FILES}
        GENERATE C CPP LIB
        CPP_DEPENDS wb-resources)
endif()

# find all wbo's in movesense-core
file(GLOB MOVESENSE_CORE_WBO_FILES  ${MOVESENSE_CORE_LIBRARY}/resources/${COMPILER}/${CMAKE_BUILD_TYPE}/*.wbo)
list(FILTER MOVESENSE_CORE_WBO_FILES EXCLUDE REGEX ".+\\.[dr]\\.wbo$")

# add variant wbo
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(/DRELEASE)
    file(GLOB MOVESENSE_CORE_VARIANT_WBO_FILES  ${MOVESENSE_CORE_LIBRARY}/resources/${COMPILER}/${CMAKE_BUILD_TYPE}/*.r.wbo)
else()
    add_definitions(/DDEBUG)
    file(GLOB MOVESENSE_CORE_VARIANT_WBO_FILES  ${MOVESENSE_CORE_LIBRARY}/resources/${COMPILER}/${CMAKE_BUILD_TYPE}/*.d.wbo)
endif()

list(APPEND MOVESENSE_CORE_WBO_FILES ${MOVESENSE_CORE_VARIANT_WBO_FILES})

# Combine WB resource libraries from core, app and modules
generate_wb_resources(
    app-metadata APP_METADATA_SOURCES
    SOURCE_GROUP none GENERATED ${MOVESENSE_CORE_WBO_FILES} ${APP_RESOURCES} ${MODULES_RESOURCES}
    GENERATE METADATA)

####################################
# Generate SBEM serialization code
####################################

# Automatic SBEM code generation for whiteboard resources. Will add files to SOURCES
include(${BUILD_CONFIG_PATH}/sbem.cmake)


####################################
# Source files
####################################

# Application sources in current folder (app)
aux_source_directory(. APP_SOURCES)
set(APP_SOURCES ${APP_SOURCES} ${SOURCES})


####################################
# Source files
####################################

# Create exe
add_executable(${EXECUTABLE_NAME} ${APP_SOURCES} ${MODULES_SOURCES} ${APP_RESOURCE_SOURCES} ${MODULES_RESOURCE_SOURCES} ${APP_METADATA_SOURCES} ${WB_HEADERS})
CONFIGURE_EXECUTABLE(${EXECUTABLE_NAME} ${LINK_STARTAT} ${LINKER_SCRIPT_NAME})

# With C++11 support if available
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 11)

# Link with Movesense-core library
target_link_libraries(${EXECUTABLE_NAME} movesense-core ${LIBRARIES})


# Link with possible module libraries (exported from modules.cmake)
target_link_libraries(${EXECUTABLE_NAME} ${MODULE_LIBRARIES})


# include code for command line flashing of SS2
include(${BUILD_CONFIG_PATH}/flashing.cmake)
