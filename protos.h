void play(tree_t *tree, FILE *out, int numsamples, int samprate);
tree_t *readtree(FILE *fp);
void printtree(tree_t *tree, FILE *fp);
void destroy(treenode_t *node);
tree_t *copytree(const tree_t *tree);
