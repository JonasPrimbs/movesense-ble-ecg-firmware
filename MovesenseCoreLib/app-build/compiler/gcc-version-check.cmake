if (NOT CMAKE_C_COMPILER_VERSION VERSION_EQUAL 9.2.1)
    message(FATAL_ERROR "GCC C compiler version must be 9.2.1! --- (${CMAKE_C_COMPILER_VERSION} detected)")
endif()
if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 9.2.1)
    message(FATAL_ERROR "GCC C++ compiler version must be 9.2.1! --- (${CMAKE_CXX_COMPILER_VERSION} detected)")
endif()