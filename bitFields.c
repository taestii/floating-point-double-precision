#include "bitFields.h"
#include <assert.h>

int getBit(int bn, unsigned long val)
{

	assert((0 <= bn) && (bn < 64));
	int shiftedVal = val >> bn; // value after being shifted
	int position = shiftedVal & 1;
	if (position == 1)
		return 1;
	else
		return 0;
	// TODO:
	//		check to make sure bn is a valid bit position in long (you may assert this is true)
	//		If the bit in val at position bn is 1, return 1
	//		otherwise return 0.
	return 0; // replace this with the correct code
}

void setBit(int bn, int new, unsigned long *val)
{
	assert((0 <= bn) && (bn <= 63));
	unsigned long bit;
	if (new == 1)
	{
		bit = 1;
		bit <<= bn;
		*val |= bit;

	}
	else
	{
		bit = 0;
		bit <<= bn;
		*val |= bit;
	}
	// TODO:
	//		check to make sure bn is a valid bit position in long (you may assert this is true)
	//		if new is true, set the bit in val at bn to 1
	//     otherwise, set the bit in val at bn to 0
}

long getBitFld(int bs, int len, unsigned long val)
{

	assert((0 <= bs) && (bs <= 63));
	assert((len > 0) && (len < 64));

	 long uBitField = getBitFldU(bs, len, val);

	 int highestBit = getBit(len - 1, uBitField); // shift right until at end of bit field

	if (highestBit != 0)
	{
		uBitField <<= (63 - len + 1);
		uBitField >>= (63 - len + 1);
		// what do i do
	}
	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	// 		perform an unsigned getBitFldU to extract the unsigned version of the field
	//		get the high order bit of the result... that's the "sign" bit
	//		if the sign bit not zero, replace the 0 padding with 1 padding
	return uBitField; // replace this with the correct code
}
unsigned long getBitFldU(int bs, int len, unsigned long val)
{

	assert((0 <= bs) && (bs <= 63));
	assert((len > 0) && (len < 64));

	unsigned long mask = (1L << len) - 1; // mask with len 1
	mask <<= ((bs - len) + 1);			   // shift, leftmost 1 starts at bs

	unsigned long bitField = (mask & val); // bitwise AND

	bitField >>= ((bs - len) + 1); // shift result to right

	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	// 		make a mask that has len 1's
	// 		Shift that mask left so it's leftmost 1 starts at bs
	//      bitwise and the mask and val to isolate the bit field bits
	//		Shift the result to the right so that the rightmost bit of the bit field is bit 0
	//			Note that shift right pads to the left with the sign bit if the field is signed, but
	//			for unsigned, it will pad to the left with 0
	return bitField; // replace this with the correct code
}

void setBitFld(int bs, int len, unsigned long new, unsigned long *val)
{

	assert((0 <= bs) && (bs <= 63));
	assert((len > 0) && (len < 64));

	unsigned long mask = (1L << len) - 1;

	new &= mask;

	new <<= ((bs - len) + 1);
	mask <<= ((bs - len) + 1);

	*val &= ~mask;
	*val |= new;

	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	//      Make a mask that has len bits, right justified
	//		Use mask to turn off any bits to the left of the bitField values in new
	// 		Shift both the new value and the mask to the correct bit position
	//		Turn off all bits in *val under the mask by bitwise anding with ~mask
	//		Or *val with new.  (new has zeros for all non bit field positions, so won't affect val)
}