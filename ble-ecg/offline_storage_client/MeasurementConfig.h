#pragma once
#include "AbsoluteSeriesBuffer.h"
#include "movesense.h"
/** Measurement intervals **/
// The smallest possible measurement interval.
constexpr uint8_t ECG_BASE_MEASUREMENT_INTERVAL = 2;
constexpr uint8_t IMU_BASE_MEASUREMENT_INTERVAL = 5;

// The default measurement interval.
constexpr uint8_t ECG_DEFAULT_MEASUREMENT_INTERVAL = 4;
constexpr uint8_t IMU_DEFAULT_MEASUREMENT_INTERVAL = 20;

// The default skip counts.
constexpr uint8_t ECG_DEFAULT_SAMPLE_SKIP_COUNT =
    (uint8_t)ECG_DEFAULT_MEASUREMENT_INTERVAL / ECG_BASE_MEASUREMENT_INTERVAL;
constexpr uint8_t IMU_DEFAULT_SAMPLE_SKIP_COUNT =
    (uint8_t)IMU_DEFAULT_MEASUREMENT_INTERVAL / IMU_BASE_MEASUREMENT_INTERVAL;

// Base Measurement sampleRate in Hz
constexpr uint16_t ECG_BASE_SAMPLE_RATE = 500;
constexpr uint16_t IMU9_BASE_SAMPLE_RATE = 208;

/** Types **/
typedef int16_t ecg_t;
typedef int16_t acc_t;
typedef int16_t gyr_t;
typedef int16_t mag_t;

template <class T> struct Vector3
{
    T x;
    T y;
    T z;
};

typedef Vector3<acc_t> acc_vec_t;
typedef Vector3<gyr_t> gyr_vec_t;
typedef Vector3<mag_t> mag_vec_t;

struct mov_t
{
    acc_vec_t acc;
    gyr_vec_t gyr;
    mag_vec_t mag;
};

/** Buffers **/
constexpr uint16_t DEFAULT_ECG_OBJECT_SIZE = 16;
constexpr uint16_t DEFAULT_MOV_OBJECT_SIZE = 8;
constexpr uint16_t numberOfEcgBuffers = 2;
constexpr uint16_t numberOfMovBuffers = 2;

/** Resulting packet sizes **/
constexpr uint16_t ECG_PACKET_SIZE =
    DEFAULT_ECG_OBJECT_SIZE * sizeof(ecg_t) + sizeof(absolute_timestamp_t);

constexpr uint16_t MOV_PACKET_SIZE =
    DEFAULT_MOV_OBJECT_SIZE * sizeof(mov_t) + sizeof(absolute_timestamp_t);

constexpr uint16_t ECG_PACKET_SIZE_UINT32 = ECG_PACKET_SIZE / sizeof(uint32);
constexpr uint16_t MOV_PACKET_SIZE_UINT32 = MOV_PACKET_SIZE / sizeof(uint32);

constexpr uint64_t MAX_TIMESTAMP_VALUE = 1ULL << 32;

/** Sample conversion constants **/
constexpr ecg_t ECG_MAX_VALUE = 32767;      // 0x7FFF = 2^15 - 1;
constexpr ecg_t ECG_MIN_VALUE = -32767;     // 0x8001 = -(2^15 - 1);
constexpr ecg_t ECG_INVALID_VALUE = -32768; // 0x8000 = -2^15;

constexpr acc_t MAX_ACC = +32767;
constexpr acc_t MIN_ACC = -32767;
constexpr acc_t ERR_ACC = -32768;

constexpr gyr_t MAX_GYR = +32767;
constexpr gyr_t MIN_GYR = -32767;
constexpr gyr_t ERR_GYR = -32768;

constexpr mag_t MAX_MAG = +32767;
constexpr mag_t MIN_MAG = -32767;
constexpr mag_t ERR_MAG = -32768;

// TODO: use power of 2 to minimize rounding errors.
constexpr float accScale = 100.0f;
constexpr float gyrScale = 10.0f;
constexpr float magScale = 100.0f;
