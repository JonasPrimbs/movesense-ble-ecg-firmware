#pragma once

#include "movesense.h"


// At compile-time adjustable constants:

/**
 * @brief Default Movement Measurement Interval in milliseconds.
 * Must be one of the following: 
 *  -  1 ms = 1000 Hz
 *  -  2 ms =  500 Hz
 *  -  4 ms =  250 Hz
 *  -  8 ms =  125 Hz
 *  - 10 ms =  100 Hz
 *  - 20 ms =   50 Hz
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

