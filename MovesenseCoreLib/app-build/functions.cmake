# Export the name of the variable as define set to one if the given variable
# evaluates true. otherwise there will not be an export
#
# TODO: Get rid of this, settings should go to a generated header file.
function(EXPORT_DEF name)
  #message("testing name=${name}, val=${${name}}")
  # evaluate the contents of the variable whose name is passed in 'name'
  if (${${name}})
    #message("emitting -DCMAKE_${name}=1")
    add_definitions(-DCMAKE_${name}=1)
  endif()
endfunction(EXPORT_DEF)

# Set up the custom commands to do the necessary extra configuration for
# cross-compiled binaries for the target hardware.
function(CONFIGURE_EXECUTABLE executable_name link_address linker_script_path)
    if(${BSP} MATCHES "nRF5x")
        ASSERT_DEFINED(LINKER_SCRIPTS_PATH)
        ASSERT_DEFINED(PATH_TOOL_PYTHON)

        if(${COMPILER} MATCHES "GCC")
            # Set link script
            set_target_properties(${executable_name} PROPERTIES
                LINK_FLAGS "-T${linker_script_path}"
                )
            set_property(TARGET ${executable_name} APPEND PROPERTY
                LINK_DEPENDS ${linker_script_path})
        else()
            message(FATAL_ERROR "Unsupported compiler for target platform")
        endif()

        set(PRODUCT_OUTPUT_CLEAN_FILES
            ${executable_name}.bin
            ${executable_name}.map
            ${executable_name}.mr.txt)
        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${ADDITIONAL_MAKE_CLEAN_FILES} "${PRODUCT_OUTPUT_CLEAN_FILES}")
    endif()

endfunction(CONFIGURE_EXECUTABLE)

# Add custom command to run C preprocessor.
function(add_c_preprocessor_command)
    # From: https://stackoverflow.com/questions/3562483/only-run-c-preprocessor-in-cmake#answer-66896673
    #
    # Arguments
    #   OUTPUT          output file
    #   SOURCE          input file
    #   TARGET          CMake target to inherit compile definitions, include directories, and compile options
    #   EXTRA_C_FLAGS   extra compiler flags added after all flags inherited from the TARGET

    set(one_value_args TARGET SOURCE OUTPUT)
    set(multi_value_args EXTRA_C_FLAGS)
    cmake_parse_arguments(CPRE "" "${one_value_args}" "${multi_value_args}" ${ARGN})

    string(TOUPPER ${CMAKE_BUILD_TYPE} build_type)
    string(REPLACE " " ";" c_flags "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${build_type}}")
    message("CPRE_SOURCE: ${CPRE_SOURCE}")
    add_custom_command(
        OUTPUT ${CPRE_OUTPUT}
        COMMAND ${CMAKE_C_COMPILER}
            ${CPRE_EXTRA_C_FLAGS}
            "-D$<JOIN:$<TARGET_PROPERTY:${CPRE_TARGET},COMPILE_DEFINITIONS>,;-D>"
            "-I$<JOIN:$<TARGET_PROPERTY:${CPRE_TARGET},INCLUDE_DIRECTORIES>,;-I>"
            -E -P ${CPRE_SOURCE} -o ${CPRE_OUTPUT}
        COMMAND_EXPAND_LISTS VERBATIM
        IMPLICIT_DEPENDS C ${CPRE_SOURCE}
        DEPENDS ${CPRE_SOURCE})

endfunction(add_c_preprocessor_command)

# Copy extra files needed by simulator to build directory.
function(INIT_SIMULATOR_ENVIRONMENT)
    if(${BSP} MATCHES "SDL")
        if (MSVC)
            if(VC_ARCH_X86)
                set(VS_BITNESS "x86")
            elseif(VC_ARCH_X64)
                set(VS_BITNESS "x64")
            else()
                message(FATAL_ERROR "Build system not supported")
            endif()

            file(GLOB VS_DLLS
                "${MOVESENSE_CORE_LIBRARY}/lib/VisualC/${HWCONFIG}/*.dll")
            file(GLOB VS_FONTS
                "${MOVESENSE_CORE_LIBRARY}/lib/VisualC/${HWCONFIG}/*.ttf")
            foreach(f ${VS_DLLS} ${VS_FONTS})
                # Visual studio builds into either Debug or Release folder,
                # copy the necessary files into both.
                file(COPY ${f} DESTINATION ${CMAKE_BINARY_DIR}/Debug)
                file(COPY ${f} DESTINATION ${CMAKE_BINARY_DIR}/Release)
            endforeach(f)
        endif()
    endif()
endfunction(INIT_SIMULATOR_ENVIRONMENT)

# Extract unique paths for files in file_list into paths_var
function(EXTRACT_PATHS paths_var)
    # Drop the bound first argument.
    list(REMOVE_AT ARGV 0)
    foreach(f ${ARGV})
        get_filename_component(dir ${f} ABSOLUTE)
        get_filename_component(dir ${dir} DIRECTORY)
        list(APPEND ${paths_var} ${dir})
    endforeach(f)
    list(REMOVE_DUPLICATES ${paths_var})
    set(${paths_var} ${${paths_var}} PARENT_SCOPE)
endfunction(EXTRACT_PATHS)

# Glob header files from paths of all the files in the input list into the
# target header list variables
function(GLOB_HEADERS headers_var)
    # Drop the bound first argument.
    list(REMOVE_AT ARGV 0)
    EXTRACT_PATHS(dirs ${ARGV})
    foreach(dir ${dirs})
        file(GLOB ${headers_var} "${dir}/*.h*")
    endforeach(dir)
    set(${headers_var} ${${headers_var}} PARENT_SCOPE)
endfunction(GLOB_HEADERS)


if(NOT COMMAND ASSERT_DEFINED)
    # Parts of the build process need to define symbols. This is used to check that
    # a specific symbol exists at a given point.
    function(ASSERT_DEFINED symbol)
        if(NOT DEFINED ${symbol})
            message(FATAL_ERROR "${symbol} is not defined")
        endif()
    endfunction(ASSERT_DEFINED)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../shared-build/functions.cmake)
