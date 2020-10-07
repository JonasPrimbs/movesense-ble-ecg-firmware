cmake_minimum_required(VERSION 3.4)
enable_language(C CXX ASM)

if(NOT DEFINED MOVESENSE_CORE_LIBRARY)
    # Give error that user must provide  path to movescount-core library
    message(FATAL_ERROR "Path to movesense-core library not set. Add -DMOVESENSE_CORE_LIBRARY=<path_to_core_lib>  to cmake command line")
endif()

if(NOT IS_ABSOLUTE ${MOVESENSE_CORE_LIBRARY})
    set(MOVESENSE_CORE_LIBRARY ${CMAKE_BINARY_DIR}/${MOVESENSE_CORE_LIBRARY})
endif()

include(${MOVESENSE_CORE_LIBRARY}/MovesenseFromStaticLib.cmake REQUIRED)

