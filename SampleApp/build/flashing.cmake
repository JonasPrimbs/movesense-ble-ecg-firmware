
# Code for flashing resulting nrf hex file to the target

add_custom_command(
        OUTPUT flash.1
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}
        COMMAND nrfjprog.exe --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/softdevice/s132_nrf52_2.0.0_softdevice.hex --chiperase --verify
        COMMENT "Flashing Softdevice to device"
)
add_custom_command(
        OUTPUT flash.2
        DEPENDS flash.1
        COMMAND nrfjprog.exe --family NRF52 --program ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex --verify
        COMMENT "Flashing Program to device"
)
add_custom_command(
        OUTPUT flash.3
        DEPENDS flash.2
        COMMAND nrfjprog.exe --family NRF52 --reset
        COMMENT "Reseting device"
)


add_custom_target(flash DEPENDS flash.3 flash.2 flash.1)