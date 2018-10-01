#pragma once
// Copyright (c) Suunto Oy 2018. All rights reserved.

#include "quantity.h"

namespace quantity
{
namespace electric_charge
{
enum /* : Size */
{
    COULOMB = unit::BaseId, /* MUST be 1st */
    AMPEREHOUR,
    MILLIAMPEREHOUR,
    UNIT_COUNT
};

} // namespace electric_charge

///
// Type aliases
///

typedef Quantity<ELECTRIC_CHARGE, electric_charge::COULOMB> Coulombs;
typedef Quantity<ELECTRIC_CHARGE, electric_charge::AMPEREHOUR> AmpereHours;
typedef Quantity<ELECTRIC_CHARGE, electric_charge::MILLIAMPEREHOUR> MilliampereHours;
// Base type
typedef Coulombs ElectricCharge;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<ELECTRIC_CHARGE>(const Id, const Id, const Value);

} // namespace unit
