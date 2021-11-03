set(CPU_SPEED 48000000)
add_definitions(-DCPU_SPEED=${CPU_SPEED})
add_definitions(-DARCH_SDL=1)



# HWConfig to target
set(HWCONFIG "SS2_Simu" CACHE STRING "HWConfig")
set_property(CACHE HWCONFIG PROPERTY STRINGS
             "SS2_Simu")

# Display HW version (Cut)
set(HW_DISPLAY "None" CACHE STRING "DisplayHW")
set_property(CACHE HW_DISPLAY PROPERTY STRINGS "None")
if(NOT ${HW_DISPLAY} MATCHES "None")
        add_definitions(-DDISPLAY_HW=${HW_DISPLAY})
endif()

include(${CMAKE_CURRENT_LIST_DIR}/sdl-hwconfigs.cmake)


set(BSP_LINK_STARTAT "0")
add_definitions(-DWB_HAVE_TCP_COMM=1)

include(CheckSymbolExists)
if (MSVC)
    check_symbol_exists("_M_IX86" "" VC_ARCH_X86)
    if(NOT VC_ARCH_X86)
        check_symbol_exists("_M_AMD64" "" VC_ARCH_X64)
    endif()
    if (NOT VC_ARCH_X64)
        add_definitions(-D_USE_32BIT_TIME_T=1)
    endif()


    set(SDL_LIB_PATH ${MOVESENSE_CORE_LIBRARY}/lib/VisualC/${HWCONFIG})
    link_libraries(${SDL_LIB_PATH}/SDL2.lib)
    link_libraries(${SDL_LIB_PATH}/SDL2_net.lib)
    link_libraries(${SDL_LIB_PATH}/SDL2_ttf.lib)
    link_libraries(${SDL_LIB_PATH}/SDL2main.lib)
endif()
