# Compiler environment setup.

# If cross-compiling,
if (DEFINED CMAKE_TOOLCHAIN_FILE)
    if (NOT EXISTS ${CMAKE_TOOLCHAIN_FILE})
        message(FATAL_ERROR "CMAKE_TOOLCHAIN_FILE(${CMAKE_TOOLCHAIN_FILE}) does not exist!")
    endif()
    if (NOT DEFINED COMPILER)
        message(FATAL_ERROR "${CMAKE_TOOLCHAIN_FILE} does not define COMPILER")
    endif()
else()
    set(BSP "SDL")

    # Default toolchains
    if (MSVC)
        set(COMPILER "VS2015")
    elseif (APPLE OR UNIX)
        set(COMPILER "Clang64")
    else()
        message(FATAL_ERROR "Use toolchain file to specify build options")
    endif()
endif()
