# Settings for cross-compiling to the ARM / nRF52 / softdevice target.
set(CMSIS_DEVICE "nRF52")
set(CMSIS_DEVICE_LOWER "nrf52")

add_definitions(-DPRAGMA_PACK_SUPPORTED)
add_definitions(-DSERIAL_FRAME_WITH_CRC)

add_definitions(-DHWCONFIG_USED=1)
##
# TARGET CHIP SPECIFIC SETTINGS
##

add_definitions(-DARCH_CORTEX)
add_definitions(-DARCH_CORTEX_M4)
add_definitions(-DARMVFP_SP__)
add_definitions(-DARCH_NRF52)
add_definitions(-DNRF52)

# Since SS2 uses always softdevice on nrf52, we define the macro here.
add_definitions(-DSOFTDEVICE_PRESENT=1)

# TODO: XXX Confirm Actual value!
add_definitions(-DCPU_SPEED_HZ_ACTUAL=64000000)
# We round it up so that it can be divided more easily (smaller rounding error)
# TODO: XXX Confirm Actual value!
add_definitions(-DCPU_SPEED_HZ=64000000)

# HWConfig to target
set(HWCONFIG "SS2" CACHE STRING "HWConfig")
set_property(CACHE HWCONFIG PROPERTY STRINGS
             "SS2")

# Link target (bootloader or regular app, they start at different addresses)
set(BSP_LINKTO "appflash" CACHE STRING "Link to")
string(TOUPPER ${BSP_LINKTO} LINKTOUC)
add_definitions(-DBSP_LINKTO_${LINKTOUC}=1)

# OS configuration
set(NEA_OS "freertos" CACHE STRING "Operating system")
set_property(CACHE NEA_OS PROPERTY STRINGS
             "freertos" "sekos" "Posix" "kernel_win32")
mark_as_advanced(NEA_OS)
