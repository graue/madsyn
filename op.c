/* Defining operators. */

#include <stddef.h>
#include <math.h>
#include "data.h"

#define SIMPLE_FUNC(name)						\
static void op_ ## name (const frame_t *in, frame_t *out, void *state)	\
{									\
	(void)state;							\
	out[0][0] = name(in[0][0]);					\
	out[0][1] = name(in[0][1]);					\
}

SIMPLE_FUNC(sin)
SIMPLE_FUNC(cos)
SIMPLE_FUNC(log)
SIMPLE_FUNC(fabs)

#define OPER_FUNC(name, oper)						\
static void op_ ## name (const frame_t *in, frame_t *out, void *state)	\
{									\
	(void)state;							\
	out[0][0] = in[0][0] oper in[1][0];				\
	out[0][1] = in[0][1] oper in[1][1];				\
}

OPER_FUNC(add, +)
OPER_FUNC(sub, -)
OPER_FUNC(mul, *)

#define OPER_FUNC2(name, func)						\
static void op_ ## name (const frame_t *in, frame_t *out, void *state)	\
{									\
	(void)state;							\
	out[0][0] = func(in[0][0], in[1][0]);				\
	out[0][1] = func(in[0][1], in[1][1]);				\
}

static smp_t mydiv(smp_t x, smp_t y)
{
	if (y == 0.0f) return 0.0f;
	return x / y;
}

static smp_t mypow(smp_t x, smp_t y)
{
	if (x < 0.0f) return pow(x, floor(y));
	return pow(x, y);
}

OPER_FUNC2(mod, fmod)
OPER_FUNC2(div, mydiv)
OPER_FUNC2(pow, mypow)

static void op_pan(const frame_t *in, frame_t *out, void *state)
{
	smp_t angle;
	(void)state;
	angle = M_PI/2 * ((in[1][0] + in[1][1]) * 0.5f - 0.5f);
	out[0][0] = in[0][0] * cos(angle) - sin(angle);
	out[0][1] = in[0][1] * cos(angle) + sin(angle);
}

opdef_t ops[] =
{
	{ 1, op_sin, NULL, NULL, "sin" },
	{ 1, op_cos, NULL, NULL, "cos" },
	{ 1, op_log, NULL, NULL, "log" },
	{ 1, op_fabs, NULL, NULL, "abs" },
	{ 2, op_pan, NULL, NULL, "pan" },
	{ 2, op_add, NULL, NULL, "+" },
	{ 2, op_sub, NULL, NULL, "-" },
	{ 2, op_mul, NULL, NULL, "*" },
	{ 2, op_mod, NULL, NULL, "%" },
	{ 2, op_div, NULL, NULL, "/" },
	{ 2, op_pow, NULL, NULL, "^" },
};
