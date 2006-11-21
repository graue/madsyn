void play(tree_t *tree, FILE *out, int numsamples, int samprate, double amp,
	int usefloats);
tree_t *readtree(FILE *fp);
void printtree(tree_t *tree, FILE *fp);
void destroy(treenode_t *node);
void destroytree(tree_t *tree);
tree_t *copytree(const tree_t *tree);
treenode_t *copynode(const treenode_t *node);
void mutate(tree_t *tree);

/* random numbers */
int rnd(int n);
double frnd(void); /* from 0.0f to 1.0f */
