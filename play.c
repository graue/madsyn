#include <stdio.h>
#include "types.h"
#include "data.h"

static smp_t currenttime;

static void run(treenode_t *node, frame_t *dst)
{
	frame_t inputs[MAXINPUTS];
	int ix;

	if (node->op == NULL)
		*dst[0] = *dst[1] = node->istime ? currenttime : node->constant;
	else
	{
		if (node->op->init != NULL && node->state == NULL)
			node->state = node->op->init();
		for (ix = 0; ix < node->op->numinputs; ix++)
			run(node->inputs[ix], &inputs[ix]);
		node->op->run((const frame_t *)&inputs[0], dst, node->state);
	}
}

static void dumpsample(FILE *out, smp_t sample)
{
	int16_t isample;
	u_int16_t uisample;

	if (sample < -32768.0f) sample = -32768.0f;
	else if (sample > 32767.0f) sample = 32767.0f;
	isample = (int16_t)sample;
	uisample = htole16(*(u_int16_t *)&sample);
	fwrite(&uisample, sizeof uisample, 1, out);
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
