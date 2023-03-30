#include "floatx.h"
#include <assert.h>
#include <limits.h> // for CHAR_BIT - number of bits per byte
#include <math.h>	// for isinf and isnan
#include "bitFields.h"
#include "bitFields.c"

typedef union
{
	double dVal;
	unsigned long lVal;
} undl;

floatx doubleToFloatx(double val, int totBits, int expBits)
{
	/*-----------------------------------------------------------------------------------------------
	From the README: At a high level, doing the conversion requires several manipulations:
	1. Extracting the sign bit from the double value, and inserting it into the floatx value at
	the correct position.
	2. Handle special cases, such as 0.0, infinity, or not-a-number.
	3. Extract the biased exponent from the double value.
	Check to see if the double value is sub-normal. If so, handle it.
	Check to make sure the floatx exponent won't overflow or underflow).
	If so, handle as a special case.
	If not, rebias the exponent using the floatx bias (which depends on the number of exponent bits)
	and write the result to the correct location in the floatx result.
	4. Extract the fraction bits from the double value.
	Determine how many bits are available for the fraction in the floatx value,
	and truncate or extend the original value,
	and write the resulting bits to the floatx result.
	6. Return the floatx result.
	----------------------------------------------------------------------------------------------------*/
	// First, make some assertions to ensure the totBits and expBits parameters are OK
	// Then, implement the algorithm

	assert(expBits >= 3 && totBits >= expBits + 1);
	// expBits needs to be at least 3, a signed exponent
	// totBits: needs to have space for signed bit, expBits, and mantissa

	floatx floatxValue = 0;
	undl unionVal;
	unionVal.dVal = val;

	int mantissaLen = totBits - expBits - 1;
	int mantissaIndex = totBits - expBits - 2;
	int mantissaSize = 52;

	setBit(totBits - 1, getBit(63, unionVal.lVal), &floatxValue);
	// set unsigned bit

	unsigned long exponent = getBitFldU(62, 11, unionVal.lVal);
	// get the bit field for the exponent, which is up to 11 bits long, and starts at bit 62
	// takes from value

	unsigned long mantissa = getBitFldU(mantissaSize - 1, mantissaSize, unionVal.lVal);
	// get bit field for mantissa, is up to 52 bits long, starts at bit 52

	unsigned long mask = 1;
	mask <<= 11;
	mask -= 1;

	if ((exponent & mask) == mask) // check if inf/nan
	{
		mask = 0;
		mask -= 1;

		setBitFld(totBits - 2, expBits, mask, &floatxValue); // changes exp bits to 1

		if (mantissa == 0) // if inf
		{
			setBitFld(mantissaIndex, mantissaLen, 0, &floatxValue);
		}
		else // if nan
		{
			setBitFld(mantissaIndex, mantissaLen, mask, &floatxValue);
		}

		return floatxValue;
	}

	long longExp = exponent;

	if (exponent == 0) // check if denormalized num
	{
		if (mantissa == 0) /// if mantissa is 0 as well, value is 0.0
		{
			setBitFld(totBits - 2, totBits - 1, 0, &floatxValue);
			return floatxValue; // done
		}
		// if denormal, leading digit is assumed to be zero
		/// begin shifting bits to the left by the num of leading zero digits
		int leadingZeroes = 0;

		while (leadingZeroes < mantissaSize && !getBit(mantissaSize - 1 - leadingZeroes, mantissa))
		{					 // check if leadingZeroes is under 52 and if current bit is 0
			leadingZeroes++; // continue until leading 1 is found
		}

		longExp -= leadingZeroes + 1; // find

		for (int i = -1; i < leadingZeroes; i++)
		{
			mantissa <<= 1; // shifts leading 1 to the front
		}
	}

	unsigned long bias = 1;
	bias <<= (expBits - 1); // 2^(expBits - 1) - 1
	bias--;

	longExp = longExp - 1023 + bias; // find double point exp version


	if (longExp <= 0)
	{
		// Denormalized number
		mantissa = (mantissa >> (1 - longExp)) | (1 << mantissaSize);
		longExp = 0;
	}
	else if (longExp >= ((1 << expBits) - 1))
	{
		// Exponent overflow
		exponent = (1 << expBits) - 1;
		setBitFld(totBits - 2, expBits, exponent, &floatxValue);
		setBitFld(mantissaIndex, mantissaLen, 0, &floatxValue);
		return floatxValue;
	}

	setBitFld(totBits - 2, expBits, longExp, &floatxValue);
	setBitFld(mantissaIndex, mantissaLen, mantissa, &floatxValue);

	exponent = longExp;
	setBitFld(totBits - 2, expBits, exponent, &floatxValue);

	if ((mantissaLen) < mantissaSize)
	{
		mantissa >>= (mantissaSize - (mantissaLen));
	}
	else
	{
		mantissa <<= (totBits - expBits - 53);
	}
	setBitFld(mantissaIndex, mantissaLen, mantissa, &floatxValue);

	return floatxValue;
}