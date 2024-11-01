# Settings that are present in every Movesense build go here.

cmake_policy(SET CMP0005 NEW) # Preprocessor definition values are now escaped automatically.
cmake_policy(SET CMP0011 NEW) # Included scripts do automatic cmake_policy PUSH and POP.
cmake_policy(SET CMP0077 NEW) # option() honors normal variables

# TODO: This is from when you used to have several applications you could make
# from one build. The define is used in some places in code. Might just get rid
# of this.
if(NOT DEFINED APPLICATION)
    set(APPLICATION "MovesenseApplication")
endif()
add_definitions(-DCMAKE_APPLICATION="${APPLICATION}")

# Set default HWCONFIG if not yet set
if(NOT DEFINED HWCONFIG)
    if(COMPILER MATCHES "GCCARM")
        set(HWCONFIG "SS2" CACHE STRING "HWConfig")
    elseif(COMPILER MATCHES "VisualC")
        set(HWCONFIG "SS2_Simu" CACHE STRING "HWConfig")
    endif()
    message("Using default HWCONFIG: ${HWCONFIG}")
endif()

# Horrific hack until we get cstdint properly in
# (UINT32_MAX etc won't work without this)
add_definitions(-D__STDC_LIMIT_MACROS)

set(TOOLS_PATH ${MOVESENSE_CORE_LIBRARY}/tools)

# check if the host is windows
if (NOT CMAKE_HOST_WIN32)
    message(STATUS "PATH_TOOL_PYTHON will use python from shell PATH")
    set(PATH_TOOL_PYTHON python3)
else()
	# get winpython tool from movesense-device-lib downloads
	include(FetchContent)
	FetchContent_Declare(
		winpython
		URL "https://bitbucket.org/movesense/movesense-device-lib/downloads/winpython-3.9.8.0.7z"
	)
	FetchContent_MakeAvailable(winpython)
	
    message(STATUS "PATH_TOOL_PYTHON set to winpython (windows only) downloaded from bitbucket:")
    set(PATH_TOOL_PYTHON ${winpython_SOURCE_DIR}/python-3.9.8.amd64/python.exe)
	message(STATUS "PATH_TOOL_PYTHON: ${PATH_TOOL_PYTHON}")
endif()

# helper functions
include(${CMAKE_CURRENT_LIST_DIR}/functions.cmake)
