///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_FORCE_H_
#define QUANTITY_FORCE_H_

#include "quantity.h"

namespace quantity
{
namespace force
{
enum /* : Size */
{
    NEWTON = unit::BaseId, /* MUST be 1st */
    UNIT_COUNT

};

} // namespace force

///
// Type aliases
///

typedef Quantity<FORCE, force::NEWTON> Newtons;
// Base unit
typedef Newtons Force;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<FORCE>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_FORCE_H_
