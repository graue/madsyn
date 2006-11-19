/* Defining operators. */

typedef struct
{
	int numinputs;
	smp_t (*run)(const smp_t *inputs, void *state);
	void *(*init)(void); /* NULL if no state */
	const char *name;
} opdef_t;
