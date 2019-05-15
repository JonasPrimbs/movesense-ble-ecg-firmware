#pragma once

/******************************************************************************

    Copyright (c) Suunto Oy 2017.
    All rights reserved.

******************************************************************************/

/*
 * Internal macros for System Event C API.
 *
 */

// System event macros for FAULT
#define SYSEVENT_FAULT_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_FAULT_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_FAULT_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_FAULT_TRACE( ... ) // Trace severity level is not enabled

// System event macros for DEBUGOUT
#define SYSEVENT_DEBUGOUT_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_DEBUGOUT_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_DEBUGOUT_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_DEBUGOUT_TRACE( ... ) // Trace severity level is not enabled

// System event macros for WBAPI
#define SYSEVENT_WBAPI_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_WBAPI_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_WBAPI_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_WBAPI_TRACE( ... ) // Trace severity level is not enabled

// System event macros for WBCOMM
#define SYSEVENT_WBCOMM_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_WBCOMM_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_WBCOMM_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_WBCOMM_TRACE( ... ) // Trace severity level is not enabled

// System event macros for WBDEVD
#define SYSEVENT_WBDEVD_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_WBDEVD_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_WBDEVD_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_WBDEVD_TRACE( ... ) // Trace severity level is not enabled

// System event macros for WBMAIN
#define SYSEVENT_WBMAIN_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_WBMAIN_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_WBMAIN_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_WBMAIN_TRACE( ... ) // Trace severity level is not enabled

// System event macros for WBPORT
#define SYSEVENT_WBPORT_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_WBPORT_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_WBPORT_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_WBPORT_TRACE( ... ) // Trace severity level is not enabled

// System event macros for SCHEDULER
#define SYSEVENT_SCHEDULER_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_SCHEDULER_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_SCHEDULER_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_SCHEDULER_TRACE( ... ) // Trace severity level is not enabled

// System event macros for MPU
#define SYSEVENT_MPU_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_MPU_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_MPU_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_MPU_TRACE( ... ) // Trace severity level is not enabled

// System event macros for WATCHDOG
#define SYSEVENT_WATCHDOG_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_WATCHDOG_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_WATCHDOG_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_WATCHDOG_TRACE( ... ) // Trace severity level is not enabled

// System event macros for POWER_STATE
#define SYSEVENT_POWER_STATE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_POWER_STATE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_POWER_STATE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_POWER_STATE_TRACE( ... ) // Trace severity level is not enabled

// System event macros for DRT
#define SYSEVENT_DRT_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_DRT_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_DRT_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_DRT_TRACE( ... ) // Trace severity level is not enabled

// System event macros for BOOTLOOP
#define SYSEVENT_BOOTLOOP_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_BOOTLOOP_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_BOOTLOOP_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_BOOTLOOP_TRACE( ... ) // Trace severity level is not enabled

// System event macros for SPI
#define SYSEVENT_SPI_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_SPI_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_SPI_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_SPI_TRACE( ... ) // Trace severity level is not enabled

// System event macros for I2C
#define SYSEVENT_I2C_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_I2C_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_I2C_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_I2C_TRACE( ... ) // Trace severity level is not enabled

// System event macros for UART
#define SYSEVENT_UART_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_UART_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_UART_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_UART_TRACE( ... ) // Trace severity level is not enabled

// System event macros for BLE
#define SYSEVENT_BLE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_BLE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_BLE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_BLE_TRACE( ... ) // Trace severity level is not enabled

// System event macros for WIFI
#define SYSEVENT_WIFI_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_WIFI_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_WIFI_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_WIFI_TRACE( ... ) // Trace severity level is not enabled

// System event macros for LOGGER
#define SYSEVENT_LOGGER_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_LOGGER_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_LOGGER_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_LOGGER_TRACE( ... ) // Trace severity level is not enabled

// System event macros for FILESYSTEM
#define SYSEVENT_FILESYSTEM_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_FILESYSTEM_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_FILESYSTEM_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_FILESYSTEM_TRACE( ... ) // Trace severity level is not enabled

// System event macros for SPEEDFUSION
#define SYSEVENT_SPEEDFUSION_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_SPEEDFUSION_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_SPEEDFUSION_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_SPEEDFUSION_TRACE( ... ) // Trace severity level is not enabled

