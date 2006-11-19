/* Defining operators. */

typedef struct
{
	int numinputs;
	int numoutputs;
	void (*run)(const frame_t *in, frame_t *out, void *state);
	void *(*init)(void); /* NULL if no state */
	const char *name;
} opdef_t;
