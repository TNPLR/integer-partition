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

struct node *leftmost(struct node *nd);
struct node *rightmost(struct node *nd);
struct node* inorder_successor(struct node *nd);
struct node* inorder_predecessor(struct node *nd);
void setup_rbtree(struct rbtree *rbtree);
void insert_non_exist_node(struct rbtree *rbtree, long value);
long range_rbtree(struct rbtree *rbtree);
long sum_delete_rbtree(struct rbtree* rbtree);
long sum_rbtree(struct rbtree* rbtree);
double medium_rbtree(struct rbtree *rbtree);
void delete_rbtree(struct rbtree* rbtree);
void join_tree(struct rbtree* rbtree1, const struct rbtree* rbtree2);
void copy_tree(struct rbtree *dest, const struct rbtree *src);

void copy_to_array(long *buffer, const struct rbtree* src);
#endif // RBTREE_H_
