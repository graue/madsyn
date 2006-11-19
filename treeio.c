#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "err.h"
#include "data.h"
#include "xm.h"

static char *readtoken(FILE *fp)
{
	int ch;
	char *chars = NULL;
	int nchars = 0, schars = 0;

	while ((ch = getc(fp)) != EOF && isspace(ch))
		;

	ungetc(ch, fp);

	while ((ch = getc(fp)) != EOF && !isspace(ch))
	{
		XPND(chars, nchars, schars);
		chars[nchars++] = ch;
	}

	XPND(chars, nchars, schars);
	chars[nchars++] = '\0';
	return chars;
}

tree_t *readtree(FILE *fp)
{
	tree_t *tree;
	treenode_t *top = NULL;
	treenode_t **nodes = NULL;
	int nnodes = 0, snodes = 0;
	int ix;
	char *txt;

	while (strlen(txt = readtoken(fp)) > 0)
	{
		top = xm(sizeof *top, 1);
		top->state = NULL;
		top->op = NULL;
		top->istime = 0;
		top->constant = 0.0f;

		if (isdigit(txt[0]))
		{
			top->istime = 0;
			top->constant = strtod(txt, NULL);
		}
		else if (strcmp(txt, "time") == 0)
			top->istime = 1;
		else
		{
			for (ix = 0; ops[ix].numinputs > 0; ix++)
				if (strcmp(txt, ops[ix].name) == 0)
					top->op = &ops[ix];

			if (top->op == NULL)
				errx(1, "unknown op: %s", txt);
			if (top->op->numinputs > nnodes)
			{
				errx(1, "op %s needs %d inputs, "
					"only %d on stack", txt,
					top->op->numinputs, nnodes);
			}

			for (ix = top->op->numinputs-1; ix >= 0;
				ix--, nnodes--)
			{
				top->inputs[ix] = nodes[nnodes-1];
			}
		}

		XPND(nodes, nnodes, snodes);
		nodes[nnodes++] = top;
		free(txt);
	}

	if (top == NULL)
		errx(1, "empty program");

	tree = xm(sizeof *tree, 1);
	tree->top = top;
	return tree;
}

static void printnode(treenode_t *node, FILE *fp)
{
	int ix;
	if (node->op == NULL)
	{
		if (node->istime)
			fputs("time", fp);
		else
			fprintf(fp, "%f", node->constant);
	}
	else
	{
		for (ix = 0; ix < node->op->numinputs; ix++)
		{
			printnode(node->inputs[ix], fp);
			putc(' ', fp);
		}
		fputs(node->op->name, fp);
	}
}

void printtree(tree_t *tree, FILE *fp)
{
	printnode(tree->top, fp);
	putc('\n', fp);
}
