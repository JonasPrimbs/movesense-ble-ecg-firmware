// Copyright (c) Suunto Oy 2016. All rights reserved.
#pragma once
#include "buildconfig/hal/features.h"

#ifdef BUILD_HAL_WANT_GPS
#include "hal/gps/nextgen-driver/gps-driver.hpp"
#endif // def BUILD_HAL_WANT_GPS

#ifdef BUILD_HAL_WANT_BUTTONS
#include "hal/userinput/ButtonProvider.hpp"
#endif // def BUILD_HAL_WANT_BUTTONS

#include "hal/sensor/accelerometer/lsm6ds3/AccelerometerProvider.hpp"
#include "hal/sensor/AFE/MAX30001/RrProvider.hpp"

#ifdef HAL_WANT_MAGNETOMETER
#include "hal/sensor/magnetometer/ak09912/MagnetometerProvider.hpp"
#endif // def HAL_WANT_MAGNETOMETER


#ifdef HAL_WANT_PRESSURE
#include "hal/sensor/pressureSensor/ms5837-02ba/PressureProvider.hpp"
#endif // def HAL_WANT_PRESSURE

#ifdef BUILD_HAL_WANT_TMP112
#include "hal/sensor/temperature/tmp112/TemperatureProvider.hpp"
#endif // def BUILD_HAL_WANT_TMP112

#ifdef BUILD_HAL_WANT_1WIRE
#include "hal/sensor/security/ds24l65/DS24L65_provider.hpp"
#endif // def BUILD_HAL_WANT_1WIRE

#ifdef BUILD_HAL_WANT_TOUCH
#include "hal/touchscreen/tma5xx/TouchProviderDriver.hpp"
#endif // def BUILD_HAL_WANT_TOUCH

#if defined (BUILD_HAL_WANT_BUZZER) || defined (BUILD_HAL_WANT_VIBRA)
#include "hal/vibra/drv2605/VibraProvider.hpp"
#include "hal/buzzer/BuzzerProvider.hpp"
#include "hal/useroutput/UserFeedbackProvider.hpp"
#endif // defined (BUILD_HAL_WANT_BUZZER) || defined (BUILD_HAL_WANT_VIBRA)

#include "hal/pmic/PmicProvider.hpp"
#include "hal/sensor/batteryGauge/SS2/BatteryChargeProvider.hpp"


// include hwconfig specific restrictions (not all hardware is present in all
// designs)
#include "hwconfig.h"

#ifdef BUILD_HAL_WANT_ACCELEROMETER
// if we cannot satisfy the wish, break the build here
#if !defined(HWCONFIG_PERF16_SIMU) && !defined(BSP_HACK_ENABLE_SPI_EXPERIMENT)
#warning "Cannot enable accelerometer since SPIE is disabled (or not building for SIMU)"
#endif
#endif

#if defined(BUILD_HAL_WANT_PMIC) && defined(HWCONFIG_HAVE_PMIC_MAXIM)
#define _ENABLE_PMIC_MAXIM
#endif

#if defined(BUILD_HAL_WANT_CHARGEMETER) && defined(HWCONFIG_HAVE_FUEL_GAUGE_MAXIM)
#define _ENABLE_CHARGEMETER_MAXIM
#endif

class HardwareServices FINAL
{
public:
    HardwareServices():
#ifdef _ENABLE_PMIC_MAXIM
        // NOTE: keep this first, so that sleep lock can be released when PMIC
        //       is launched first
        pmicDriver(HWCONFIG_LP_PMIC_I2C_BUS),
        //rrDriver(),
#endif
#ifdef BUILD_HAL_WANT_ACCELEROMETER
        accDriver(),
#endif
#ifdef BUILD_HAL_WANT_MAGNETOMETER
        magDriver(HWCONFIG_MAGNETOMETER_I2C_BUS),
#endif
#ifdef BUILD_HAL_WANT_PRESSURE
        presDriver(HWCONFIG_PRESSURE_I2C_BUS),
#endif
#ifdef BUILD_HAL_WANT_TMP112
        tmp112Driver(),
#endif
#ifdef BUILD_HAL_WANT_1WIRE
        ds24l65Provider(),
#endif
#ifdef BUILD_HAL_WANT_TOUCH
        touchDriver(HWCONFIG_TOUCH_I2C_BUS),
#endif
        chargeDriver(),

#ifdef BUILD_HAL_WANT_BUTTONS
        buttonProvider(),
#endif
#ifdef BUILD_HAL_WANT_GPS
        gpsProvider(),
#endif
#if defined (BUILD_HAL_WANT_BUZZER) || defined (BUILD_HAL_WANT_VIBRA)
        mUserFeedbackProvider(),
#endif
        dummy(0)
    {
    }

    const char *const*getProviderNames() const {
        static const char* const moduleList[] = {
#ifdef BUILD_HAL_WANT_PRESSURE
            hal::PressureProvider::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_ACCELEROMETER
            hal::AccelerometerProvider::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_MAGNETOMETER
            hal::MagnetometerProvider::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_TMP112
            hal::TemperatureService::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_1WIRE
            hal::DS24L65Provider::LAUNCHABLE_NAME,
#endif
#ifdef _ENABLE_PMIC_MAXIM
            hal::PmicProvider::LAUNCHABLE_NAME,
#endif
            hal::BatteryChargeProvider::LAUNCHABLE_NAME,
#ifdef BUILD_HAL_WANT_BUTTONS
            hal::ButtonProvider::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_GPS
            hal::GpsProvider::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_TOUCH
            nea::TouchProvider::LAUNCHABLE_NAME,
#endif
#if defined (BUILD_HAL_WANT_BUZZER) || defined (BUILD_HAL_WANT_VIBRA)
            hal::UserFeedbackProvider::LAUNCHABLE_NAME,
#endif
            NULL };
        return moduleList;
    }

//    hal::RrProvider rrDriver;

#ifdef _ENABLE_PMIC_MAXIM
    hal::PmicProvider pmicDriver;
#endif
#ifdef BUILD_HAL_WANT_ACCELEROMETER
    hal::AccelerometerProvider accDriver;
#endif
#ifdef BUILD_HAL_WANT_MAGNETOMETER
    hal::MagnetometerProvider magDriver;
#endif
#ifdef BUILD_HAL_WANT_PRESSURE
    hal::PressureProvider presDriver;
#endif
#ifdef BUILD_HAL_WANT_TMP112
    hal::TemperatureService tmp112Driver;
#endif
#ifdef BUILD_HAL_WANT_1WIRE
    hal::DS24L65Provider ds24l65Provider;
#endif
#ifdef BUILD_HAL_WANT_TOUCH
    hal::TouchProviderDriver touchDriver;
#endif

    hal::BatteryChargeProvider chargeDriver;

#ifdef BUILD_HAL_WANT_BUTTONS
    hal::ButtonProvider buttonProvider;
#endif

#ifdef BUILD_HAL_WANT_GPS
    // constructor will start the initialization cycle during which SPI1 becomes
    // unavailable for accelerometer. Accelerometer initialization is deferred
    // internally in the accel driver (horrible hack)
    hal::GpsProvider gpsProvider;
#endif
#if defined (BUILD_HAL_WANT_BUZZER) || defined (BUILD_HAL_WANT_VIBRA)
    hal::UserFeedbackProvider mUserFeedbackProvider;
#endif
    uint8 dummy;
};
