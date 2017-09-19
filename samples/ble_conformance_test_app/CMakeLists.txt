cmake_minimum_required(VERSION 3.4)
enable_language(C CXX ASM)

# Set the proper type of build and pull the necessary files 
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "The build type was set to release")
    file( COPY ${CMAKE_BINARY_DIR}/${MOVESENSE_CORE_LIBRARY}/release/ DESTINATION  ${CMAKE_BINARY_DIR}/${MOVESENSE_CORE_LIBRARY})
else()
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "The build type was set to debug")
    file( COPY ${CMAKE_BINARY_DIR}/${MOVESENSE_CORE_LIBRARY}/debug/ DESTINATION ${CMAKE_BINARY_DIR}/${MOVESENSE_CORE_LIBRARY})
endif()

include_directories(BEFORE .)

set(APPLICATION "MovesenseApplication")
add_definitions(-DAPP_SS2_APPLICATION=1)
set(BUILD_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../_build)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
    message(WARNING "Defaulting build type to 'Debug'")
endif()

set(OPTIMIZATIONS HIGH)

if(NOT DEFINED LINKER_SCRIPT_NAME)
    set(LINKER_SCRIPT_NAME appflash)
endif()

if(NOT DEFINED LINK_STARTAT)
    set(LINK_STARTAT ${BSP_LINK_STARTAT})
endif()

if(NOT DEFINED MOVESENSE_CORE_LIBRARY)
    # Give error that user must provide  path to movescount-core library
    message(FATAL_ERROR "Path to movesense-core library not set. Add -DMOVESENSE_CORE_LIBRARY=<path_to_core_lib>  to cmake command line")
endif()

if(NOT IS_ABSOLUTE ${MOVESENSE_CORE_LIBRARY})
    set(MOVESENSE_CORE_LIBRARY ${CMAKE_BINARY_DIR}/${MOVESENSE_CORE_LIBRARY})
endif()


include(${BUILD_CONFIG_PATH}/toolchain-setup.cmake)
include(${BUILD_CONFIG_PATH}/prolog.cmake)

# XXXX TODO: Since BSP is built as part of core-lib, the settings related to that should be published from the core lib if they really are needed...
include(${BUILD_CONFIG_PATH}/platform/${BSP}.cmake)
include(${BUILD_CONFIG_PATH}/compiler/${COMPILER}.cmake)

include(${MOVESENSE_CORE_LIBRARY}/MovesenseFromStaticLib.cmake REQUIRED)

set(EXECUTABLE_NAME Movesense)

set(PATH_GENERATED_ROOT ${CMAKE_CURRENT_BINARY_DIR}/generated)

include_directories(${MOVESENSE_CORE_LIBRARY}/include/whiteboard/src)
include_directories(${MOVESENSE_CORE_LIBRARY}/include/whiteboard)
include_directories(${MOVESENSE_CORE_LIBRARY}/include/movesense_core)
include_directories(${MOVESENSE_CORE_LIBRARY}/generated)
include_directories(${MOVESENSE_CORE_LIBRARY}/generated/wb-resources)
include_directories(${PATH_GENERATED_ROOT})

include_directories(${CMAKE_BINARY_DIR})


set(WB_DIRECTORY "${MOVESENSE_CORE_LIBRARY}/include/whiteboard")

# XXXX TODO:  Somewhere else! corelib cmake?
set(WB_BSP "nrf")
set(WB_PORT "freertos")

add_definitions(-DWB_BSP="bsp/${WB_BSP}/bsp.h")
add_definitions(-DWB_PORT="os/${WB_PORT}/port.h")


# Copy all wb-resource folders from movesense-core to app's build directory
#file(GLOB MOVESENSE_RESOURCE_FOLDERS "${MOVESENSE_CORE_LIBRARY}/generated/*")

#foreach(SUBDIR ${WB_INCLUDE_DIRECTORIES})
#    file(COPY ${SUBDIR} DESTINATION ${CMAKE_BINARY_DIR}/generated)
#endforeach()


# The first one contains real application yaml with execution context definitions
generate_wb_resources(
    app_execution_contexts app_execution_contexts_RESOURCE_SOURCES
    SOURCE_GROUP app_execution_contexts  ${CMAKE_CURRENT_SOURCE_DIR}/*_root.yaml
    GENERATE C CPP LIB
    CPP_DEPENDS wb-resources)

# find all yaml files
file(GLOB MOVESENSE_CORE_YAML_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/wbresources/*.yaml)

# Generate our own resources
if (MOVESENSE_CORE_YAML_FILES)
    set(APP_RESOURCES ${CMAKE_BINARY_DIR}/app-resources.wbo)
    generate_wb_resources(
        app-resources APP_RESOURCE_SOURCES
        INCLUDE_DIRECTORIES ${WB_DIRECTORY}/include/whiteboard/builtinTypes
        SOURCE_GROUP app ${CMAKE_CURRENT_SOURCE_DIR}/wbresources/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/*_root.yaml
        GENERATE CPP LIB
        CPP_DEPENDS wb-resources)
endif()

# find all wbo's in movesense-core
file(GLOB MOVESENSE_CORE_WBO_FILES  ${MOVESENSE_CORE_LIBRARY}/resources/*.wbo)

# Combine libraries
generate_wb_resources(
    app-metadata APP_METADATA_SOURCES
    SOURCE_GROUP none GENERATED ${MOVESENSE_CORE_WBO_FILES} ${APP_RESOURCES} ${CMAKE_BINARY_DIR}/app_execution_contexts.wbo
    GENERATE METADATA)

# Automatic SBEM code generation for whiteboard resources. Will add files to SOURCES
include(${BUILD_CONFIG_PATH}/sbem.cmake)

# Application sources in current folder (ss2_app)
aux_source_directory(. APP_SOURCES)
set(APP_SOURCES ${APP_SOURCES} ${SOURCES})
INIT_SIMULATOR_ENVIRONMENT()

# Create exe
add_executable(${EXECUTABLE_NAME} ${APP_SOURCES} ${APP_RESOURCE_SOURCES} ${APP_METADATA_SOURCES} ${WB_HEADERS})

CONFIGURE_EXECUTABLE(${EXECUTABLE_NAME} ${LINK_STARTAT} ${LINKER_SCRIPT_NAME})

# With C++11 support if available
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 11)

# Link with Movesense-core library
target_link_libraries(${EXECUTABLE_NAME} movesense-core ${LIBRARIES})


# Convert resulting ELF file to .hex file (for flashing)
if (${COMPILER} MATCHES "IAR")
    add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        POST_BUILD
        COMMAND ielftool.exe --ihex ${EXECUTABLE_NAME}.elf ${EXECUTABLE_NAME}.hex
        )
elseif(${COMPILER} STREQUAL "GCCARM")
    add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.hex
        )
endif()

# include code for command line flashing of SS2
include(${BUILD_CONFIG_PATH}/flashing.cmake)
