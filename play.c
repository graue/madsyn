#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "data.h"

static smp_t currenttime;

static void run(treenode_t *node, frame_t *dst)
{
	frame_t inputs[MAXINPUTS];
	int ix;

	if (node->op == NULL)
	{
		dst[0][0] = dst[0][1] =
			node->istime ? currenttime : node->constant;
	}
	else
	{
		if (node->op->init != NULL && node->state == NULL)
			node->state = node->op->init();
		for (ix = 0; ix < node->op->numinputs; ix++)
			run(node->inputs[ix], &inputs[ix]);
		node->op->run((const frame_t *)&inputs[0], dst, node->state);
	}
}

void destroy(treenode_t *node)
{
	int ix;
	if (node->op != NULL)
	{
		if (node->op->init != NULL && node->op->destroy != NULL)
		{
			node->op->destroy(node->state);
			node->state = NULL;
		}
#ifdef DEBUG
		fprintf(stderr, "node %p op %d (%d inputs): deleting ",
			node, (int)(node->op-ops), node->op->numinputs);
#endif
		for (ix = 0; ix < node->op->numinputs; ix++)
		{
#ifdef DEBUG
			fprintf(stderr, "%d (%p) [[", ix, node->inputs[ix]);
#endif
			destroy(node->inputs[ix]);
#ifdef DEBUG
			fprintf(stderr, "]] ");
#endif
		}
#ifdef DEBUG
		putc('\n', stderr);
#endif
	}
	free(node);
}

void destroytree(tree_t *tree)
{
	destroy(tree->top);
	free(tree);
}

#define RANGE 32767.0f

static void dumpsample(FILE *out, smp_t sample)
{
	int16_t isample;

	sample *= RANGE;
	if (sample < -RANGE) sample = -RANGE;
	else if (sample > RANGE) sample = RANGE;
	isample = (int16_t)sample;
	isample = htole16(isample);
	fwrite(&isample, sizeof isample, 1, out);
}

void play(tree_t *tree, FILE *out, int numsamples, int samprate)
{
	smp_t d_time = 1.0f / samprate;

	currenttime = 0.0f;
	while (numsamples-- > 0)
	{
		frame_t frame;
		run(tree->top, &frame);
		dumpsample(out, frame[0]);
		dumpsample(out, frame[1]);
		currenttime += d_time;
	}
}
