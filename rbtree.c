#include "rbtree.h"
#include <stdlib.h>
// This RB-Tree is for the Integer-Partition Usage

static struct node nil = {
	.value = -1,
	.color = BLACK,
	.lchild = &nil,
	.rchild = &nil,
	.parent = &nil
};

static void right_rotate(struct rbtree* rbtree, struct node* nd)
{
	struct node* new_root = nd->lchild;
	nd->lchild = new_root->rchild;
	if (new_root->rchild != &nil) {
		new_root->rchild->parent = nd;
	}
	new_root->parent = nd->parent;

	if (nd->parent == &nil) {
		rbtree->root = new_root;
	} else if (nd == nd->parent->rchild) {
		nd->parent->rchild = new_root;
	} else {
		nd->parent->lchild = new_root;
	}
	new_root->rchild = nd;
	nd->parent = new_root;
}

static void left_rotate(struct rbtree* rbtree, struct node* nd)
{
	struct node* new_root = nd->rchild;
	nd->rchild = new_root->lchild;
	if (new_root->lchild != &nil) {
		new_root->lchild->parent = nd;
	}
	new_root->parent = nd->parent;

	if (nd->parent == &nil) {
		rbtree->root = new_root;
	} else if (nd == nd->parent->lchild) {
		nd->parent->lchild = new_root;
	} else {
		nd->parent->rchild = new_root;
	}
	new_root->lchild = nd;
	nd->parent = new_root;
}

static void insert_fix_rbtree(struct rbtree* rbtree, struct node *fixing_node)
{
	while (fixing_node->parent->color == RED) {
		if (fixing_node->parent == fixing_node->parent->parent->lchild) {
			struct node *uncle = fixing_node->parent->parent->rchild;
			if (uncle->color == RED) {
				fixing_node->parent->color = BLACK;
				uncle->color = BLACK;
				fixing_node->parent->parent->color = RED;
				fixing_node = fixing_node->parent->parent;
			} else {
				if (fixing_node == fixing_node->parent->rchild) {
					fixing_node = fixing_node->parent;
					left_rotate(rbtree, fixing_node);
				}
				fixing_node->parent->color = BLACK;
				fixing_node->parent->parent->color = RED;
				right_rotate(rbtree, fixing_node->parent->parent);
			}
		} else {
			struct node *uncle = fixing_node->parent->parent->lchild;
			if (uncle->color == RED) {
				fixing_node->parent->color = BLACK;
				uncle->color = BLACK;
				fixing_node->parent->parent->color = RED;
				fixing_node = fixing_node->parent->parent;
			} else {
				if (fixing_node == fixing_node->parent->lchild) {
					fixing_node = fixing_node->parent;
					right_rotate(rbtree, fixing_node);
				}
				fixing_node->parent->color = BLACK;
				fixing_node->parent->parent->color = RED;
				left_rotate(rbtree, fixing_node->parent->parent);
			}
		}
	}
	rbtree->root->color = BLACK;
}

void setup_rbtree(struct rbtree *rbtree)
{
	rbtree->root = &nil;
	rbtree->node_count = 0L;
}

void insert_non_exist_node(struct rbtree *rbtree, long value)
{
	struct node *sentinel = rbtree->root;
	struct node *parent = &nil;
	while (sentinel != &nil) {
		parent = sentinel;
		if (value < sentinel->value) { // Go Left
			sentinel = sentinel->lchild;
		} else if (value > sentinel->value) { // Go Right
			sentinel = sentinel->rchild;
		} else { // Exists
			return;
		}
	}
	++rbtree->node_count;
	struct node *new_node = malloc(sizeof(struct node));
	new_node->parent = parent;
	if (parent == &nil) {
		rbtree->root = new_node; 
	} else if (value < parent->value) {
		parent->lchild = new_node;
	} else {
		parent->rchild = new_node;
	}
	new_node->lchild = new_node->rchild = &nil;
	new_node->value = value;
	new_node->color = RED;

	insert_fix_rbtree(rbtree, new_node);
}

static struct node *leftmost(struct node *nd)
{
	while (nd->lchild != &nil) {
		nd = nd->lchild;
	}
	return nd;
}

static struct node *rightmost(struct node *nd)
{
	while (nd->rchild != &nil) {
		nd = nd->rchild;
	}
	return nd;
}

long range_rbtree(struct rbtree *rbtree)
{
	return rightmost(rbtree->root)->value - 1;
}

static struct node* inorder_successor(struct rbtree *rbtree, struct node *nd)
{
	if (nd->rchild != &nil) {
		return leftmost(nd->rchild);
	}
	struct node *successor = nd->parent;
	while (successor != &nil && nd == successor->rchild) {
		nd = successor;
		successor = successor->parent;
	}
	return successor;
}

double medium_rbtree(struct rbtree *rbtree)
{
	long medium_index = rbtree->node_count >> 1;
	struct node* inorder_first = leftmost(rbtree->root);
	// Using InOrder To Travel
	for (long i = 1; i < medium_index; ++i) {
		inorder_first = inorder_successor(rbtree, inorder_first);
	}
	if (rbtree->node_count & 1) {
		return (double)(inorder_successor(rbtree, inorder_first)->value);
	} else {
		return (double)(inorder_first->value + inorder_successor(rbtree, inorder_first)->value) / 2.0;
	}
}

static long travel_sum_delete_all(struct node *nd)
{
	if (nd != &nil) {
		long sum = travel_sum_delete_all(nd->rchild);
		sum += travel_sum_delete_all(nd->lchild);
		sum += nd->value;
		free(nd);
		return sum;
	}
	return 0;
}

long sum_delete_rbtree(struct rbtree* rbtree)
{
	return travel_sum_delete_all(rbtree->root);
}