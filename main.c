#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* isatty */
#include "err.h"
#include "binary.h"
#include "types.h"
#include "data.h"
#include "protos.h"

#define SAMPRATE 44100
#define LEN (20*SAMPRATE)

int main(void)
{
	tree_t *tree;

	if (isatty(STDOUT_FILENO))
		errx(1, "I won't write junk onto your terminal.");

	tree = readtree(stdin);
	printtree(tree->top, stderr);
	play(tree, stdout, LEN, SAMPRATE);
	destroy(tree->top);
	free(tree);
	return 0;
}
