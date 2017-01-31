///
//
//		Copyright (c) Suunto Oy 2015.
//		All rights reserved.
//
///

#ifndef QUANTITY_INTERNAL_UNIT_H_
#define QUANTITY_INTERNAL_UNIT_H_

#include "quantity/quantity.h"
#include "quantity/internal/constant.h"

#include <string.h>

#define CASE_UNIT_TO_STRING(qId, uId)                                                                                            \
    case uId:                                                                                                                    \
        return #qId ", quantity::" #uId

namespace unit
{
/** 'Standard' symbol index in symbol table.
*/
const Size SymbolIndex = 0;

/** Symbol count per unit in symbol table.
*/
const Size SymbolCount = 2;

Value convert(const Value[], const Size, const Id, const Id, const Value, bool invert = false);

bool parseSymbol(const quantity::Id, const Symbol[][SymbolCount], const Size, const Symbol&, Info&);

bool toSymbol(const Id, const Symbol[][SymbolCount], const Size, Symbol&);

bool isConversionValid(const Size, const Id, const Id);

bool isConversionNeeded(const Size, const Id, const Id);

template <quantity::Id> const char* toString(const unit::Id uId);

} // namespace unit

#endif // QUANTITY_INTERNAL_UNIT_H_
