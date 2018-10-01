# divert malloc and free
set(LINK_DIVERT_STD_MEMORY_API YES)


set(LINKER_SCRIPTS_PATH ${FW_PATH}/platform/bsp/${BSP}/linker/iar )

# Check that the relevant linker file exists
if(NOT EXISTS ${LINKER_SCRIPTS_PATH}/${BSP_KINETIS_LINKTO}.icf)
    message(FATAL_ERROR "BSP_KINETIS_LINKTO not chosen or invalid (cannot find ${LINKER_SCRIPTS_PATH}/${BSP_KINETIS_LINKTO}.icf or is not valid)")
endif()

# Helper function for converting argument list to string
function(APPEND_LIST_TO_STRING OUTPUT)
  set (_TMP_STR "${${OUTPUT}}")
  foreach(arg ${ARGN})
    string(CONCAT _TMP_STR "${_TMP_STR}" " ${arg}")
  endforeach()
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

set(COMMON_TARGET_SELECTOR
    --cpu Cortex-M4
    --thumb
)

# keep the following symbols
set(LINK_KEEP_SYMBOLS
    HardFault_Handler
)

if (IAR_DLIB_DISABLE_FP)
    message(" * IAR_DLIB_DISABLE_FP set, DLIB will not have floating point support")
else()
    APPEND_LIST_TO_STRING(COMMON_TARGET_SELECTOR --fpu VFPv4_sp)
endif()


if( NOT ("${WB_HARDWARE}" STREQUAL "nRF52.Cortex.M4.fp") )
    set(COMMON_BUILD_FLAGS
        ${COMMON_TARGET_SELECTOR}
        --warnings_are_errors
        --silent
        --require_prototypes
        --dlib_config=Normal)
else()
    # NOTE: can't use --require_prototypes
    set(COMMON_BUILD_FLAGS
        ${COMMON_TARGET_SELECTOR}
#        --warnings_are_errors
        --silent
        --dlib_config=Normal)
endif()

# Heap tracing needs full file names. Othwerwise we strip paths from __FILE__ and __BASE_FILE__ macros.
if (NOT NEA_HAVE_HEAP_TRACE)
    set(COMMON_BUILD_FLAGS ${COMMON_BUILD_FLAGS} --no_path_in_file_macros)
endif()

if (IAR_DLIB_DISABLE_FILE)
    message(" * IAR_DLIB_DISABLE_FILE active, DLIB will not have FILE* functions")
else()
    # default is to have the functions (no stdout otherwise)
    APPEND_LIST_TO_STRING(COMMON_BUILD_FLAGS -D_DLIB_FILE_DESCRIPTOR)
endif()

APPEND_LIST_TO_STRING(CMAKE_C_FLAGS
    ${COMMON_BUILD_FLAGS}
    -e
)

APPEND_LIST_TO_STRING(CMAKE_CXX_FLAGS
    ${COMMON_BUILD_FLAGS}
    --diag_suppress=Pa050
    -e
    -Dstd=""
)

APPEND_LIST_TO_STRING(CMAKE_ASM_FLAGS
    ${COMMON_TARGET_SELECTOR}
)

# Optimization flags
if (NOT DEFINED OPTIMIZATIONS)
  if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(OPTIMIZATIONS HIGH)
  else()
    set(OPTIMIZATIONS LOW)
  endif()
endif()

if (OPTIMIZATIONS STREQUAL "HIGH")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ohz")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ohz")
elseif (OPTIMIZATIONS STREQUAL "MEDIUM")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Om")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Om")
elseif (OPTIMIZATIONS STREQUAL "LOW")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ol")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ol")
elseif (OPTIMIZATIONS STREQUAL "NONE")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -On")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -On")
else()
  message(FATAL_ERROR "Unsupported optimization level '${OPTIMIZATIONS}'")
endif()

message(STATUS "Optimizations set to ${OPTIMIZATIONS}")

# Debug information always enabled, because this doesn't affect size of output binary
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --debug")

# Setup the symbol keeping magic
foreach(KEEP_SYMBOL_NAME ${LINK_KEEP_SYMBOLS})
  APPEND_LIST_TO_STRING(CMAKE_EXE_LINKER_FLAGS --keep ${KEEP_SYMBOL_NAME})
endforeach(KEEP_SYMBOL_NAME)

if(LINK_DIVERT_STD_MEMORY_API)
    message("I(IAR.cmake): diverting malloc and free")
    APPEND_LIST_TO_STRING(CMAKE_EXE_LINKER_FLAGS
        --redirect malloc=UNIMPLEMENTED_memalloc
        --redirect free=UNIMPLEMENTED_memfree
    )
endif()

# Dependency file generation
set(CMAKE_DEPFILE_FLAGS_C "--dependencies=ns <DEPFILE>")
set(CMAKE_DEPFILE_FLAGS_CXX "--dependencies=ns <DEPFILE>")

APPEND_LIST_TO_STRING(CMAKE_EXE_LINKER_FLAGS
    --cpu=Cortex-M4
    --semihosting
    --entry __iar_program_start
    --vfe
    --no_exceptions
    --map ${CMAKE_CURRENT_BINARY_DIR}
    --config_def __heap_size__=0x0
    --config_def __stack_size__=${IAR_DLIB_STACK_SIZE}
)

if (NEA_LINK_SULO)
    # check that the binary exists
    set(PATH_SULO ${APP_PATH}/sulo/frozen/sulo.bin)
    if (NOT EXISTS ${PATH_SULO})
        message(FATAL_ERROR "Linking SuLo enabled (NEA_LINK_SULO), but cannot be found (${PATH_SULO}).")
    endif()
    APPEND_LIST_TO_STRING(CMAKE_EXE_LINKER_FLAGS
        --image_input ${PATH_SULO},SULO,SULO
    )
endif(NEA_LINK_SULO)

if (IAR_DLIB_DISABLE_FP)
    # we already displayed a warning above, so don't do it again
else()
    APPEND_LIST_TO_STRING(CMAKE_EXE_LINKER_FLAGS --fpu=VFPv4_sp)
endif()

if (IAR_DLIB_DISABLE_REENTRANT)
    # Sane enough warning, but it's not an error
    message(" * IAR_DLIB_DISABLE_REENTRANT active, DLIB will not have locking callouts")
else()
    # default is to have protection
    APPEND_LIST_TO_STRING(CMAKE_EXE_LINKER_FLAGS --threaded_lib)
endif()

if (IAR_DLIB_ENABLE_RW_IRQVECS)
    APPEND_LIST_TO_STRING(CMAKE_EXE_LINKER_FLAGS --config_def __ram_vector_table__=1 )
endif()
