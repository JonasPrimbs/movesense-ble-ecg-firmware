///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///
#pragma once
#include <stdint.h> // uint8_t

#define UNIT_VALUE_TYPE float

namespace quantity
{
/** Size type for quantityID.
*/
typedef uint8_t Size;

/** Quantity ID type.
*/
typedef Size Id;

/** List of needed quantities. Add new ones to bottom of the
* list to keep IDs backward compatible.
*/
enum /* : Size */
{
    ACCELERATION,
    ANGLE,
    ANGULAR_VELOCITY,
    DISTANCE,
    ELECTRIC_CURRENT,
    ELECTRIC_POTENTIAL,
    ENERGY,
    FORCE,
    FREQUENCY,
    LIGHTING,
    MAGNETIC_FLUX_DENSITY,
    MASS,
    NUMERIC, /* Dimensionless */
    POWER,
    PRESSURE,
    SPEED,
    TEMPERATURE,
    TIME,
    ELECTRIC_CHARGE,

    QUANTITY_COUNT
};

} // namespace quantity

/** Unit types and definitions.
*/
namespace unit
{
/** Size type for quantityID.
*/
typedef uint8_t Size;

/** Unit ID type.
*/
typedef Size Id;

/** Unit symbol type.
*/
typedef const char* Symbol;

/** Unit value type.
*/
typedef UNIT_VALUE_TYPE Value;

/** SI prefixes for scaling.
*/
namespace prefix
{
// multipliers
const Value Mega = Value(1e6);
const Value Kilo = Value(1e3);
const Value Hecto = Value(1e2);
const Value Deca = Value(1e1);
// divisors
const Value Deci = Value(1e-1);
const Value Centi = Value(1e-2);
const Value Milli = Value(1e-3);
const Value Micro = Value(1e-6);

} // namespace prefix

/** Unit info type for queries.
*/
struct Info
{
    /** Constructs Info structure from given quantity and unit IDs 
     *
     * @param _qId Quantity ID
     * @param _uId Unit ID
     * @return Info structure instance
     */
    static Info to_info(quantity::Id _qId, unit::Id _uId)
    {
        Info i = {_qId, _uId};
        return i;
    }

    /** Constructs Info structure from given numeric value that represents quantity and unit IDs
    *
    * @param value Numeric value that represents quantity and unit IDs
    * @return Info structure instance
    */
    static Info to_info(uint16_t value)
    {
        Info i = { static_cast<quantity::Id>(value >> 8), static_cast<unit::Id>(value) };
        return i;
    }

    /** Converts the info structure to single numeric value
     *
     * @return Single numberic value representing the quantity unit info
     */
    inline operator uint16_t() const
    {
        return (static_cast<uint16_t>(qId) << 8) | uId;
    }

    quantity::Id qId;
    unit::Id uId;
};

bool operator==(const Info&, const Info&);
bool operator!=(const Info&, const Info&);
const char* toString(const unit::Info&);

/** Quantity and unit are undefined.
*/
const Info None = {static_cast<quantity::Id>(-1), static_cast<unit::Id>(-1)};

/** Base unit index is 0.
*/
const Id BaseId = 0;

/** Quantity is numeric scalar.
*/
const Info ScalarQuantity = {quantity::NUMERIC, BaseId};

/** Dynamic conversion between two units of the same quantity.
* @return true if conversion is successful.
*/
bool tryConvert(const Info& from, const Info& to, const Value value, Value& result);

/** Dynamic conversion between two units of the same quantity.
* Quantity ID must be known at compile time.
* @return true if conversion is successful.
*/
template <quantity::Id Q> bool tryConvert(const Id from, const Id to, const Value value, Value& result);

/** Conversion between two units of the same quantity.
* Quantity ID must be known at compile time.
*/
template <quantity::Id Q> Value convert(const Id from, const Id to, const Value value);

/** Unit info to 'standard' unit symbol.
*/
bool toSymbol(const Info&, Symbol&);

/** Unit ID to 'standard' unit symbol.
* Quantity ID must be known at compile time.
*/
template <quantity::Id Q> bool toSymbol(const Id, Symbol&);

/** Unit symbol (one of many) to unit info.
* @return true if successful.
*/
bool parseSymbol(const Symbol&, Info&);

/** Unit symbol (one of many) ot unit info.
* Quantity ID must be known at compile time.
* @ return true if successful.
*/
template <quantity::Id Q> bool parseSymbol(const Symbol&, Info&);

} // namespace unit

#include "internal/quantity.hpp"
