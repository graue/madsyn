#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "data.h"
#include "xm.h"
#include "protos.h"

static treenode_t *randomnode(treenode_t *start, int depth)
{
	if (depth <= 0 || start->op == NULL)
		return start;

	return randomnode(start->inputs[rnd(start->op->numinputs)], depth-1);
}

static treenode_t *randomleaf(treenode_t *start)
{
	return randomnode(start, INT_MAX);
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
#define LOW -2.0f
#define HIGH 2.0f

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

/* Clobber a subtree, replacing it with a terminal. */
void mut_deletesubtree(treenode_t *node)
{
	treenode_t *goner;
	int ix;

	goner = randomnode(node, rnd(node->depth));
	if (goner->op != NULL)
	{
		for (ix = 0; ix < goner->op->numinputs; ix++)
			destroy(goner->inputs[ix]);
		goner->op = NULL;
	}
	goner->istime = rnd(2);
	goner->constant = LOW + frnd()*(HIGH-LOW);
}

void mut_copysubtree(treenode_t *node)
{
	treenode_t *src, *dest, *destparent;
	int destidx;

	if (node->depth <= 0) return;

	src = randomnode(node, rnd(node->depth));

	dest = randomleaf(node);
	destparent = dest->parent;
	/* Find which of its parent's inputs dest is. */
	for (destidx = 0; destidx < destparent->op->numinputs; destidx++)
		if (destparent->inputs[destidx] == dest)
			break;
	if (destidx == destparent->op->numinputs)
		return; /* shouldn't happen */

	destroy(dest); /* bye-bye, you nerdy terminal */
	destparent->inputs[destidx] = dest = copynode(src);
	dest->parent = destparent;
}

/* END MUTATIONS (although they are listed again below */

typedef void (*mutatefunc)(treenode_t *node);
mutatefunc mutations[] =
{
	mut_swapsubtrees,
	mut_deletesubtree,
	mut_copysubtree
};
#define NUMMUTATIONS (sizeof mutations / sizeof mutations[0])

void mutate(tree_t *tree)
{
	mutations[rnd(NUMMUTATIONS)](tree->top);
}
