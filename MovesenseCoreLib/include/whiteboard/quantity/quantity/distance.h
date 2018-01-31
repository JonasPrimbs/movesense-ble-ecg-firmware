///
//
//        Copyright (c) Suunto Oy 2015.
//        All rights reserved.
//
///

#ifndef QUANTITY_DISTANCE_H_
#define QUANTITY_DISTANCE_H_

#include "quantity.h"

namespace quantity
{
namespace distance
{
enum /* : Size */
{
    METER = unit::BaseId, /* MUST be 1st */
    KILO_METER,
    FOOT,
    KILO_FOOT,
    YARD,
    MILE,
    NAUTICAL_MILE,
    CENTI_METER,
    UNIT_COUNT

};

} // namespace distance

///
// Type aliases
///

typedef Quantity<DISTANCE, distance::METER> Meters;
typedef Quantity<DISTANCE, distance::KILO_METER> Kilometers;
typedef Quantity<DISTANCE, distance::FOOT> Feet;
typedef Quantity<DISTANCE, distance::KILO_FOOT> KiloFeet;
typedef Quantity<DISTANCE, distance::YARD> Yards;
typedef Quantity<DISTANCE, distance::MILE> Miles;
typedef Quantity<DISTANCE, distance::NAUTICAL_MILE> NauticalMiles;
typedef Quantity<DISTANCE, distance::CENTI_METER> Centimeters;
// Base unit
typedef Meters Distance;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<DISTANCE>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_DISTANCE_H_
