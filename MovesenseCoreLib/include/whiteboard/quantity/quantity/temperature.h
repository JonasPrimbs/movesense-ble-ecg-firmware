///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_TEMPERATURE_H_
#define QUANTITY_TEMPERATURE_H_

#include "quantity.h"

namespace quantity
{
namespace temperature
{
enum /* : Size */
{
    KELVIN = unit::BaseId, /* MUST be 1st */
    CELSIUS,
    FAHRENHEIT,
    UNIT_COUNT

};

} // namespace temperature

///
// Type aliases
///

typedef Quantity<TEMPERATURE, temperature::KELVIN> Kelvins;
typedef Quantity<TEMPERATURE, temperature::CELSIUS> Celsius;
typedef Quantity<TEMPERATURE, temperature::FAHRENHEIT> Fahrenheits;

// Base unit
typedef Kelvins Temperature;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<TEMPERATURE>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_TEMPERATURE_H_
