void play(tree_t *tree, FILE *out, int numsamples, int samprate);
tree_t *readtree(FILE *fp);
void printtree(treenode_t *node, FILE *fp);
void destroy(treenode_t *node);
