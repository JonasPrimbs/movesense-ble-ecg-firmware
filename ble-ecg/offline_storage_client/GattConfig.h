#pragma once
#include "movesense.h"

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

constexpr uint16_t INITIAL_A = 0;
constexpr uint16_t INITIAL_B = 4;
constexpr uint16_t INITIAL_C = 0;
constexpr uint16_t INITIAL_D = 0;

constexpr size_t OFFLINE_STORAGE_GATT_CLIENT_CHARS = 4;
