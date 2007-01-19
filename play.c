#include <stdio.h>
#include <stdlib.h>
#include "math.h"
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
			/* XXX weird MinGW gcc warning on above line */
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

static int dumpfloats = 0;

static void dumpsample(FILE *out, smp_t sample)
{
	int16_t isample;
	float fsample;

	if (dumpfloats)
	{
		fsample = (float)sample;
		if (isnanf(fsample) || isinff(fsample)) fsample = 0.0f;
		fwrite(&fsample, sizeof fsample, 1, out);
	}
	else /* clip and write little-endian 16-bit PCM */
	{
		if (sample < -1.0) sample = -1.0;
		else if (sample > 1.0) sample = 1.0;
		sample *= 32767.0;
		isample = (int16_t)sample;
		isample = htole16(isample);
		fwrite(&isample, sizeof isample, 1, out);
	}
}

void play(tree_t *tree, FILE *out, int numsamples, int samprate, double amp,
	int usefloats)
{
	smp_t d_time = 1.0f / samprate;

	dumpfloats = usefloats;
	currenttime = 0.0f;
	while (numsamples-- > 0)
	{
		frame_t frame;
		run(tree->top, &frame);
		dumpsample(out, frame[0]*amp);
		dumpsample(out, frame[1]*amp);
		currenttime += d_time;
	}
}
