include(${CMAKE_CURRENT_LIST_DIR}/gcc-version-check.cmake)

set(GCCARM_USE_LTO True CACHE BOOL "Enable link-time optimization in GCC build")

set(LINKER_SCRIPTS_PATH ${CMAKE_CURRENT_LIST_DIR}/../platform/${BSP}/linker/gcc )

# Check that the relevant linker file exists
if(NOT EXISTS ${LINKER_SCRIPTS_PATH}/${BSP_LINKTO}.ld)
    message(FATAL_ERROR "BSP_LINKTO not chosen or invalid (cannot find ${LINKER_SCRIPTS_PATH}/${BSP_LINKTO}.ld or is not valid)")
endif()

# redefine the types to use for INT32 and UINT32 (driving the stdint.h setup)
# otherwise int32_t and uint32_t are based on long, and while it's also 32-bit
# in GCCARM targets, it will cause various unwanted issues with WB templates
# as well as require casting sprintf-style parameters unnecessarily.
set(GCCARM_TYPEFIX "-U__INT32_TYPE__ -D__INT32_TYPE__=int \
-U__UINT32_TYPE__ -D__UINT32_TYPE__=\"unsigned int\"")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
-W -Wall -Wextra -Werror -Wfatal-errors \
-Wshadow -Wpointer-arith -Wcast-qual -Wwrite-strings -Wunreachable-code \
-Wbad-function-cast -Wsign-compare \
-Wno-aggregate-return -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-function \
-Wno-cast-align -Wno-strict-aliasing -Wno-maybe-uninitialized \
-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
-fno-common -ffreestanding -fno-builtin -mapcs -gdwarf-3 \
-ffunction-sections -fdata-sections \
${GCCARM_TYPEFIX}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-W -Wall -Wextra -Werror -Wfatal-errors \
-Wshadow -Wpointer-arith -Wcast-qual -Wwrite-strings -Wunreachable-code \
-Wsign-compare \
-Wno-aggregate-return -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-function \
-Wno-cast-align -Wno-reorder -Wno-strict-aliasing -Wno-maybe-uninitialized \
-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
-fno-common -ffreestanding -fno-builtin -mapcs -gdwarf-3 \
-ffunction-sections -fdata-sections \
-fno-rtti -fno-exceptions -fno-unwind-tables -fno-use-cxa-atexit -fno-threadsafe-statics \
${GCCARM_TYPEFIX}")

# For some reason C_FLAGS is the base of CMAKE_EXE_LINKER_FLAGS here, argh?
# This translates to a final link call which will attempt to execute the subdir macro
# but since it would require gnu make, this will fail.
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
-Wl,--wrap -Wl,_malloc_r -Wl,--wrap -Wl,_calloc_r -Wl,--wrap -Wl,_free_r -Wl,--wrap -Wl,_realloc_r \
--specs=nano.specs \
-Wl,-Map -Wl,target.map \
-Wl,--gc-sections -Wl,-static -L${LINKER_SCRIPTS_PATH}")

if(${GCCARM_USE_LTO})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto -ffat-lto-objects")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -ffat-lto-objects")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-linker-plugin -flto")
# fix lto in archives
SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_FINISH "echo")

endif()

# setup __FILENAME__ so that it won't include the file name prefix
# For some reason these flags are also uesd at linking phase? Why?
# Undocumented "feature" in cmake: http://public.kitware.com/Bug/view.php?id=12652
# Can't easily work around this shit, so comment the support out for now. This results
# in quite bloated binaries with gnu toolchain, but can't help it
# Perhaps some day, someone will have time to fix this properly.
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRACE_FILENAME=\\\"$(notdir $<)\\\"")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTRACE_FILENAME=\\\"$(notdir $<)\\\"")

add_definitions(-DPRAGMA_PACK_SUPPORTED=1)
add_definitions(-D_GNU_SOURCE=1)

# GCC wants that the link libraries are listed in dependency order. The nextgen
# libraries do not have a well-specified acyclic dependency order. So we need
# to tell GCC to treat the bunch of nextgen libraries as on intermingled mess
# of symbols. This is done with linker directives that are passed to
# target_link_libraries and will be empty strings for non-GCC toolchains:
#
# target_link_libraries(${TARGET} ${LIBRARY_GROUP_START} ${LIBRARIES} ${LIBRARY_GROUP_END})

set(LIBRARY_GROUP_START "-Wl,--start-group")
set(LIBRARY_GROUP_END "-Wl,--end-group")

# We have to manually add c++ and math libraries
set(LIBRARIES ${LIBRARIES} m stdc++)

if (OPTIMIZATIONS STREQUAL "HIGH")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
elseif (OPTIMIZATIONS STREQUAL "MEDIUM")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
elseif (OPTIMIZATIONS STREQUAL "LOW")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
elseif (OPTIMIZATIONS STREQUAL "NONE")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
else()
  message(FATAL_ERROR "Unsupported optimization level '${OPTIMIZATIONS}'")
endif()

# Set stack size definition
add_definitions("-D__STACK_SIZE=1024")
add_definitions("-D__HEAP_SIZE=0")