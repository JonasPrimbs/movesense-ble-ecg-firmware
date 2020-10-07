
# If the given relative or absolute path is .cpp or .c source, add the short
# basename define to the definitions for that file. Used below by the three
# redefined functions.
#
# In same cases, one might want to get the original __FILE__ names instead, and
# this is transparently handled by bsp/assert.h if TRACE_FILENAME is not defined
# In these cases, make sure that DISABLE_TRACE_FILENAME cmake variable is set to
# OFF in the build, which will disable the three re-definitions

if(NOT COMMAND STAMP_SOURCE_WITH_TRACE_FILENAME)
    function(STAMP_SOURCE_WITH_TRACE_FILENAME PATH)
        if(NOT DISABLE_TRACE_FILENAME)
            get_filename_component(_EXTENSION ${PATH} EXT)
            if (_EXTENSION MATCHES "\.(cpp|c)$")
                # isolate basename
                get_filename_component(_BASENAME ${PATH} NAME)
                # convert relative paths to absolute (if required) so that the
                # source property is applied with a path that is always correct
                # irrespective of the level where the path is first used (was
                # a problem with speedFusion at least)
                get_filename_component(_ABSOLUTE ${PATH} ABSOLUTE)
                set_source_files_properties(${_ABSOLUTE} PROPERTIES COMPILE_DEFINITIONS TRACE_FILENAME="${_BASENAME}")
            endif()
        endif()
    endfunction()
endif()

# NOTE: The following three redefinitions of built-in cmake functions are
#       required to pass the basename filename define when building so that the
#       basename is available (as TRACE_FILENAME) even with compilers that do
#       not supported basenaming the __FILE__ via command line switches (gcc).
#       Such redefinition is only supported once and since the functions are
#       already built-in, regular "double inclusion" protection cannot be used,
#       so empty functions are created (global namespace) as guards instead.
#       The function override mechanism is explained at least here:
#       * https://www.youtube.com/watch?v=bsXLMQ6WgIk&feature=youtu.be&t=504
#       However, googling for "cmake function override" will yield some other
#       hits and gotchas.

if(NOT COMMAND ADD_LIBRARY_REDEFINED)
    function(add_library TARGET)
        _add_library(${TARGET} ${ARGN} "")
        foreach(_PATH IN LISTS ARGN)
            STAMP_SOURCE_WITH_TRACE_FILENAME(${_PATH})
        endforeach()
    endfunction()
    function(ADD_LIBRARY_REDEFINED)
    endfunction()
endif()

if(NOT COMMAND TARGET_SOURCES_REDEFINED)
    function(target_sources TARGET)
        _target_sources(${TARGET} ${ARGN} "")
        foreach(_PATH IN LISTS ARGN)
            STAMP_SOURCE_WITH_TRACE_FILENAME(${_PATH})
        endforeach()
    endfunction()
    function(TARGET_SOURCES_REDEFINED)
    endfunction()
endif()

if(NOT COMMAND ADD_EXECUTABLE_REDEFINED)
    function(add_executable TARGET)
        _add_executable(${TARGET} ${ARGN} "")
        foreach(_PATH IN LISTS ARGN)
            STAMP_SOURCE_WITH_TRACE_FILENAME(${_PATH})
        endforeach()
    endfunction()
    function(ADD_EXECUTABLE_REDEFINED)
    endfunction()
endif()
