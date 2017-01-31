///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_MASS_H_
#define QUANTITY_MASS_H_

#include "quantity.h"

namespace quantity
{
namespace mass
{
enum /* : Size */
{
    KILOGRAM = unit::BaseId, /* MUST be 1st */
    POUND,
    UNIT_COUNT

};

} // namespace mass

///
// Type aliases
///

typedef Quantity<MASS, mass::KILOGRAM> Kilograms;
typedef Quantity<MASS, mass::POUND> Pounds;
// Base unit
typedef Kilograms Mass;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<MASS>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_MASS_H_
