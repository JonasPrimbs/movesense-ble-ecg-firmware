///
//
//        Copyright (c) Suunto Oy 2016.
//        All rights reserved.
//
///

#ifndef QUANTITY_SPEED_H_
#define QUANTITY_SPEED_H_

#include "distance.h"
#include "time.h"

namespace quantity
{
namespace speed
{
enum /* : Size */
{
    METERS_PER_SECOND = unit::BaseId, /* MUST be 1st */
    METERS_PER_MINUTE,
    METERS_PER_HOUR,
    KILOMETERS_PER_HOUR,
    FEET_PER_MINUTE,
    FEET_PER_HOUR,
    MILES_PER_HOUR,
    KNOTS,
    SECONDS_PER_METER,
    SECONDS_PER_KILOMETER,
    SECONDS_PER_MILE,
    SECONDS_PER_100_METERS,
    SECONDS_PER_500_METERS,
    SECONDS_PER_100_YARDS,
    UNIT_COUNT,
    FIRST_INVERTER_UNIT = SECONDS_PER_METER
};

} // namespace speed

///
// Type aliases
///

typedef Quantity<SPEED, speed::METERS_PER_SECOND> MetersPerSecond;
typedef Quantity<SPEED, speed::METERS_PER_MINUTE> MetersPerMinute;
typedef Quantity<SPEED, speed::METERS_PER_HOUR> MetersPerHour;
typedef Quantity<SPEED, speed::FEET_PER_MINUTE> FeetPerMinute;
typedef Quantity<SPEED, speed::FEET_PER_HOUR> FeetPerHour;
typedef Quantity<SPEED, speed::KILOMETERS_PER_HOUR> KilometersPerHour;
typedef Quantity<SPEED, speed::MILES_PER_HOUR> MilesPerHour;
typedef Quantity<SPEED, speed::KNOTS> Knots;
typedef Quantity<SPEED, speed::SECONDS_PER_METER> SecondsPerMeter;
typedef Quantity<SPEED, speed::SECONDS_PER_KILOMETER> SecondsPerKiloMeter;
typedef Quantity<SPEED, speed::SECONDS_PER_MILE> SecondsPerMile;
typedef Quantity<SPEED, speed::SECONDS_PER_100_METERS> SecondsPer100Meters;
typedef Quantity<SPEED, speed::SECONDS_PER_500_METERS> SecondsPer500Meters;
typedef Quantity<SPEED, speed::SECONDS_PER_100_YARDS> SecondsPer100Yards;


// Base unit
typedef MetersPerSecond Speed;

///
// Quantity operations
///

Speed operator/(const Distance& d, const Time& t);

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<SPEED>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_SPEED_H_
