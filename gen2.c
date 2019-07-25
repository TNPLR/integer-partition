#include "rbtree.h"
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
struct rbtree **rbtree;
char buffer[256];

char *part(long n)
{
	rbtree = malloc(n * sizeof(struct rbtree *));
	for (long i = 0; i < n; ++i) {
		rbtree[i] = malloc(sizeof(struct rbtree));
		setup_rbtree(rbtree[i]);
	}

	insert_non_exist_node(rbtree[0], 1);
	for (long i = 2; i <= n; ++i) {
		copy_tree(rbtree[i-1], rbtree[i-2]);
		insert_non_exist_node(rbtree[i-1], i);
		for (long j = i-1; j > 1; --j) {
			struct node* nd = rightmost(rbtree[i-j-1]->root);
			long break_point;
			if (i-j-j-1 >= 0) {
				break_point = rbtree[i-j-1]->node_count - rbtree[i-j-j-1]->node_count;
			} else {
				break_point = rbtree[i-j-1]->node_count;
			}
			for (long k = 0; k < break_point; ++k) {
				insert_non_exist_node(rbtree[i-1], j * nd->value);
				nd = inorder_predecessor(nd);
			}
		}
		long range = range_rbtree(rbtree[i-1]);
		double medium = medium_rbtree(rbtree[i-1]);
		double average = (double)sum_rbtree(rbtree[i-1]) / rbtree[i-1]->node_count;
		printf("N=%ld Range: %ld Average: %.2f Median: %.2f\n", i, range, average, medium);
	}
	long range = range_rbtree(rbtree[n-1]);
	double medium = medium_rbtree(rbtree[n-1]);
	double average = (double)sum_delete_rbtree(rbtree[n-1]) / rbtree[n-1]->node_count;
	sprintf(buffer, "Range: %ld Average: %.2f Median: %.2f", range, average, medium);

	for (long i = 0; i < n-1; ++i) {
		delete_rbtree(rbtree[i]);
	}
	free(rbtree);
	return buffer;
}

int main(int argc, char *argv[])
{
	if (argc == 1) {
		return 1;
	}
	long n;
	sscanf(argv[1], "%ld", &n);
	part(n);
}
