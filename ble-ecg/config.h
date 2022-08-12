#pragma once

#include "movesense.h"


// At compile-time adjustable constants:

/**
 * @brief Default ECG Measurement Interval in milliseconds.
 * Must be one of the following: 
 *  -  2 ms =  500 Hz
 *  -  4 ms =  250 Hz
 *  -  5 ms =  200 Hz
 *  -  8 ms =  125 Hz
 *  - 10 ms =  100 Hz
 */
const int DEFAULT_ECG_MEASUREMENT_INTERVAL = 4;

/** Default number of ECG samples per message. */
const uint16_t DEFAULT_ECG_OBJECT_SIZE = 16;

/**
 * @brief Default Movement Measurement Interval in milliseconds.
 * Must be one of the following: 
 *  -  5 ms = 208 Hz
 *  - 10 ms = 104 Hz
 *  - 20 ms =  52 Hz
 *  - 40 ms =  26 Hz
 */
const int DEFAULT_MOV_MEASUREMENT_INTERVAL = 20;

/** Default number of Movement samples per message. */
const uint16_t DEFAULT_MOV_OBJECT_SIZE = 8;

// General GATT Services and Characteristics UUIDs:

/** 16-bit UUID for sample interval Characteristic. */
const uint16_t measurementIntervalCharUUID16 = 0x2A21;
/** 16-bit UUID for object size Characteristic. */
const uint16_t objectSizeCharUUID16 = 0x2BDE;


// ECG GATT Services and Characteristics UUIDs:

/** 16-bit UUID for ECG Service. */
const uint16_t ecgSvcUUID16 = 0x1857;

/** 16-bit UUID for ECG Voltage Characteristic. */
const uint16_t ecgVoltageCharUUID16 = 0x2BDD;

/** Number of ECG buffers. Must be > 1. */
const size_t numberOfEcgBuffers = 2;


// Movement GATT Services and Characteristics UUIDs:

/** 16-bit UUID for Movement Service. */
const uint16_t movSvcUUID16 = 0x1858;

/** 16-bit UUID for Acceleration Characteristic. */
const uint16_t movAccCharUUID16 = 0x2BDF;
/** 16-bit UUID for Gyroscope Characteristic. */
const uint16_t movGyrCharUUID16 = 0x2BE0;
/** 16-bit UUID for Magnetic Field Characteristic. */
const uint16_t movMagCharUUID16 = 0x2BE1;

/** Number of Acceleration buffers. Must be > 1. */
const size_t numberOfMovAccBuffers = 2;
/** Number of Gyroscope buffers. Must be > 1. */
const size_t numberOfMovGyrBuffers = 2;
/** Number of Magnetic Field buffers. Must be > 1. */
const size_t numberOfMovMagBuffers = 2;


// Activity GATT Services and Characteristics UUIDs:

/** 16-bit UUID for Activity Service. */
const uint16_t activitySvcUUID16 = 0x1859;

/** 16-bit UUID for Movement Characteristic. */
const uint16_t movCharUUID16 = 0x2BE2;

/** 16-bit UUID for ECG Measurement Interval Characteristic. */
const uint16_t ecgMeasurementIntervalCharUUID16 = 0x2BE3;
/** 16-bit UUID for Movement Measurement Interval Characteristic. */
const uint16_t movMeasurementIntervalCharUUID16 = 0x2BE4;


// ECG value definitions:

/** Type definition of ECG value. */
typedef int16_t ecg_t;
/** Maximum ECG value. */
const ecg_t ECG_MAX_VALUE = 32767; // 0x7FFF = 2^15 - 1;
/** Minimum ECG value. */
const ecg_t ECG_MIN_VALUE = -32767; // 0x8001 = -(2^15 - 1);
/** Invalid ECG value. */
const ecg_t ECG_INVALID_VALUE = -32768; // 0x8000 = -2^15;


// Movement value definitions:

template<class T>
struct Vector3
{
    T x;
    T y;
    T z;
};

/** Type definition of Acceleration value. */
typedef float acc_t;
/** Type definition of Acceleration vector. */
typedef Vector3<acc_t> acc_vec_t;

/** Type definition of Gyroscope value. */
typedef float gyr_t;
/** Type definition of Gyroscope vector. */
typedef Vector3<gyr_t> gyr_vec_t;

/** Type definition of Magnetic Field value. */
typedef float mag_t;
/** Type definition of Magnetic Field vector. */
typedef Vector3<mag_t> mag_vec_t;

/** Number of Movement buffers. Must be > 1. */
const size_t numberOfMovBuffers = 2;

/** Size of the movement buffer. DEFAULT_MOV_OBJECT_SIZE must be a multiple of this. */
const size_t movBufferSize = 4;

struct mov_t
{
    acc_vec_t acc;
    gyr_vec_t gyr;
    mag_vec_t mag;
};
