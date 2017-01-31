///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_ELECTRIC_CURRENT_H_
#define QUANTITY_ELECTRIC_CURRENT_H_

#include "quantity.h"

namespace quantity
{
namespace electric_current
{
enum /* : Size */
{
    AMPERE = unit::BaseId, /* MUST be 1st */
    MILLIAMPERE,
    MICROAMPERE,
    UNIT_COUNT

};

} // namespace electric_current

///
// Type aliases
///

typedef Quantity<ELECTRIC_CURRENT, electric_current::AMPERE> Amperes;
typedef Quantity<ELECTRIC_CURRENT, electric_current::MILLIAMPERE> Milliamperes;
typedef Quantity<ELECTRIC_CURRENT, electric_current::MICROAMPERE> Microamperes;
// Base type
typedef Amperes ElectricCurrent;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<ELECTRIC_CURRENT>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_ELECTRIC_CURRENT_H_
