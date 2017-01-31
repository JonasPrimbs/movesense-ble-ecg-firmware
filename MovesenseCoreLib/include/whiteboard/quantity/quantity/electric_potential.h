///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_ELECTRIC_POTENTIAL_H_
#define QUANTITY_ELECTRIC_POTENTIAL_H_

#include "quantity.h"

namespace quantity
{
namespace electric_potential
{
enum /* : Size */
{
    VOLT = unit::BaseId, /* MUST be 1st */
    MILLIVOLT,
    UNIT_COUNT

};

} // namespace electric_potential

///
// Type aliases
///

typedef Quantity<ELECTRIC_POTENTIAL, electric_potential::VOLT> Volts;
typedef Quantity<ELECTRIC_POTENTIAL, electric_potential::MILLIVOLT> Millivolts;
// Base unit
typedef Volts ElectricPotential;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<ELECTRIC_POTENTIAL>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_ELECTRIC_POTENTIAL_H_
