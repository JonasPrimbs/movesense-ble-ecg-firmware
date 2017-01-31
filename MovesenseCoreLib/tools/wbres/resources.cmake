#*******************************************************************************
#
#	Copyright (c) Suunto Oy 2015.
#	All rights reserved.
#
#*******************************************************************************

cmake_minimum_required(VERSION 3.6)

# Determine wbres tool location
if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
  set(WBRES_TOOL "${CMAKE_CURRENT_LIST_DIR}/bin/win32/wbres.exe")
elseif ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Darwin")
  set(WBRES_TOOL "${CMAKE_CURRENT_LIST_DIR}/bin/macos/wbres")
elseif ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
  set(WBRES_TOOL "${CMAKE_CURRENT_LIST_DIR}/bin/linux/wbres")
else()
  message(FATAL_ERROR "No support for host OS '${CMAKE_HOST_SYSTEM_NAME}'.")
endif()

# Hard coded in wbres.exe
list(APPEND WBRES_INTERNAL_INPUTS "${CMAKE_CURRENT_LIST_DIR}/resources/headerDefinitions.h")
list(APPEND WBRES_INTERNAL_INPUTS "${CMAKE_CURRENT_LIST_DIR}/resources/sourceDefinitions.cpp")
list(APPEND WBRES_INTERNAL_INPUTS "${CMAKE_CURRENT_LIST_DIR}/resources/metadataSourceDefinitions.cpp")

