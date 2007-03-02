/* Defining operators. */

#include <stdlib.h>
#include "math.h"
#include "data.h"
#include "xm.h"

#define SIMPLE_FUNC(name)						\
static void op_ ## name (const frame_t *in, frame_t *out, void *state)	\
{									\
	(void)state;							\
	out[0][0] = name(in[0][0]);					\
	out[0][1] = name(in[0][1]);					\
}

static double mylog(double x)
{
	if (x < 0.0) return -log(-x);
	return log(x);
}

SIMPLE_FUNC(sin)
SIMPLE_FUNC(cos)
SIMPLE_FUNC(mylog)
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
	if (y == 0.0) return 0.0;
	return x / y;
}

static smp_t mypow(smp_t x, smp_t y)
{
	if (x < 0.0) return -pow(-x, y);
	return pow(x, y);
}

static smp_t min(smp_t x, smp_t y)
{
	return x < y ? x : y;
}

OPER_FUNC2(mod, fmod)
OPER_FUNC2(div, mydiv)
OPER_FUNC2(pow, mypow)
OPER_FUNC2(min, min)

static void op_pan(const frame_t *in, frame_t *out, void *state)
{
	smp_t angle;
	(void)state;
	angle = M_PI/2 * ((in[1][0] + in[1][1]) * 0.5f - 0.5f);
	out[0][0] = in[0][0] * cos(angle) - sin(angle);
	out[0][1] = in[0][1] * cos(angle) + sin(angle);
}

/* An init function for anything needing two samples of state. */
static void *init2smps(void)
{
	smp_t *smps = xm(sizeof *smps, 2);
	smps[0] = smps[1] = 0.0;
	return smps;
}

#ifdef BAD_OPS
/*
 * statesin keeps its phases as a state and takes new frequencies
 * (Hz) as input. (No inertia for the frequency.)
 */
static void op_statesin(const frame_t *in, frame_t *out, void *state)
{
	smp_t *phases = state;

	out[0][0] = sin(phases[0]);
	out[0][1] = sin(phases[1]);
	phases[0] += (in[0][0] * 2*M_PI / SAMPRATE);
	phases[1] += (in[0][1] * 2*M_PI / SAMPRATE);
}
#endif

static void op_deriv(const frame_t *in, frame_t *out, void *state)
{
	smp_t *last = state;
	out[0][0] = in[0][0] - 0.5*last[0];
	out[0][1] = in[0][1] - 0.5*last[1];
	last[0] = in[0][0];
	last[1] = in[0][1];
}

static void op_int(const frame_t *in, frame_t *out, void *state)
{
	smp_t *last = state;
	out[0][0] = in[0][0] + 0.5*last[0];
	out[0][1] = in[0][1] + 0.5*last[1];
	last[0] = in[0][0];
	last[1] = in[0][1];
}

static const opdef_t ops_array[] =
{
	{ 1, op_sin, NULL, NULL, "sin" },
	{ 1, op_cos, NULL, NULL, "cos" },
	{ 1, op_mylog, NULL, NULL, "log" },
	{ 1, op_fabs, NULL, NULL, "abs" },
	{ 2, op_pan, NULL, NULL, "pan" },
	{ 2, op_add, NULL, NULL, "+" },
	{ 2, op_sub, NULL, NULL, "-" },
	{ 2, op_mul, NULL, NULL, "*" },
	{ 2, op_mod, NULL, NULL, "%" },
	{ 2, op_div, NULL, NULL, "/" },
	{ 2, op_pow, NULL, NULL, "^" },
	{ 2, op_min, NULL, NULL, "min" },
#ifdef BAD_OPS
	{ 1, op_statesin, init2smps, free, "sin_" },
#endif
	{ 1, op_deriv, init2smps, free, "deriv" },
	{ 1, op_int, init2smps, free, "int" },
	{ 0, NULL, NULL, NULL, NULL }
};

const opdef_t *ops = ops_array;