// System event macros for INTERCHIP
#define SYSEVENT_INTERCHIP_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_INTERCHIP_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_INTERCHIP_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_INTERCHIP_TRACE( ... ) // Trace severity level is not enabled

// System event macros for SETTINGS
#define SYSEVENT_SETTINGS_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_SETTINGS_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_SETTINGS_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_SETTINGS_TRACE( ... ) // Trace severity level is not enabled

// System event macros for COMPASS
#define SYSEVENT_COMPASS_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_COMPASS_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_COMPASS_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_COMPASS_TRACE( ... ) // Trace severity level is not enabled

// System event macros for PERIPHERAL_FIRMWARE
#define SYSEVENT_PERIPHERAL_FIRMWARE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_PERIPHERAL_FIRMWARE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_PERIPHERAL_FIRMWARE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_PERIPHERAL_FIRMWARE_TRACE( ... ) // Trace severity level is not enabled

// System event macros for FWUPDATE
#define SYSEVENT_FWUPDATE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_FWUPDATE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_FWUPDATE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_FWUPDATE_TRACE( ... ) // Trace severity level is not enabled

// System event macros for ALTIFUSION
#define SYSEVENT_ALTIFUSION_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_ALTIFUSION_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_ALTIFUSION_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_ALTIFUSION_TRACE( ... ) // Trace severity level is not enabled

// System event macros for ACTIVITY
#define SYSEVENT_ACTIVITY_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_ACTIVITY_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_ACTIVITY_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_ACTIVITY_TRACE( ... ) // Trace severity level is not enabled

// System event macros for NAVIGATION
#define SYSEVENT_NAVIGATION_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_NAVIGATION_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_NAVIGATION_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_NAVIGATION_TRACE( ... ) // Trace severity level is not enabled

// System event macros for LAPENGINE
#define SYSEVENT_LAPENGINE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_LAPENGINE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_LAPENGINE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_LAPENGINE_TRACE( ... ) // Trace severity level is not enabled

// System event macros for SYNC
#define SYSEVENT_SYNC_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_SYNC_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_SYNC_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_SYNC_TRACE( ... ) // Trace severity level is not enabled

// System event macros for TRAININGLAB
#define SYSEVENT_TRAININGLAB_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_TRAININGLAB_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_TRAININGLAB_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_TRAININGLAB_TRACE( ... ) // Trace severity level is not enabled

// System event macros for SLEEP
#define SYSEVENT_SLEEP_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_SLEEP_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_SLEEP_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_SLEEP_TRACE( ... ) // Trace severity level is not enabled

// System event macros for ANALYTICS
#define SYSEVENT_ANALYTICS_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_ANALYTICS_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_ANALYTICS_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_ANALYTICS_TRACE( ... ) // Trace severity level is not enabled

// System event macros for COACH
#define SYSEVENT_COACH_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_COACH_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_COACH_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_COACH_TRACE( ... ) // Trace severity level is not enabled

// System event macros for PLANNEDMOVES
#define SYSEVENT_PLANNEDMOVES_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_PLANNEDMOVES_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_PLANNEDMOVES_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_PLANNEDMOVES_TRACE( ... ) // Trace severity level is not enabled

// System event macros for APPLICATION
#define SYSEVENT_APPLICATION_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_APPLICATION_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_APPLICATION_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_APPLICATION_TRACE( ... ) // Trace severity level is not enabled

// System event macros for UI_FRAMEWORK
#define SYSEVENT_UI_FRAMEWORK_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_UI_FRAMEWORK_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_UI_FRAMEWORK_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_UI_FRAMEWORK_TRACE( ... ) // Trace severity level is not enabled

// System event macros for DUKTAPE
#define SYSEVENT_DUKTAPE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_DUKTAPE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_DUKTAPE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_DUKTAPE_TRACE( ... ) // Trace severity level is not enabled

// System event macros for MAGNETOMETER
#define SYSEVENT_MAGNETOMETER_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_MAGNETOMETER_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_MAGNETOMETER_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_MAGNETOMETER_TRACE( ... ) // Trace severity level is not enabled

