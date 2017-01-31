///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_POWER_H_
#define QUANTITY_POWER_H_

#include "quantity.h"

namespace quantity
{
namespace power
{
enum /* : Size */
{
    WATT = unit::BaseId, /* MUST be 1st */
    UNIT_COUNT

};

} // namespace power

///
// Type aliases
///

typedef Quantity<POWER, power::WATT> Watts;
// Base unit
typedef Watts Power;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<POWER>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_POWER_H_
