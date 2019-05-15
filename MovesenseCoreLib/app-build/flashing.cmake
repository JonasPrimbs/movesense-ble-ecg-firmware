
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


# Check nrfutil version
execute_process(COMMAND ${NRFUTIL_CMD} version
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  RESULT_VARIABLE NRFUTIL_RESULT
  OUTPUT_VARIABLE NRFUTIL_VERSION_STR)
if (NOT NRFUTIL_VERSION_STR MATCHES "^nrfutil version 3\.")
    message("nrfutil version >=4 detected. adding --no-backup option")
    set(NRFUTIL_NO_BACKUP "--no-backup")
endif()


set (WITH_SETTINGS_HEX ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}_w_settings.hex)
set (COMBINED_HEX ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}_combined.hex)
set (DFU_PACKAGE_NAME ${EXECUTABLE_NAME}_dfu.zip)
set (MANUFACTURING_DATA_NAME ${EXECUTABLE_NAME}_manufacturing_data.txt)
# Command for converting ELF to Intel-HEX. results Movesense.hex
if (${COMPILER} MATCHES "IAR")
    add_custom_command(
        OUTPUT ${EXECUTABLE_NAME}.hex
        DEPENDS ${EXECUTABLE_NAME}.elf
        POST_BUILD
        COMMAND ielftool.exe --ihex ${EXECUTABLE_NAME}.elf ${EXECUTABLE_NAME}.hex
        )
elseif(${COMPILER} STREQUAL "GCCARM")
    add_custom_command(
        OUTPUT ${EXECUTABLE_NAME}.hex
        DEPENDS ${EXECUTABLE_NAME}
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.hex
        )
endif()

# Create Movesense_w_settings.hex
add_custom_command(
        OUTPUT ${WITH_SETTINGS_HEX}
        DEPENDS ${EXECUTABLE_NAME}.hex
        COMMAND ${NRFUTIL_CMD} settings generate ${NRFUTIL_NO_BACKUP} --family NRF52 --application ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex --application-version ${APPLICATION_VERSION} --bootloader-version ${BOOTLOADER_VERSION} --bl-settings-version 1 ${CMAKE_CURRENT_BINARY_DIR}/settings.hex
        COMMAND ${MERGEHEX_CMD} -m ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex ${CMAKE_CURRENT_BINARY_DIR}/settings.hex -o ${WITH_SETTINGS_HEX}
        COMMENT "Prepare app hex files for flashing"
)

# Create Movesense_manuf.hex
add_custom_command(
        OUTPUT ${COMBINED_HEX}
        DEPENDS ${WITH_SETTINGS_HEX} ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex
        COMMAND ${MERGEHEX_CMD} -m ${WITH_SETTINGS_HEX} ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex -o ${COMBINED_HEX}
        COMMENT "Prepare combined hex-file"
)

# DFU package command
add_custom_command(
        OUTPUT ${DFU_PACKAGE_NAME}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex
        COMMAND ${NRFUTIL_CMD} pkg generate --hw-version 52 --sd-req 0x8C,0x9F --application-version ${APPLICATION_VERSION} --application ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex --sd-id 0x9F --softdevice ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} --bootloader ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex --bootloader-version ${BOOTLOADER_VERSION} --key-file ${MOVESENSE_CORE_LIBRARY}/privatekey_debug.pem ${EXECUTABLE_NAME}_dfu_w_bootloader.zip
        COMMAND ${NRFUTIL_CMD} pkg generate --hw-version 52 --sd-req 0x9F --application-version 2 --application ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.hex --key-file ${MOVESENSE_CORE_LIBRARY}/privatekey_debug.pem ${DFU_PACKAGE_NAME}
        COMMENT "Creating DFU packages"
)

# flash & reset commands, dummy output file
add_custom_command(
        OUTPUT cmd.flash_with_erase
        DEPENDS ${WITH_SETTINGS_HEX} ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} --chiperase --verify
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${WITH_SETTINGS_HEX} --verify
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex --verify
        COMMENT "Erasing and flashing Softdevice, app & bootloader to device"
)

add_custom_command(
        OUTPUT cmd.flash_without_erasing
        DEPENDS ${WITH_SETTINGS_HEX} ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/softdevice/${MOVESENSE_INTENDED_SOFTDEVICE_HEX_FILE} --sectorerase --verify
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${WITH_SETTINGS_HEX} --sectorerase --verify
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${MOVESENSE_CORE_LIBRARY}/bootloader/bootloader.hex --sectorerase --verify
        COMMENT "Flashing Softdevice, app & bootloader to device"
)

add_custom_command(
        OUTPUT cmd.reset_device_after_flash_and_erase
        DEPENDS cmd.flash_with_erase
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMAND ${PING_CMD} 127.0.0.1
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMENT "Reset device"
)

add_custom_command(
        OUTPUT cmd.reset_flash_without_erasing
        DEPENDS cmd.flash_without_erasing
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMAND ${PING_CMD} 127.0.0.1
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMENT "Reset device"
)
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${MANUFACTURING_DATA_NAME}
        COMMAND ${NRFJPROG_CMD} --family NRF52 --memrd 0x74000 --n 4096 > ${CMAKE_CURRENT_BINARY_DIR}/${MANUFACTURING_DATA_NAME}
        COMMENT "Gathering the manufacturing data backup in the file: ${CMAKE_CURRENT_BINARY_DIR}/${MANUFACTURING_DATA_NAME}"
)

add_custom_command(
        OUTPUT flash.pkg
        DEPENDS ${COMBINED_HEX}
        COMMAND ${NRFJPROG_CMD} --family NRF52 --program ${COMBINED_HEX} --chiperase --verify
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMAND ${PING_CMD} 127.0.0.1
        COMMAND ${NRFJPROG_CMD} --family NRF52 --reset
        COMMENT "Flashing hex package which includes bootloader, softdevice and application. This command erases manufacturing data"
)

# Create targets for flashing and hex generation
if (NOT ${BSP} MATCHES "SDL")
    add_custom_target(dfupkg DEPENDS ${DFU_PACKAGE_NAME})
    add_custom_target(create_hex DEPENDS ${WITH_SETTINGS_HEX})
    add_custom_target(create_combined_hex DEPENDS ${COMBINED_HEX})

    add_custom_target(flash_all DEPENDS cmd.reset_device_after_flash_and_erase)
    add_custom_target(flash DEPENDS cmd.reset_flash_without_erasing)
    add_custom_target(backup_manufacturing_data DEPENDS ${MANUFACTURING_DATA_NAME})
    add_custom_target(pkgs DEPENDS ${DFU_PACKAGE_NAME} ${COMBINED_HEX} COMMENT "Creates both DFU and HEX packages")
    add_custom_target(flash_pkg DEPENDS flash.pkg)
endif()
