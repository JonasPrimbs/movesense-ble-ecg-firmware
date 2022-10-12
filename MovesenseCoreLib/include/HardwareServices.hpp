// Copyright (c) Suunto Oy 2016. All rights reserved.
#pragma once
#include "buildconfig/hal/features.h"
#include "hwconfig.h"

#ifdef BUILD_HAL_WANT_ACCELEROMETER
#include "hal/sensor/accelerometer/AccelerometerProvider.hpp"
#endif

#ifdef BUILD_HAL_WANT_MAGNETOMETER
#include "hal/sensor/magnetometer/ak09912/MagnetometerProvider.hpp"
#endif // def HAL_WANT_MAGNETOMETER

#ifdef BUILD_HAL_WANT_PRESSURE
#include "hal/sensor/pressureSensor/ms5837-02ba/PressureProvider.hpp"
#endif // def HAL_WANT_PRESSURE

#if defined(BUILD_HAL_TEMPERATURE_DRIVER_TMP112)
#include "hal/sensor/temperature/tmp112/TemperatureProvider.hpp"
#elif defined(BUILD_HAL_TEMPERATURE_DRIVER_SIMU)
#include "hal/sensor/temperature/simulator/TemperatureProvider.hpp"
#endif

#if defined(BUILD_HAL_HEARTRATE_DRIVER_MAX3000X)
#include "hal/sensor/AFE/MAX3000x/MAX30004_provider.hpp"
#elif defined(BUILD_HAL_HEARTRATE_DRIVER_SIMU)
#include "hal/sensor/AFE/MAX3000X_simu/MAX30004_provider.hpp"
#endif

#ifdef BUILD_HAL_1WIRE_DRIVER_DS24165
#include "hal/sensor/security/ds24l65/DS24L65_provider.hpp"
#endif

#ifdef BUILD_HAL_WANT_SKISENSE_MSM
#include "hal/sensor/msm/SkisenseMSM_provider.hpp"
#endif // def BUILD_HAL_WANT_SKISENSE_MSM


#if defined(BUILD_HAL_BATTERY_GAUGE_DRIVER_SIMU)
#include "hal/sensor/batteryGauge/simu/BatteryChargeProvider.hpp"
#elif defined(BUILD_HAL_BATTERY_GAUGE_DRIVER_MAX17055)
#include "hal/sensor/batteryGauge/max17055/BatteryChargeProvider.hpp"
#elif defined(BUILD_HAL_BATTERY_GAUGE_DRIVER_SS2)
#include "hal/sensor/batteryGauge/SS2/BatteryChargeProvider.hpp"
#endif

// include hwconfig specific restrictions (not all hardware is present in all
// designs)
#include "hwconfig.h"

#ifdef BUILD_HAL_PMIC_DRIVER_BQ25120
#include "hal/pmic/bq25120/BQPmicService.hpp"
#endif

#if defined(BUILD_HAL_PMIC_DRIVER_BQ25120) && defined(BUILD_HAL_BATTERY_GAUGE_DRIVER_MAX17055)
#include "LowBatteryShipmodeService.hpp"
#endif

/*
 * These services are NOT deinitialized on mode switch
 */

class CoreHardwareServices FINAL
{
public:
    CoreHardwareServices():
#ifdef BUILD_HAL_PMIC_DRIVER_BQ25120
        bqPmicDriver(),
#endif
#ifdef BUILD_HAL_WANT_CHARGEMETER
        chargeDriver(),
#endif
#if defined(BUILD_HAL_PMIC_DRIVER_BQ25120) && defined(BUILD_HAL_BATTERY_GAUGE_DRIVER_MAX17055)
        lowBatteryShutdown(),
#endif
        dummy(0)
        {
        }

