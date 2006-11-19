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

/*typedef struct
{
	int numinputs;
	int numoutputs;
	void (*run)(const frame_t *in, frame_t *out, void *state);
	void *(*init)(void); // NULL if no state
	const char *name;
} opdef_t;
*/

opdef_t ops[] =
{
	{ 1, 1, op_sin, NULL, "sin" },
	{ 1, 1, op_cos, NULL, "cos" },
	{ 1, 1, op_log, NULL, "log" },
	{ 1, 1, op_fabs, NULL, "abs" },
	{ 2, 2, op_swap, NULL, "swap" },
	{ 1, 2, op_pan, NULL, "pan" },
};
