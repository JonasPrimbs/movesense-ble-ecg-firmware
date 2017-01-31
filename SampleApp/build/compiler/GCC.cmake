set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wbad-function-cast -Wnested-externs -W -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings \
-Wsign-compare -Wno-aggregate-return -Wunreachable-code -Wno-unused-parameter -Wno-unused-function -ffunction-sections -fdata-sections -mno-ms-bitfields")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -pedantic -std=gnu++11 -Werror -Wfatal-errors -s -W -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings \
-Wsign-compare -Wno-aggregate-return -Wunreachable-code -Wno-unused-parameter -Wno-unused-function -ffunction-sections -fdata-sections -mno-ms-bitfields \
-c") # -c Should not be here - prevents linking the target

add_definitions(-DPRAGMA_PACK_SUPPORTED=1)
add_definitions(-DMSYS=1)
add_definitions(-D_GNU_SOURCE=1)
add_definitions(-Dmain=SDL_main)
add_definitions(-DSDL_SHOW_VERSION)

set(LIBRARY_GROUP_START "-Wl,--start-group")
set(LIBRARY_GROUP_END "-Wl,--end-group")
