/* Defining operators. */

#include <stddef.h>
#include <math.h>
#include "defs.h"
#include "op.h"

static void op_sin(const frame_t *in, frame_t *out, void *state)
{
	(void)state;
	out[0][0] = sin(in[0][0]);
	out[0][1] = sin(in[0][1]);
}

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
	/* sine */
	{ 1, op_sin, NULL, "sin" }
};