// System event macros for GPS
#define SYSEVENT_GPS_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_GPS_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_GPS_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_GPS_TRACE( ... ) // Trace severity level is not enabled

// System event macros for DISPLAY
#define SYSEVENT_DISPLAY_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_DISPLAY_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_DISPLAY_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_DISPLAY_TRACE( ... ) // Trace severity level is not enabled

// System event macros for TOUCH
#define SYSEVENT_TOUCH_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_TOUCH_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_TOUCH_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_TOUCH_TRACE( ... ) // Trace severity level is not enabled

// System event macros for ACCELEROMETER
#define SYSEVENT_ACCELEROMETER_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_ACCELEROMETER_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_ACCELEROMETER_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_ACCELEROMETER_TRACE( ... ) // Trace severity level is not enabled

// System event macros for GYROSCOPE
#define SYSEVENT_GYROSCOPE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_GYROSCOPE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_GYROSCOPE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_GYROSCOPE_TRACE( ... ) // Trace severity level is not enabled

// System event macros for VIBRA
#define SYSEVENT_VIBRA_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_VIBRA_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_VIBRA_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_VIBRA_TRACE( ... ) // Trace severity level is not enabled

// System event macros for BUZZER
#define SYSEVENT_BUZZER_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_BUZZER_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_BUZZER_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_BUZZER_TRACE( ... ) // Trace severity level is not enabled

// System event macros for EXT_FLASH
#define SYSEVENT_EXT_FLASH_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_EXT_FLASH_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_EXT_FLASH_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_EXT_FLASH_TRACE( ... ) // Trace severity level is not enabled

// System event macros for INT_FLASH
#define SYSEVENT_INT_FLASH_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_INT_FLASH_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_INT_FLASH_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_INT_FLASH_TRACE( ... ) // Trace severity level is not enabled

// System event macros for PMIC
#define SYSEVENT_PMIC_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_PMIC_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_PMIC_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_PMIC_TRACE( ... ) // Trace severity level is not enabled

// System event macros for FUEL_GAUGE
#define SYSEVENT_FUEL_GAUGE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_FUEL_GAUGE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_FUEL_GAUGE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_FUEL_GAUGE_TRACE( ... ) // Trace severity level is not enabled

// System event macros for BUTTON
#define SYSEVENT_BUTTON_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_BUTTON_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_BUTTON_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_BUTTON_TRACE( ... ) // Trace severity level is not enabled

// System event macros for PRESSURE
#define SYSEVENT_PRESSURE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_PRESSURE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_PRESSURE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_PRESSURE_TRACE( ... ) // Trace severity level is not enabled

// System event macros for OHR
#define SYSEVENT_OHR_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_OHR_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_OHR_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_OHR_TRACE( ... ) // Trace severity level is not enabled

// System event macros for MSM
#define SYSEVENT_MSM_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_MSM_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_MSM_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_MSM_TRACE( ... ) // Trace severity level is not enabled

// System event macros for SKS_CABLE
#define SYSEVENT_SKS_CABLE_ERROR( ... ) // Error severity level is not enabled
#define SYSEVENT_SKS_CABLE_WARNING( ... ) // Warning severity level is not enabled
#define SYSEVENT_SKS_CABLE_EVENT( ... ) // Event severity level is not enabled
#define SYSEVENT_SKS_CABLE_TRACE( ... ) // Trace severity level is not enabled

//
// Use the macros below for flagging out code which is needed only when specific severity level is enabled
//

// Systemevent feature test macros for FAULT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FAULT_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FAULT_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FAULT_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FAULT_TRACE

// Systemevent feature test macros for DEBUGOUT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DEBUGOUT_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DEBUGOUT_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DEBUGOUT_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DEBUGOUT_TRACE

// Systemevent feature test macros for WBAPI
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBAPI_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBAPI_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBAPI_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBAPI_TRACE

// Systemevent feature test macros for WBCOMM
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBCOMM_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBCOMM_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBCOMM_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBCOMM_TRACE

// Systemevent feature test macros for WBDEVD
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBDEVD_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBDEVD_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBDEVD_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBDEVD_TRACE

// Systemevent feature test macros for WBMAIN
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBMAIN_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBMAIN_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBMAIN_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBMAIN_TRACE

