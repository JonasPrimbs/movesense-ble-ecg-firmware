///
//
//        Copyright (c) Suunto Oy 2015-2016.
//        All rights reserved.
//
///

#ifndef QUANTITY_FREQUENCY_H_
#define QUANTITY_FREQUENCY_H_

#include "quantity.h"

namespace quantity
{
namespace frequency
{
enum /* : Size */
{
    HERTZ = unit::BaseId, /* MUST be 1st */
    KILOHERTZ,
    MEGAHERTZ,
    BEAT_PER_MINUTE,
    REVOLUTION_PER_MINUTE,
    STROKES_PER_MINUTE,
    UNIT_COUNT

};

} // namespace frequency

///
// Type aliases
///

typedef Quantity<FREQUENCY, frequency::HERTZ> Hertz;
typedef Quantity<FREQUENCY, frequency::KILOHERTZ> Kilohertz;
typedef Quantity<FREQUENCY, frequency::MEGAHERTZ> Megahertz;
typedef Quantity<FREQUENCY, frequency::BEAT_PER_MINUTE> BeatPerMinute;
typedef Quantity<FREQUENCY, frequency::REVOLUTION_PER_MINUTE> RevolutionPerMinute;
typedef Quantity<FREQUENCY, frequency::STROKES_PER_MINUTE> StrokesPerMinute;

// Base unit
typedef Hertz Frequency;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<FREQUENCY>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_FREQUENCY_H_
