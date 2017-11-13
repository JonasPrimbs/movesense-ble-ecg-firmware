set(CMAKE_SYSTEM_NAME Generic)

# Compiler and toolchain configuration
set(COMPILER "GCCARM" CACHE STRING "COMPILER")
set_property(CACHE COMPILER PROPERTY STRINGS "GCCARM")
mark_as_advanced(COMPILER)

set(BSP "nRF5x")

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER_ID GNU)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
# By default, CMake will try to verify the designated compiler by building an
# executable. This will not work correctly when cross-compiling, so we set it to
# try making a static library instead.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
# Must use gcc frontends for ar and ranlib to get lto to work.
set(CMAKE_AR arm-none-eabi-gcc-ar CACHE FILEPATH "Path to ar")
mark_as_advanced(CMAKE_AR)
set(CMAKE_RANLIB arm-none-eabi-gcc-ranlib CACHE FILEPATH "Path to ranlib")
mark_as_advanced(CMAKE_RANLIB)

# Restrict autosearch on windows, on others allow cmake to do what it's supposed
# to do.
if(${CMAKE_HOST_SYSTEM} MATCHES "Windows")
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE CMAKE_FIND_ROOT_PATH_BOTH)
endif()
