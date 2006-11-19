/* Defining operators. */

typedef struct
{
	int numinputs;
	frame_t (*run)(const frame_t *inputs, void *state);
	void *(*init)(void); /* NULL if no state */
	const char *name;
} opdef_t;
