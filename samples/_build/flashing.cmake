
# Code for flashing resulting nrf hex file to the target. Requires installation of python2.7 & nrfutil package
if(CMAKE_HOST_WIN32)
set(NRFUTIL_CMD nrfutil.exe)
set(MERGEHEX_CMD mergehex.exe)
set(NRFJPROG_CMD nrfjprog.exe)
set(PING_CMD ping.exe -n 6)
else()
set(NRFUTIL_CMD nrfutil)
set(MERGEHEX_CMD mergehex)
set(NRFJPROG_CMD nrfjprog)
set(PING_CMD ping -c 6)
endif()

set(APPLICATION_VERSION 2)
set(BOOTLOADER_VERSION 2)

add_custom_command(
        OUTPUT flash.1
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}
        COMMAND ${NRFUTIL_CMD} settings generate --family NRF52 --application ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex --application-version ${APPLICATION_VERSION} --bootloader-version ${BOOTLOADER_VERSION} --bl-settings-version 1 ${CMAKE_CURRENT_BINARY_DIR}/settings.hex
        COMMAND ${MERGEHEX_CMD} -m ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex ${CMAKE_CURRENT_BINARY_DIR}/settings.hex -o ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}_w_settings.hex
        COMMENT "Prepare app for flashing"
)

add_custom_command(
        OUTPUT create_dfu_pkg
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}
        COMMAND ${NRFUTIL_CMD} pkg generate --hw-version 52 --sd-req 0x8C,0x9F --application-version ${APPLICATION_VERSION} --application ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex --sd-id 0x9F --softdevice ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} --bootloader ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex --bootloader-version ${BOOTLOADER_VERSION} --key-file ${MOVESENSE_CORE_LIBRARY}/privatekey_debug.pem movesense_dfu_w_bootloader.zip
        COMMAND ${NRFUTIL_CMD} pkg generate --hw-version 52 --sd-req 0x9F --application-version 2 --application ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex --key-file ${MOVESENSE_CORE_LIBRARY}/privatekey_debug.pem movesense_dfu.zip
        COMMENT "Creating DFU packages"
)
add_custom_command(
        OUTPUT flash.2
        DEPENDS flash.1
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} --chiperase --verify
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}_w_settings.hex --verify
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex --verify
        COMMENT "Erasing and flashing Softdevice, app & bootloader to device"
)
add_custom_command(
        OUTPUT reset.flash
        DEPENDS flash.2
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMAND ${PING_CMD} 127.0.0.1
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMENT "Reset device"
)
add_custom_command(
        OUTPUT flash.softdevice
        DEPENDS flash.1
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} --sectorerase --verify
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}_w_settings.hex --sectorerase --verify
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex --sectorerase --verify
        COMMENT "Flashing Softdevice, app & bootloader to device"
)
add_custom_command(
        OUTPUT reset.flash_without_erasing
        DEPENDS flash.softdevice
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMAND ${PING_CMD} 127.0.0.1
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMENT "Reset device"
)
add_custom_command(
        OUTPUT backup.manufacturing_data
        COMMAND ${NRFJPROG_CMD} --family NRF52 --memrd 0x74000 --n 4096 >> ${MOVESENSE_CORE_LIBRARY}/manufacturing_data.txt
        COMMENT "Gathering the manufacturing data backup in the file: ${MOVESENSE_CORE_LIBRARY}/manufacturing_data.txt"
)

add_custom_target(flash_all DEPENDS reset.flash flash.2 flash.1 create_dfu_pkg)
add_custom_target(dfupkg DEPENDS create_dfu_pkg)
add_custom_target(flash DEPENDS reset.flash_without_erasing flash.softdevice flash.1 create_dfu_pkg)
add_custom_target(backup_manufacturing_data DEPENDS backup.manufacturing_data)
