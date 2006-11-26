#include "math.h"
#include "types.h"

#ifdef LACK_ISINFF
/* Copied from OpenBSD's libm. */
typedef union
{
	float value;
	u_int32_t word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i,d) do					\
{								\
	ieee_float_shape_type gf_u;				\
	gf_u.value = (d);					\
	(i) = gf_u.word;					\
} while(0)

int isinff(float x)
{
	int32_t ix;
	GET_FLOAT_WORD(ix, x);
	ix &= 0x7fffffff;
	ix ^= 0x7f800000;
	return ix == 0;
}
#endif
