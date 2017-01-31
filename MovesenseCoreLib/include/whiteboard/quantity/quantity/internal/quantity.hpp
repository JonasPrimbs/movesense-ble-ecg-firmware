///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_INTERNAL_QUANTITY_HPP_
#define QUANTITY_INTERNAL_QUANTITY_HPP_

namespace quantity
{
	/** Value class representing physical quantity.
	* Physical quantitity consists of numeric value and unit.
	* - Conversion to base when assigning two units of the same quantity.
	* - Value is stored in base unit to minimize loss of precision.
	* - Value is converted to named unit when getValue() is called.
	*/
	template <Id QuantityId, unit::Id UnitId>
	class Quantity
	{
	public:// Life-cycle
		/* constexpr */ explicit Quantity(const unit::Value& value = unit::Value())
			: mBaseValue(unit::convert<QuantityId>(UnitId, unit::BaseId, value)) {}

	public:// Conversion
		template <unit::Id FromUnitId>
		/* constexpr */ Quantity(const Quantity<QuantityId, FromUnitId>& from) :
			mBaseValue(unit::convert<QuantityId>(FromUnitId, unit::BaseId, from.getValue())) {}

	public:// Interface
		unit::Value getValue() const { return unit::convert<QuantityId>(unit::BaseId, UnitId, mBaseValue); }
		Id getQuantityId() const { return QuantityId; }
		unit::Id getUnitId() const { return UnitId; }

	private:// Data
		unit::Value mBaseValue;

	};// Quantity

	///
	// Comparison operations
	///

	template <Id Q, unit::Id U1, unit::Id U2>
	bool operator == (const Quantity<Q, U1>& q1, const Quantity<Q, U2>& q2)
	{
		return q1.getValue() == static_cast<Quantity<Q, unit::BaseId> >(q2).getValue();
	}

	template <Id Q, unit::Id U1, unit::Id U2>
	inline// associativity
	bool operator != (const Quantity<Q, U1>& q1, const Quantity<Q, U2>& q2)
	{
		return !(q1 == q2);
	}

	///
	// Ordering operations
	///

	template <Id Q, unit::Id U1, unit::Id U2>
	bool operator < (const Quantity<Q, U1>& q1, const Quantity<Q, U2>& q2)
	{
		return q1.getValue() < static_cast<Quantity<Q, unit::BaseId> >(q2).getValue();
	}

	template <Id Q, unit::Id U1, unit::Id U2>
	inline// associativity
	bool operator > (const Quantity<Q, U1>& q1, const Quantity<Q, U2>& q2)
	{
		return q2 < q1;
	}

	template <Id Q, unit::Id U1, unit::Id U2>
	inline// associativity
	bool operator <= (const Quantity<Q, U1>& q1, const Quantity<Q, U2>& q2)
	{
		return q1 < q2 || q1 == q2;
	}

	template <Id Q, unit::Id U1, unit::Id U2>
	inline// associativity
	bool operator >= (const Quantity<Q, U1>& q1, const Quantity<Q, U2>& q2)
	{
		return q2 <= q1;
	}

	///
	// Adding and subtracting same units
	///

	template <Id Q, unit::Id U>
	Quantity<Q, U> operator + (const Quantity<Q, U>& q1, const Quantity<Q, U>& q2)
	{
		return Quantity<Q, U>(q1.getValue() + q2.getValue());
	}

	template <Id Q, unit::Id U>
	Quantity<Q, U> operator - (const Quantity<Q, U>& q1, const Quantity<Q, U>& q2)
	{
		return Quantity<Q, U>(q1.getValue() - q2.getValue());
	}

	///
	// Adding and subtracting different units
	///

	template <Id Q, unit::Id U1, unit::Id U2>
	inline// -> adding same units
	Quantity<Q, unit::BaseId> operator + (const Quantity<Q, U1>& q1, const Quantity<Q, U2>& q2)
	{
		return static_cast<Quantity<Q, unit::BaseId> >(q1) + static_cast<Quantity<Q, unit::BaseId> >(q2);
	}

	template <Id Q, unit::Id U1, unit::Id U2>
	inline// -> subtracting same units
	Quantity<Q, unit::BaseId> operator - (const Quantity<Q, U1>& q1, const Quantity<Q, U2>& q2)
	{
		return static_cast<Quantity<Q, unit::BaseId> >(q1) - static_cast<Quantity<Q, unit::BaseId> >(q2);
	}

}// namespace quantity

#endif//QUANTITY_INTERNAL_QUANTITY_HPP_
