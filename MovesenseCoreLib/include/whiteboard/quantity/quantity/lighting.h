///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///
#ifndef QUANTITY_LIGHTING_H_
#define QUANTITY_LIGHTING_H_

#include "quantity.h"

namespace quantity
{
namespace lighting
{
enum /* : Size */
{
    LUX = unit::BaseId, /* MUST be 1st */
    UNIT_COUNT

};

} // namespace lighting

///
// Type aliases
///

typedef Quantity<LIGHTING, lighting::LUX> Lux;
// Base unit
typedef Lux Lighting;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<LIGHTING>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_LIGHTING_H_
