///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_NUMERIC_H_
#define QUANTITY_NUMERIC_H_

#include "quantity.h"

namespace quantity
{
namespace numeric
{
enum /* : Size */
{
    SCALAR = unit::BaseId, /* MUST be 1st */
    PERCENT,
    UNIT_COUNT

};

} // namespace numeric

///
// Type aliases
///

typedef Quantity<NUMERIC, numeric::SCALAR> Scalar;
typedef Quantity<NUMERIC, numeric::PERCENT> Percent;
// Base unit
typedef Scalar Numeric;

///
// Quantity operations
///

template <Id Q, unit::Id U> Quantity<Q, U> operator*(const Numeric& n, const Quantity<Q, U>& q)
{
    return Quantity<Q, U>(q.getValue() * n.getValue());
}

template <Id Q, unit::Id U>
inline // associativity
    Quantity<Q, U>
    operator*(const Quantity<Q, U>& q, const Numeric& n)
{
    return n * q;
}

template <Id Q, unit::Id U> Quantity<Q, U> operator/(const Quantity<Q, U>& q, const Numeric& n)
{
    return Quantity<Q, U>(q.getValue() / n.getValue());
}

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<NUMERIC>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_NUMERIC_H_
