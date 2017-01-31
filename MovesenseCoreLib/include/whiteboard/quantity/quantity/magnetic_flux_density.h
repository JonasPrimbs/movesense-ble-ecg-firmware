///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_MAGNETIC_FLUX_DENSITY_H_
#define QUANTITY_MAGNETIC_FLUX_DENSITY_H_

#include "quantity.h"

namespace quantity
{
namespace magnetic_flux_density
{
enum /* : Size */
{
    TESLA = unit::BaseId, /* MUST be 1st */
    MILLITESLA,
    MICROTESLA,
    UNIT_COUNT

};

} // namespace magnetic_flux_density

///
// Type aliases
///

typedef Quantity<MAGNETIC_FLUX_DENSITY, magnetic_flux_density::TESLA> Teslas;
typedef Quantity<MAGNETIC_FLUX_DENSITY, magnetic_flux_density::MILLITESLA> Milliteslas;
typedef Quantity<MAGNETIC_FLUX_DENSITY, magnetic_flux_density::MICROTESLA> Microteslas;
// Base unit
typedef Teslas MagneticFluxDensity;

} // namespace quantity

namespace unit
{
using namespace quantity;

template <> Value convert<MAGNETIC_FLUX_DENSITY>(const Id, const Id, const Value);

} // namespace unit

#endif // QUANTITY_MAGNETIC_FLUX_DENSITY_H_
