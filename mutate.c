#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "xm.h"
#include "protos.h"

static treenode_t *randomnode(treenode_t *start, int depth)
{
	if (depth <= 0 || start->op == NULL)
		return start;

	return randomnode(start->inputs[rnd(start->op->numinputs)], depth-1);
}

/* BEGIN MUTATIONS */

void mut_swapsubtrees(treenode_t *node)
{
	treenode_t extra;
	treenode_t *one, *two;

	if (node->depth <= 0) return;
	one = randomnode(node, rnd(node->depth));
	two = randomnode(node, rnd(node->depth));
	if (one == two) return;

	memcpy(&extra, one, sizeof extra);
	memcpy(one, two, sizeof *one);
	memcpy(two, &extra, sizeof *two);
}

/* END MUTATIONS (although they are listed again below */

typedef void (*mutatefunc)(treenode_t *node);
mutatefunc mutations[] =
{
	mut_swapsubtrees
};
#define NUMMUTATIONS (sizeof mutations / sizeof mutations[0])

void mutate(tree_t *tree)
{
	mutations[rnd(NUMMUTATIONS)](tree->top);
}
