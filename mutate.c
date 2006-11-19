#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "data.h"
#include "xm.h"
#include "protos.h"

#define PERSISTENCE 50
#define LOW -1000.0f
#define HIGH 1000.0f

static smp_t randomconstant(void)
{
	return LOW + frnd()*(HIGH-LOW);
}

/* Return a random op number. XXX a little bit of a hack. */
static int randop(void)
{
	static int count = -1;

	if (count == -1)
		for (count = 0; ops[count].numinputs > 0; count++)
			;

	return rnd(count);
}

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

static treenode_t *newrandomizedleaf(void)
{
	treenode_t *node;
	node = xm(sizeof *node, 1);
	node->op = NULL;
	node->istime = rnd(3) ? 0 : 1;
	node->constant = randomconstant();
	node->state = NULL;
	node->parent = NULL;
	return node;
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

/* Propagate correct depth information upwards from a leaf. */
static void fixupdepths(treenode_t *node)
{
	int ix;

	node->depth = 0;
	if (node->op != NULL)
		for (ix = 0; ix < node->op->numinputs; ix++)
			if (node->inputs[ix]->depth >= node->depth)
				node->depth = node->inputs[ix]->depth+1;

	if (node->parent != NULL)
		fixupdepths(node->parent);
}

/* Tells if low has ancestor of high. */
static int hasancestor(const treenode_t *low, const treenode_t *high)
{
	if (low->parent == NULL)
		return 0;
	if (low->parent == high)
		return 1;
	return hasancestor(low->parent, high);
}

/* BEGIN MUTATIONS */

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
		 * Note that we make sure neither of the nodes
		 * swapped is the ancestor of the other, as
		 * then we'd end up creating a loop.
		 */
	} while (tries-- > 0 && (one == two || hasancestor(one, two)
		|| hasancestor(two, one)));
	if (tries == 0) return; /* didn't work out */

	memcpy(&extra, one, sizeof extra);
	memcpy(one, two, sizeof *one);
	memcpy(two, &extra, sizeof *two);

	fixupparents(node);

	/* Note: Depths remain correct after this operation. I think. */
}

/* Clobber a subtree, replacing it with a terminal. */
void mut_deletesubtree(treenode_t *node)
{
	treenode_t *goner;
	int ix;

	if (node->depth > 0)
		goner = randomnode(node, rnd(node->depth));
	else goner = node;
	if (goner->op != NULL)
	{
		for (ix = 0; ix < goner->op->numinputs; ix++)
			destroy(goner->inputs[ix]);
		goner->op = NULL;
	}
	goner->istime = rnd(2);
	goner->constant = randomconstant();
	fixupdepths(goner);
}

void mut_copysubtree(treenode_t *node)
{
	treenode_t *src, *dest, *destparent;
	int destidx;
	int tries = PERSISTENCE;

	if (node->depth <= 0) return;

	do
	{
		src = randomnode(node, rnd(node->depth));
		dest = randomleaf(node); /* can't be top, due to depth check */
	} while (tries-- > 0 && hasancestor(dest, src));
	if (tries == 0) return;
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
	fixupdepths(dest);
}

/* Slip a new op in the tree (not necessarily at a leaf). */
void mut_addop(treenode_t *node)
{
	treenode_t *addspot;
	treenode_t *newnode;
	int ix;
	int inputidx;

	addspot = node->depth > 0 ? randomnode(node, rnd(node->depth)) : node;
	newnode = xm(sizeof *newnode, 1);
	memcpy(newnode, addspot, sizeof *newnode);
	newnode->parent = addspot;

	addspot->op = &ops[randop()];
	addspot->state = NULL;
	inputidx = rnd(addspot->op->numinputs);
	for (ix = 0; ix < addspot->op->numinputs; ix++)
	{
		if (ix == inputidx)
			addspot->inputs[ix] = newnode;
		else
		{
			addspot->inputs[ix] = newrandomizedleaf();
			addspot->inputs[ix]->parent = addspot;
		}
	}
	addspot->depth++;
}

/* END MUTATIONS (although they are listed again below */

typedef void (*mutatefunc)(treenode_t *node);
mutatefunc mutations[] =
{
	mut_swapsubtrees,
	mut_deletesubtree,
	mut_copysubtree,
	mut_addop
};
#define NUMMUTATIONS (sizeof mutations / sizeof mutations[0])

void mutate(tree_t *tree)
{
	int method = rnd(NUMMUTATIONS);
#ifdef DEBUG
	fprintf(stderr, "using method %d\n", method);
#endif
	mutations[method](tree->top);
}
