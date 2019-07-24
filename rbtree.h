#ifndef RBTREE_H_
#define RBTREE_H_
enum {RED, BLACK};
struct node {
	long value;
	int color;
	struct node *parent;
	struct node *lchild;
	struct node *rchild;
};

struct rbtree {
	struct node *root;
	long node_count;
};

void setup_rbtree(struct rbtree *rbtree);
void insert_non_exist_node(struct rbtree *rbtree, long value);
long range_rbtree(struct rbtree *rbtree);
long sum_delete_rbtree(struct rbtree* rbtree);
double medium_rbtree(struct rbtree *rbtree);
#endif // RBTREE_H_
