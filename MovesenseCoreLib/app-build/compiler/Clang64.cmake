# Using C++11, Compiling 64-bit executable
set(CMAKE_C_FLAGS "-m64")
set(CMAKE_CXX_FLAGS "-std=c++11 -m64")

# Currently IS_UNITTEST_BUILD means we also build for measuring code coverage using instrumented build on OSX
if(${IS_UNITTEST_BUILD})
    # --coverage is synonym for -ftest-coverage -fprofile-arcs
    # TODO check which options are actually needed
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-instr-generate -fcoverage-mapping")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate")
    message("Clang profiling activated")
endif()
