///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_ANGULAR_VELOCITY_H_
#define QUANTITY_ANGULAR_VELOCITY_H_

#include "angle.h"
#include "time.h"

namespace quantity
{
namespace angular_velocity
{
enum /* : Size */
{
    RADIANS_PER_SECOND = unit::BaseId, /* MUST be 1st */
    DEGREES_PER_SECOND,
    UNIT_COUNT

};

} // namespace angular_velocity

///
// Type aliases
///

typedef Quantity<ANGULAR_VELOCITY, angular_velocity::RADIANS_PER_SECOND> RadiansPerSecond;
typedef Quantity<ANGULAR_VELOCITY, angular_velocity::DEGREES_PER_SECOND> DegreesPerSecond;
// Base unit
typedef RadiansPerSecond AngularVelocity;

///
// Quantity operations
///

AngularVelocity operator/(const Angle& a, const Time& t);

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<ANGULAR_VELOCITY>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_ANGULAR_VELOCITY_H_