// Systemevent feature test macros for WBPORT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBPORT_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBPORT_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBPORT_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WBPORT_TRACE

// Systemevent feature test macros for SCHEDULER
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SCHEDULER_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SCHEDULER_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SCHEDULER_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SCHEDULER_TRACE

// Systemevent feature test macros for MPU
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MPU_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MPU_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MPU_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MPU_TRACE

// Systemevent feature test macros for WATCHDOG
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WATCHDOG_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WATCHDOG_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WATCHDOG_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WATCHDOG_TRACE

// Systemevent feature test macros for POWER_STATE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_POWER_STATE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_POWER_STATE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_POWER_STATE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_POWER_STATE_TRACE

// Systemevent feature test macros for DRT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DRT_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DRT_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DRT_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DRT_TRACE

// Systemevent feature test macros for BOOTLOOP
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BOOTLOOP_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BOOTLOOP_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BOOTLOOP_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BOOTLOOP_TRACE

// Systemevent feature test macros for SPI
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SPI_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SPI_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SPI_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SPI_TRACE

// Systemevent feature test macros for I2C
// #define ENABLE_NEA_SYSTEM_EVENT_ID_I2C_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_I2C_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_I2C_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_I2C_TRACE

// Systemevent feature test macros for UART
// #define ENABLE_NEA_SYSTEM_EVENT_ID_UART_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_UART_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_UART_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_UART_TRACE

// Systemevent feature test macros for BLE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BLE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BLE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BLE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BLE_TRACE

// Systemevent feature test macros for WIFI
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WIFI_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WIFI_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WIFI_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_WIFI_TRACE

// Systemevent feature test macros for LOGGER
// #define ENABLE_NEA_SYSTEM_EVENT_ID_LOGGER_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_LOGGER_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_LOGGER_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_LOGGER_TRACE

// Systemevent feature test macros for FILESYSTEM
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FILESYSTEM_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FILESYSTEM_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FILESYSTEM_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FILESYSTEM_TRACE

// Systemevent feature test macros for SPEEDFUSION
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SPEEDFUSION_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SPEEDFUSION_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SPEEDFUSION_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SPEEDFUSION_TRACE

// Systemevent feature test macros for INTERCHIP
// #define ENABLE_NEA_SYSTEM_EVENT_ID_INTERCHIP_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_INTERCHIP_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_INTERCHIP_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_INTERCHIP_TRACE

// Systemevent feature test macros for SETTINGS
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SETTINGS_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SETTINGS_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SETTINGS_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SETTINGS_TRACE

// Systemevent feature test macros for COMPASS
// #define ENABLE_NEA_SYSTEM_EVENT_ID_COMPASS_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_COMPASS_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_COMPASS_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_COMPASS_TRACE

// Systemevent feature test macros for PERIPHERAL_FIRMWARE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PERIPHERAL_FIRMWARE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PERIPHERAL_FIRMWARE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PERIPHERAL_FIRMWARE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PERIPHERAL_FIRMWARE_TRACE

// Systemevent feature test macros for FWUPDATE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FWUPDATE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FWUPDATE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FWUPDATE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FWUPDATE_TRACE

// Systemevent feature test macros for ALTIFUSION
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ALTIFUSION_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ALTIFUSION_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ALTIFUSION_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ALTIFUSION_TRACE

// Systemevent feature test macros for ACTIVITY
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ACTIVITY_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ACTIVITY_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ACTIVITY_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ACTIVITY_TRACE

// Systemevent feature test macros for NAVIGATION
// #define ENABLE_NEA_SYSTEM_EVENT_ID_NAVIGATION_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_NAVIGATION_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_NAVIGATION_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_NAVIGATION_TRACE

// Systemevent feature test macros for LAPENGINE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_LAPENGINE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_LAPENGINE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_LAPENGINE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_LAPENGINE_TRACE

// Systemevent feature test macros for SYNC
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SYNC_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SYNC_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SYNC_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SYNC_TRACE

// Systemevent feature test macros for TRAININGLAB
// #define ENABLE_NEA_SYSTEM_EVENT_ID_TRAININGLAB_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_TRAININGLAB_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_TRAININGLAB_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_TRAININGLAB_TRACE

