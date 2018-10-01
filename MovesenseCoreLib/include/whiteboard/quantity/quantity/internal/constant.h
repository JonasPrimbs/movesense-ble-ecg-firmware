///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_INTERNAL_CONSTANT_H_
#define QUANTITY_INTERNAL_CONSTANT_H_

#include "quantity/quantity.h"

namespace constant
{
using namespace unit;

const Value BASE_UNIT = Value(1);

// Acceleration
const Value GRAVITY_OF_EARTH_IN_METERS_PER_SECOND_SQUARED = Value(9.80665);

// Speed 
const Value FEET_PER_MINUTES_IN_METERS_PER_SECOND = Value(0.0050800001);
const Value FEET_PER_HOUR_IN_METERS_PER_SECOND = Value(8.4667e-5);
const Value KNOTS = Value(0.51444563339);

// Distance
const Value FOOT_IN_METERS = Value(0.3048);
const Value KILO_FOOT_IN_METERS = Value(304.8);
const Value KM_IN_METERS = Value(1000.0);
const Value YARD_IN_METERS = Value(0.9144);
const Value MILE_IN_METERS = Value(1609.355);
const Value NAUTICAL_MILE_IN_METERS = Value(1852.0);
const Value CM_IN_METERS = Value(0.01);

// Mass
const Value POUND_IN_KILOGRAMS = Value(0.45359237);

// Numeric
const Value PERCENT = Value(0.01);

// Pressure
const Value HECTO_PASCAL = Value(100.0);
const Value BAR_IN_PASCAL = Value(1e5);
const Value PSI_IN_PASCAL = Value(6894.75729);
const Value MILLIBAR_IN_PASCAL = Value(100);
const Value INHG_IN_PASCAL = Value(3386.3886667);

// Electric charge
const Value AMPERE_HOUR_IN_COULOMBS = Value(3600.0);

// Energy
const Value KCALS_IN_JOULE = Value(4183.99538087);
const Value KNM_IN_JOULE = Value(1000.0);

// Time
const Value MINUTE_IN_SECONDS = Value(60);
const Value HOUR_IN_SECONDS = Value(3600);
const Value DAY_IN_SECONDS = Value(86400);

} // namespace constant

#endif // QUANTITY_INTERNAL_CONSTANT_H_
