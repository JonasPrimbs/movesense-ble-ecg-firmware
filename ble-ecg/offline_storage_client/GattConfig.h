#pragma once

#include "MeasurementConfig.h"
#include "movesense.h"

// Size Definitions:
constexpr size_t CONFIGURATION_FIELD_SIZE = 8;
constexpr size_t SBEM_BLOCK_SIZE = 278;
constexpr size_t LOG_TRANSMISSION_MTU = SBEM_BLOCK_SIZE / 2;
constexpr size_t OFFLINE_STORAGE_GATT_CLIENT_CHARS = 4;

// Define UUIDs and constants for the GATT service
constexpr uint16_t OFFLINE_ACTIVITY_SVC_UUID16 = 0x1859;

// EcgVoltageMeasurement
// Imu9VectorMeasurement
// MeasurementCtrlConfig
// RecordedMeasStreaming

constexpr uint16_t ECG_VOLTAGE_MEASUREMENT_CHAR_UUID16 = 0x2BDD;
constexpr uint16_t IMU9_VECTOR_MEASUREMENT_CHAR_UUID16 = 0x2BDE;
constexpr uint16_t MEASUREMENT_CTRL_CONFIG_CHAR_UUID16 = 0x2BE5;
constexpr uint16_t RECORDED_MEAS_STREAMING_CHAR_UUID16 = 0x2BE6;

constexpr uint16_t UUID_A = ECG_VOLTAGE_MEASUREMENT_CHAR_UUID16;
constexpr uint16_t UUID_B = IMU9_VECTOR_MEASUREMENT_CHAR_UUID16;
constexpr uint16_t UUID_C = MEASUREMENT_CTRL_CONFIG_CHAR_UUID16;
constexpr uint16_t UUID_D = RECORDED_MEAS_STREAMING_CHAR_UUID16;

// Initial values for each Characteristic.
constexpr uint16_t INITIAL_A = 0;
constexpr uint16_t INITIAL_B = 0;
constexpr uint8_t INITIAL_C[CONFIGURATION_FIELD_SIZE] = {
    ECG_DEFAULT_MEASUREMENT_INTERVAL,
    IMU_DEFAULT_MEASUREMENT_INTERVAL,
    0,
    0,
    0,
    0,
    0,
    0,
};
constexpr uint16_t INITIAL_D = 0;