// Systemevent feature test macros for SLEEP
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SLEEP_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SLEEP_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SLEEP_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SLEEP_TRACE

// Systemevent feature test macros for ANALYTICS
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ANALYTICS_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ANALYTICS_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ANALYTICS_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ANALYTICS_TRACE

// Systemevent feature test macros for COACH
// #define ENABLE_NEA_SYSTEM_EVENT_ID_COACH_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_COACH_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_COACH_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_COACH_TRACE

// Systemevent feature test macros for PLANNEDMOVES
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PLANNEDMOVES_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PLANNEDMOVES_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PLANNEDMOVES_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PLANNEDMOVES_TRACE

// Systemevent feature test macros for APPLICATION
// #define ENABLE_NEA_SYSTEM_EVENT_ID_APPLICATION_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_APPLICATION_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_APPLICATION_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_APPLICATION_TRACE

// Systemevent feature test macros for UI_FRAMEWORK
// #define ENABLE_NEA_SYSTEM_EVENT_ID_UI_FRAMEWORK_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_UI_FRAMEWORK_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_UI_FRAMEWORK_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_UI_FRAMEWORK_TRACE

// Systemevent feature test macros for DUKTAPE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DUKTAPE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DUKTAPE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DUKTAPE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DUKTAPE_TRACE

// Systemevent feature test macros for MAGNETOMETER
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MAGNETOMETER_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MAGNETOMETER_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MAGNETOMETER_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MAGNETOMETER_TRACE

// Systemevent feature test macros for GPS
// #define ENABLE_NEA_SYSTEM_EVENT_ID_GPS_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_GPS_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_GPS_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_GPS_TRACE

// Systemevent feature test macros for DISPLAY
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DISPLAY_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DISPLAY_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DISPLAY_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_DISPLAY_TRACE

// Systemevent feature test macros for TOUCH
// #define ENABLE_NEA_SYSTEM_EVENT_ID_TOUCH_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_TOUCH_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_TOUCH_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_TOUCH_TRACE

// Systemevent feature test macros for ACCELEROMETER
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ACCELEROMETER_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ACCELEROMETER_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ACCELEROMETER_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_ACCELEROMETER_TRACE

// Systemevent feature test macros for GYROSCOPE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_GYROSCOPE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_GYROSCOPE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_GYROSCOPE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_GYROSCOPE_TRACE

// Systemevent feature test macros for VIBRA
// #define ENABLE_NEA_SYSTEM_EVENT_ID_VIBRA_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_VIBRA_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_VIBRA_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_VIBRA_TRACE

// Systemevent feature test macros for BUZZER
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BUZZER_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BUZZER_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BUZZER_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BUZZER_TRACE

// Systemevent feature test macros for EXT_FLASH
// #define ENABLE_NEA_SYSTEM_EVENT_ID_EXT_FLASH_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_EXT_FLASH_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_EXT_FLASH_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_EXT_FLASH_TRACE

// Systemevent feature test macros for INT_FLASH
// #define ENABLE_NEA_SYSTEM_EVENT_ID_INT_FLASH_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_INT_FLASH_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_INT_FLASH_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_INT_FLASH_TRACE

// Systemevent feature test macros for PMIC
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PMIC_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PMIC_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PMIC_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PMIC_TRACE

// Systemevent feature test macros for FUEL_GAUGE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FUEL_GAUGE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FUEL_GAUGE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FUEL_GAUGE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_FUEL_GAUGE_TRACE

// Systemevent feature test macros for BUTTON
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BUTTON_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BUTTON_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BUTTON_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_BUTTON_TRACE

// Systemevent feature test macros for PRESSURE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PRESSURE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PRESSURE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PRESSURE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_PRESSURE_TRACE

// Systemevent feature test macros for OHR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_OHR_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_OHR_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_OHR_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_OHR_TRACE

// Systemevent feature test macros for MSM
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MSM_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MSM_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MSM_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_MSM_TRACE

// Systemevent feature test macros for SKS_CABLE
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SKS_CABLE_ERROR
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SKS_CABLE_WARNING
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SKS_CABLE_EVENT
// #define ENABLE_NEA_SYSTEM_EVENT_ID_SKS_CABLE_TRACE
