///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_PRESSURE_H_
#define QUANTITY_PRESSURE_H_

#include "quantity.h"

// Undefine PASCAL calling convention definition
// from Windows headers
#undef PASCAL

namespace quantity
{
namespace pressure
{
enum /* : Size */
{
    PASCAL = unit::BaseId, /* MUST be 1st */
    HPASCAL,
    BAR,
    MILLIBAR,
    PSI,
    INHG,
    UNIT_COUNT

};

} // namespace pressure

///
// Type aliases
///

typedef Quantity<PRESSURE, pressure::PASCAL> Pascals;
typedef Quantity<PRESSURE, pressure::HPASCAL> HPascals;
typedef Quantity<PRESSURE, pressure::INHG> InHG;
typedef Quantity<PRESSURE, pressure::BAR> Bars;
typedef Quantity<PRESSURE, pressure::MILLIBAR> Millibars;
typedef Quantity<PRESSURE, pressure::PSI> Psi;
// Base unit
typedef Pascals Pressure;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<PRESSURE>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_LIGHTING_H_
