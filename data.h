typedef double smp_t;
typedef smp_t frame_t[2];

typedef void (*runptr)(const frame_t *in, frame_t *out, void *state);

#define MAXINPUTS 2

typedef struct
{
	int numinputs;
	runptr run;
	void *(*init)(void); /* NULL if no state */
	void (*destroy)(void *state); /* NULL if no state */
	const char *name;
} opdef_t;

typedef struct treenode
{
	const opdef_t *op; /* if NULL then a terminal */
	int istime; /* if op == NULL, use time value here */
	smp_t constant; /* if op == NULL && !istime, constant value */
	struct treenode *inputs[MAXINPUTS]; /* if op != NULL */
	void *state; /* if op != NULL && op->init != NULL */
} treenode_t;

typedef struct
{
	treenode_t *top;
} tree_t;

const opdef_t *ops; /* op.c */
