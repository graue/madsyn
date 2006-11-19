#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* isatty, getopt */
#include "err.h"
#include "binary.h"
#include "types.h"
#include "data.h"
#include "protos.h"

#define SAMPRATE 44100
#define DEF_LEN 5000.0f /* default 5 sec */
#define DEF_MUTATIONS 3

#define MS_TO_SAMPLES(l) ((int)((l) * SAMPRATE / 1000.0f))

static void usage(void)
{
	fprintf(stderr, "usage: madsyn [-l len_in_ms] [-m num_mutations]\n");
	exit(EXIT_FAILURE);
}

extern int optind;
extern char *optarg;

int main(int argc, char *argv[])
{
	tree_t *tree;
	tree_t *mutant;
	int ch, ix;
	int numsamples = MS_TO_SAMPLES(DEF_LEN);
	int nummutations = DEF_MUTATIONS;

	while ((ch = getopt(argc, argv, "l:m:")) != -1)
	{
		if (ch == 'l') /* length in ms */
		{
			numsamples = MS_TO_SAMPLES(strtod(optarg, NULL));
			if (numsamples < 0) numsamples = 0;
		}
		else if (ch == 'm') /* number of mutations to do */
			nummutations = atoi(optarg);
		else usage();
	}
	if (argc - optind > 0) usage();

	if (isatty(STDOUT_FILENO))
		errx(1, "I won't write junk onto your terminal.");

	tree = readtree(stdin);
	printtree(tree, stderr);
	play(tree, stdout, numsamples, SAMPRATE);

	for (ix = 0; ix < nummutations; ix++)
	{
		mutant = copytree(tree);
		mutate(mutant);
		printtree(mutant, stderr);
		play(mutant, stdout, numsamples, SAMPRATE);
		destroytree(mutant);
	}

	destroytree(tree);
	return 0;
}
