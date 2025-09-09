#pragma once
#include "movesense.h"

/** constants related to the gatt configuration of the logging activity client **/

// Service:
constexpr uint16_t ACTIVITY_SERVICE_UUID16 = 0x1859;
constexpr size_t ACTIVITY_SERVICE_NUM_OF_CHARACTERISTICS = 2;

// Characteristics:
constexpr uint16_t ECG_VOLTAGE_CHARACTERISTIC_UUID16 = 0x2BDD;

constexpr uint16_t MOV_MEASUREMENT_CHARACTERISTIC_UUID16 = 0x2BE2; // currently not used

constexpr uint16_t ECG_INTERVAL_CHARACTERISTIC_UUID16 = 0x2BE3;

constexpr uint16_t MOV_INTERVAL_CHARACTERISTIC_UUID16 = 0x2BE4; // currently not used