# Add commands to generate Whiteboard resources
#
# generate_wb_resources(
#   Name OutputVar
#   [INCLUDE_DIRECTORIES [dir1 ...]]
#   [[SOURCE_GROUP Name [source1 ...] ...]
#   GENERATE [C] [CPP] [LIB] [METADATA]
#   [CPP_DEPENDS [dependency1 ...]]
#   [EXPORT_SYMBOL SymbolName])
#
#   Name specifies unique name for the generated output. This name is
#   used in constructing output file names. OutputVar specifies name 
#   of the variable that will receive list of build outputs and list
#   of all used input files. Use this with add_executable or add_library.
#   Use optional INCLUDE_DIRECTORIES to specify list of directories
#   that are searched while doing yaml reference resolution. Then
#   specify one or more SOURCE_GROUPs. Name specifies name of the 
#   filter for Visual Studio builds and can contain '/' characters
#   that delimit subfilters from their parents. Sources are either
#   Whiteboard YAML files or prebuilt WBO library files.
#   With GENERATE you can specify which output files are genered. When 
#   C is specified a C header with all resource identifiers is generated.
#   When CPP is specified a full header with all IDs and helper classes 
#   are generated. When LIB is specified resource metadata is saved in
#   internal storage format that allows later combination with other 
#   resources and libraries as well as metadata generation. Use METADATA
#   to generate metadata for the application. Note that currently there
#   can be only one metadata build step per application.
#   With CPP_DEPENDS you can specify a list of additional headers that
#   are required for building CPP generated headers. EXPORT_SYMBOL defines
#   name of preprocessor symbol that should be used to export symbols from
#   dynamically linked libraries or shared libraries. 
function (generate_wb_resources Name OutputVar)
  set(WBRES_OUTPUT_HEADER ${PATH_GENERATED_ROOT}/${Name}/resources.h)
  set(WBRES_OUTPUT_CHEADER ${PATH_GENERATED_ROOT}/${Name}/resources.c.h)
  set(WBRES_OUTPUT_SOURCE ${PATH_GENERATED_ROOT}/${Name}/resources.cpp)
  set(WBRES_OUTPUT_LIB ${CMAKE_BINARY_DIR}/${Name}.wbo)
  set(WBRES_OUTPUT_METADATA ${CMAKE_BINARY_DIR}/${Name}/metadata.cpp)
  
  set(INCLUDE_DIRECTORIES)
  set(SOURCE_GROUP_NAME)
  set(SOURCE_GROUP_SOURCES)
  set(STATE 0)
  set(INPUTS)
  set(SPECIFICATION_INPUTS)
  set(OUTPUTS)
  set(HIDDEN_OUTPUTS)
  set(CMDLINE)
  
  macro(_add_wb_resources_source_group)
    if (NOT SOURCE_GROUP_NAME STREQUAL "")
      # Get files
      set(SOURCE_GROUP_FILES)
      set(GENERATED 0)
      foreach(file_pattern ${SOURCE_GROUP_SOURCES})
        if ("${file_pattern}" STREQUAL "GENERATED")
          set(GENERATED 1)
          continue()
        elseif("${file_pattern}" STREQUAL "PATTERN")
          set(GENERATED 0)
          continue()
        endif()
        
        if (GENERATED EQUAL 0)
          file(GLOB _group_files ${file_pattern})
          list(APPEND SOURCE_GROUP_FILES ${_group_files})
        else()
          list(APPEND SOURCE_GROUP_FILES ${file_pattern})
        endif()
      endforeach()
      
      # Create source group
      string(CONCAT SRC_GROUP "Specification Files\\" ${SOURCE_GROUP_NAME})
      source_group(${SRC_GROUP} FILES ${SOURCE_GROUP_FILES})

      list(APPEND INPUTS ${SOURCE_GROUP_FILES})
    endif()
    set(SOURCE_GROUP_NAME)
    set(SOURCE_GROUP_SOURCES)    
  endmacro()
  
  # Parse arguments
  foreach(arg ${ARGN})
    # Check for keywords
    if ("${arg}" STREQUAL "INCLUDE_DIRECTORIES")
      set(STATE 1)
      _add_wb_resources_source_group(SOURCE_GROUP_NAME SOURCE_GROUP_SOURCES)
      continue()
    elseif ("${arg}" STREQUAL "SOURCE_GROUP")
      set(STATE 2)
      _add_wb_resources_source_group(SOURCE_GROUP_NAME SOURCE_GROUP_SOURCES)
      continue()
    elseif ("${arg}" STREQUAL "GENERATE")
      set(STATE 4)
      _add_wb_resources_source_group(SOURCE_GROUP_NAME SOURCE_GROUP_SOURCES)
      continue()
    elseif ("${arg}" STREQUAL "CPP_DEPENDS")
      set(STATE 5)
      _add_wb_resources_source_group(SOURCE_GROUP_NAME SOURCE_GROUP_SOURCES)
      continue()
    elseif ("${arg}" STREQUAL "EXPORT_SYMBOL")
      set(STATE 6)
      _add_wb_resources_source_group(SOURCE_GROUP_NAME SOURCE_GROUP_SOURCES)
      continue()
    endif()
    
    if (STATE EQUAL 0)
      message(FATAL_ERROR "Invalid parameters")
    elseif (STATE EQUAL 1)
      list(APPEND INCLUDE_DIRECTORIES ${arg})
      list(APPEND CMDLINE --includeDir ${arg})
    elseif (STATE EQUAL 2)
      set(SOURCE_GROUP_NAME ${arg})
      set(STATE 3)
    elseif (STATE EQUAL 3)
      list(APPEND SOURCE_GROUP_SOURCES ${arg})
    elseif (STATE EQUAL 4)
      if (${arg} STREQUAL "C")
        list(APPEND OUTPUTS ${WBRES_OUTPUT_CHEADER})
        list(APPEND CMDLINE --cheaderFile "${WBRES_OUTPUT_CHEADER}")
      elseif (${arg} STREQUAL "CPP")
        file(MAKE_DIRECTORY ${PATH_GENERATED_ROOT}/${Name})
        list(APPEND OUTPUTS ${WBRES_OUTPUT_HEADER})
        list(APPEND OUTPUTS ${WBRES_OUTPUT_SOURCE})
        list(APPEND CMDLINE --headerFile "${WBRES_OUTPUT_HEADER}" --sourceFile "${WBRES_OUTPUT_SOURCE}")
      elseif (${arg} STREQUAL "LIB")
        list(APPEND HIDDEN_OUTPUTS ${WBRES_OUTPUT_LIB})
        list(APPEND CMDLINE --libFile "${WBRES_OUTPUT_LIB}")
      elseif (${arg} STREQUAL "METADATA")
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${Name})
        list(APPEND OUTPUTS ${WBRES_OUTPUT_METADATA})
        list(APPEND CMDLINE --metadataSourceFile "${WBRES_OUTPUT_METADATA}")
      else()
        message(FATAL_ERROR "Invalid parameter '${arg}'")
      endif()
    elseif (STATE EQUAL 5)
      if (${arg} MATCHES "^.*\\..*$") # *.*
        list(APPEND CMDLINE --cppDepends ${arg})
      else()
        list(APPEND CMDLINE --cppDepends ../${arg}/resources.h)
      endif()
    elseif (STATE EQUAL 6)
      list(APPEND CMDLINE --exportSymbol ${arg})
    endif()
  endforeach()
  _add_wb_resources_source_group(SOURCE_GROUP_NAME SOURCE_GROUP_SOURCES)

  # Add command to generate outputs
  add_custom_command(
	OUTPUT ${OUTPUTS} ${HIDDEN_OUTPUTS}
	COMMAND "${WBRES_TOOL}" ${CMDLINE} ${INPUTS}
	DEPENDS ${INPUTS} "${WBRES_TOOL}" ${WBRES_INTERNAL_INPUTS}
    VERBATIM
  )

  source_group("Resources Files" FILES ${OUTPUTS})
  
  foreach(input ${INPUTS})
    if (NOT input MATCHES "^.*\\.[wW][bB][oO]")
      list(APPEND SPECIFICATION_INPUTS ${input})
    endif()
  endforeach()

  # Export results to parent scope
  set(${OutputVar} ${SPECIFICATION_INPUTS} ${OUTPUTS} PARENT_SCOPE)
endfunction()

