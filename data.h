typedef double smp_t;
typedef smp_t frame_t[2];

typedef void (*runptr)(const frame_t *in, frame_t *out, void *state);

#define MAXINPUTS 2

typedef struct
{
	int numinputs;
	runptr run;
	void *(*init)(void); /* NULL if no state */
	const char *name;
} opdef_t;