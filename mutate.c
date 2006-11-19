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

/* Fix up the parent links under node, after a mutation fubars them. */
static void fixupparents(treenode_t *node)
{
	int ix;
	if (node->op == NULL) return;
	for (ix = 0; ix < node->op->numinputs; ix++)
	{
		fixupparents(node->inputs[ix]);
		node->inputs[ix]->parent = node;
	}
}

/* BEGIN MUTATIONS */

#define PERSISTENCE 50

void mut_swapsubtrees(treenode_t *node)
{
	treenode_t extra;
	treenode_t *one, *two;
	int tries = PERSISTENCE;

	if (node->depth <= 0) return;
	do
	{
		one = randomnode(node, rnd(node->depth));
		two = randomnode(node, rnd(node->depth));

		/*
		 * Note that we make sure the two nodes swapped
		 * are at the same depth, so one can't be a
		 * subtree of the other (that would be pretty
		 * bad):
		 */
	} while (tries-- > 0 && (one == two || one->depth != two->depth));
	if (tries == 0) return; /* didn't work out */

	memcpy(&extra, one, sizeof extra);
	memcpy(one, two, sizeof *one);
	memcpy(two, &extra, sizeof *two);

	fixupparents(node);
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