    const char *const*getProviderNames() const {
        static const char_t* const moduleList[] = {
#ifdef BUILD_HAL_PMIC_DRIVER_BQ25120
            BQPmicService::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_CHARGEMETER
            hal::BatteryChargeProvider::LAUNCHABLE_NAME,
#endif
#if defined(BUILD_HAL_PMIC_DRIVER_BQ25120) && defined(BUILD_HAL_BATTERY_GAUGE_DRIVER_MAX17055)
            LowBatteryShipmodeService::LAUNCHABLE_NAME,
#endif
            NULL };
        return moduleList;
    }

#ifdef BUILD_HAL_PMIC_DRIVER_BQ25120
    BQPmicService bqPmicDriver;
#endif
#ifdef BUILD_HAL_WANT_CHARGEMETER
    hal::BatteryChargeProvider chargeDriver;
#endif
#if defined(BUILD_HAL_PMIC_DRIVER_BQ25120) && defined(BUILD_HAL_BATTERY_GAUGE_DRIVER_MAX17055)
    LowBatteryShipmodeService lowBatteryShutdown;
#endif
    uint8 dummy;
};

/*
 * These services are deinitialized on mode switch
 */
class HardwareServices FINAL
{
public:
    HardwareServices():
#if defined(BUILD_HAL_WANT_ACCELEROMETER) && !defined(BUILD_HAL_WANT_SKISENSE_MSM)
        accDriver(),
#endif
#ifdef BUILD_HAL_WANT_MAGNETOMETER
        magDriver(HWCONFIG_MAGNETOMETER_I2C_BUS),
#endif
#ifdef BUILD_HAL_WANT_PRESSURE
        presDriver(HWCONFIG_PRESSURE_I2C_BUS),
#endif
#ifdef BUILD_HAL_WANT_TEMPERATURE
        tmp112Driver(),
#endif
#ifdef BUILD_HAL_1WIRE_DRIVER_DS24165
        ds24l65Provider(),
#endif
#ifdef BUILD_HAL_WANT_SKISENSE_MSM
        skisenseMSMProvider(),
#endif
        dummy(0)
    {
    }

    const char *const*getProviderNames() const {
        static const char_t* const moduleList[] = {
#ifdef BUILD_HAL_WANT_PRESSURE
            hal::PressureProvider::LAUNCHABLE_NAME,
#endif
#if defined(BUILD_HAL_WANT_ACCELEROMETER) && !defined(BUILD_HAL_WANT_SKISENSE_MSM)
            hal::AccelerometerProvider::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_MAGNETOMETER
            hal::MagnetometerProvider::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_TEMPERATURE
            hal::TemperatureService::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_HEARTRATE
            hal::MAX30004Provider::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_1WIRE_DRIVER_DS24165
            hal::DS24L65Provider::LAUNCHABLE_NAME,
#endif
#ifdef BUILD_HAL_WANT_SKISENSE_MSM
            hal::SkisenseMSMProvider::LAUNCHABLE_NAME,
#endif
            NULL };
        return moduleList;
    }

#if defined(BUILD_HAL_WANT_ACCELEROMETER) && !defined(BUILD_HAL_WANT_SKISENSE_MSM)
    hal::AccelerometerProvider accDriver;
#endif
#ifdef BUILD_HAL_WANT_MAGNETOMETER
    hal::MagnetometerProvider magDriver;
#endif
#ifdef BUILD_HAL_WANT_PRESSURE
    hal::PressureProvider presDriver;
#endif
#ifdef BUILD_HAL_WANT_TEMPERATURE
    hal::TemperatureService tmp112Driver;
#endif
#ifdef BUILD_HAL_WANT_HEARTRATE
    hal::MAX30004Provider max30004Provider;
#endif
#ifdef BUILD_HAL_1WIRE_DRIVER_DS24165
    hal::DS24L65Provider ds24l65Provider;
#endif
#ifdef BUILD_HAL_WANT_SKISENSE_MSM
    hal::SkisenseMSMProvider skisenseMSMProvider;
#endif
    uint8 dummy;
};
