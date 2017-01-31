///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_TIME_H_
#define QUANTITY_TIME_H_

#include "quantity.h"

namespace quantity
{
namespace time
{
enum /* : Size */
{
    SECOND = unit::BaseId, /* MUST be 1st */
    MINUTE,
    HOUR,
    DAY,
    MILLISECOND,
    MICROSECOND,
    UNIT_COUNT

};

} // namespace time

///
// Type aliases
///

typedef Quantity<TIME, time::SECOND> Seconds;
typedef Quantity<TIME, time::MINUTE> Minutes;
typedef Quantity<TIME, time::HOUR> Hours;
typedef Quantity<TIME, time::DAY> Days;
typedef Quantity<TIME, time::MILLISECOND> Milliseconds;
typedef Quantity<TIME, time::MICROSECOND> Microseconds;
// Base unit
typedef Seconds Time;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<TIME>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_TIME_H_
