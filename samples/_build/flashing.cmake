
# Code for flashing resulting nrf hex file to the target. Requires installation of python2.7 & nrfutil package
set(NRFUTIL_PATH nrfutil.exe)

add_custom_command(
        OUTPUT flash.1
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}
        COMMAND ${NRFUTIL_PATH} settings generate --family NRF52 --application ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex --application-version 1 --bootloader-version 1 --bl-settings-version 1 ${CMAKE_CURRENT_BINARY_DIR}/settings.hex
        COMMAND mergehex.exe -m ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex ${CMAKE_CURRENT_BINARY_DIR}/settings.hex -o ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}_w_settings.hex
        COMMENT "Prepare app for flashing"
)

add_custom_command(
        OUTPUT create_dfu_pkg
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}
        COMMAND ${NRFUTIL_PATH} pkg generate --hw-version 52 --sd-req 0x8C --application-version 1 --application ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex --key-file ${MOVESENSE_CORE_LIBRARY}/privatekey_debug.pem movesense_dfu.zip
        COMMENT "Creating DFU package"
)
add_custom_command(
        OUTPUT flash.2
        DEPENDS flash.1
        COMMAND nrfjprog.exe --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} --chiperase --verify
        COMMAND nrfjprog.exe --family NRF52 --program ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}_w_settings.hex --verify
        COMMAND nrfjprog.exe --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex --verify
        COMMENT "Flashing Softdevice, app & bootloader to device"
)
add_custom_command(
        OUTPUT flash.3
        DEPENDS flash.2
        COMMAND nrfjprog.exe --family NRF52 --reset
        COMMENT "Reset device"
)


add_custom_target(flash DEPENDS flash.3 flash.2 flash.1 create_dfu_pkg)
add_custom_target(dfupkg DEPENDS create_dfu_pkg)