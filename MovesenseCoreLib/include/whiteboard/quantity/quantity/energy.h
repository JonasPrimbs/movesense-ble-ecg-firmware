///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_ENERGY_H_
#define QUANTITY_ENERGY_H_

#include "quantity.h"

namespace quantity
{
namespace energy
{
enum /* : Size */
{
    JOULE = unit::BaseId, /* MUST be 1st */
    KCAL,
    KNM,
    UNIT_COUNT

};

} // namespace energy

///
// Type aliases
///

typedef Quantity<ENERGY, energy::JOULE> Joules;
typedef Quantity<ENERGY, energy::KNM> KiloNewtonMeters;

// Base unit
typedef Joules Energy;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<ENERGY>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_ENERGY_H_
