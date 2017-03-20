#ifndef DATUMSUUNTO_H_
#define DATUMSUUNTO_H_

#include "fstypes.h"
#include "datum.h"

typedef DATUM Datum;

enum DatumParametersRetVal
{
	DATUM_PARAMETERS_FOUND = 0,
	DATUM_PARAMETERS_NOT_FOUND
};

enum DatumConversionRetVal
{
	DATUM_CONVERSION_FAILED = 0,
	DATUM_CONVERSION_SUCCEEDED

};

int DATUM_GetParameters(WORD datumID, DATUM *pDatum);
int DATUM_Convert(DOUBLE* pLat, DOUBLE* pLon, DOUBLE* pAlt, WORD datumID, WORD direction);

#endif /* DATUMSUUNTO_H_ */
