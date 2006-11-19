/* Defining operators. */

#include <stddef.h>
#include <math.h>
#include "defs.h"
#include "op.h"

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

OPER_FUNC2(mod, fmod)
OPER_FUNC2(div, mydiv)

static void op_swap(const frame_t *in, frame_t *out, void *state)
{
	(void)state;
	out[0][0] = in[1][0];
	out[0][1] = in[1][1];
	out[1][0] = in[0][0];
	out[1][1] = in[0][1];
}

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
	{ 1, 1, op_sin, NULL, "sin" },
	{ 1, 1, op_cos, NULL, "cos" },
	{ 1, 1, op_log, NULL, "log" },
	{ 1, 1, op_fabs, NULL, "abs" },
	{ 2, 2, op_swap, NULL, "swap" },
	{ 2, 1, op_pan, NULL, "pan" },
	{ 2, 1, op_add, NULL, "add" },
	{ 2, 1, op_sub, NULL, "sub" },
	{ 2, 1, op_mul, NULL, "mul" },
	{ 2, 1, op_mod, NULL, "mod" },
	{ 2, 1, op_div, NULL, "div" },
};
