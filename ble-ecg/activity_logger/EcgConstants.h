#pragma once
#include "movesense.h"

/** constants and definitions for ecg-related values **/

typedef int16_t ecg_t;

constexpr int DEFAULT_ECG_MEASUREMENT_INTERVAL = 4;
constexpr int ECG_BASE_MEASUREMENT_INTERVAL = 2;
constexpr uint16_t DEFAULT_ECG_OBJECT_SIZE = 16;
constexpr size_t NUMBER_OF_ECG_BUFFERS = 2;

constexpr ecg_t ECG_MAX_VALUE = +32767;
constexpr ecg_t ECG_MIN_VALUE = -32767;
constexpr ecg_t ECG_INVALID_VALUE = -32768;
