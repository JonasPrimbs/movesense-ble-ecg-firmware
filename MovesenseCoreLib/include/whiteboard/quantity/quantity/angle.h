///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_ANGLE_H_
#define QUANTITY_ANGLE_H_

#include "quantity.h"

namespace quantity
{
namespace angle
{
enum /* : Size */
{
    RADIAN = unit::BaseId, /* MUST be 1st */
    DEGREE,
    MIL,
    UNIT_COUNT

};

} // namespace angle

///
// Type aliases
///

typedef Quantity<ANGLE, angle::RADIAN> Radians;
typedef Quantity<ANGLE, angle::DEGREE> Degrees;
typedef Quantity<ANGLE, angle::MIL> Mils;

// Base unit
typedef Radians Angle;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<ANGLE>(const Id, const Id, const Value);

} // namespace unit

namespace constant
{
const unit::Value Pi = unit::Value(3.1415926535897932385);

} // namespace constant

#endif // QUANTITY_ANGLE_H_
