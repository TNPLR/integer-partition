#include "rbtree.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

char buffer[256];
struct rbtree *rbtree;
static void rec_part(long now_proc, long last_i, long n)
{
	if (!n || last_i == 1) {
		insert_non_exist_node(rbtree, now_proc);
		return;
	}
	for (long i = last_i > n ? n : last_i; i > 0; --i) {
		rec_part(now_proc * i, i, n - i);
	}
}

char *part(long n)
{
	rbtree = malloc(sizeof(struct rbtree));
	setup_rbtree(rbtree);

	rec_part(1, n, n);
	long range = range_rbtree(rbtree);
	double medium = medium_rbtree(rbtree);
	double averange = (double)sum_delete_rbtree(rbtree) / rbtree->node_count;
	sprintf(buffer, "Range: %ld Average: %.2f Median: %.2f", range, averange, medium);
	free(rbtree);
	return buffer;
}

int main(int argc, char *argv[])
{
	long i;
	if (argc == 1) {
		return 1;
	}
	sscanf(argv[1], "%ld", &i);
	puts(part(i));
}
