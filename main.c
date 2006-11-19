#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* isatty */
#include "err.h"
#include "binary.h"
#include "types.h"
#include "data.h"
#include "protos.h"

#define SAMPRATE 44100
#define LEN (5*SAMPRATE)
#define MUTATIONS 3

int main(void)
{
	tree_t *tree;
	tree_t *mutant;
	int ix;

	if (isatty(STDOUT_FILENO))
		errx(1, "I won't write junk onto your terminal.");

	tree = readtree(stdin);
	printtree(tree, stderr);
	play(tree, stdout, LEN, SAMPRATE);

	for (ix = 0; ix < MUTATIONS; ix++)
	{
		mutant = copytree(tree);
		mutate(mutant);
		printtree(mutant, stderr);
		play(mutant, stdout, LEN, SAMPRATE);
		destroytree(mutant);
	}

	destroytree(tree);
	return 0;
}
