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

/*typedef struct
{
	int numinputs;
	void (*run)(const frame_t *in, frame_t *out, void *state);
	void *(*init)(void); // NULL if no state
	const char *name;
} opdef_t;
*/

opdef_t ops[] =
{
	{ 1, op_sin, NULL, "sin" }
};
