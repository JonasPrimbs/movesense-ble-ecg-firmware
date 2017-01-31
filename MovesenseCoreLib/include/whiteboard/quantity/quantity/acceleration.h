///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_ACCELERATION_H_
#define QUANTITY_ACCELERATION_H_

#include "quantity.h"

namespace quantity
{
namespace acceleration
{
enum /* : Size */
{
    METERS_PER_SECOND_SQUARED = unit::BaseId, /* MUST be 1st */
    GRAVITY_OF_EARTH,
    UNIT_COUNT

};

} // namespace acceleration

///
// Type aliases
///

typedef Quantity<ACCELERATION, acceleration::METERS_PER_SECOND_SQUARED> MetersPerSecondSquared;
typedef Quantity<ACCELERATION, acceleration::GRAVITY_OF_EARTH> GravityOfEarth;
// Base unit
typedef MetersPerSecondSquared Acceleration;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<ACCELERATION>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_ACCELERATION_H_
