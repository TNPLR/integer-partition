#include "rbtree.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
struct rbtree *rbtree;
long start_from = 0;
long **proc_array;
char buffer[256];
const char *magic = "\xe8\x8a\xb8\xe5\x98\x89\x00\x00";

// Check First 9 bytes as Magic Value \xe8\x8a\xb8\xe5\x98\x89\x00\x00
void write_cache(long n)
{
	FILE *cache_fptr = fopen(".cache_file", "wb");
	fwrite(magic, sizeof(char), 8, cache_fptr);
	for (long i = 0; i < n; ++i) {
		fwrite(proc_array[i], sizeof(long), (proc_array[i])[0] + 1, cache_fptr);
	}
	fclose(cache_fptr);
}

int cache_exist(long n)
{
	char magic_buffer[8];
	FILE *cache_fptr = fopen(".cache_file", "rb");
	if (cache_fptr == NULL) {
		return 0;
	}
	fread(magic_buffer, sizeof(char), 8, cache_fptr);
	if (feof(cache_fptr) || memcmp(magic_buffer, magic, 8)) {
		// Not correct
		fclose(cache_fptr);
		++start_from;
		return 0;
	} else {
		// Correct
		while (start_from < n) {
			long size;
			fread(&size, sizeof(long), 1, cache_fptr);
			if (feof(cache_fptr)) {
				break;
			}
			proc_array[start_from] = malloc(sizeof(long) * (size + 1));
			(proc_array[start_from])[0] = size;
			fread(&(proc_array[start_from])[1], sizeof(long), size, cache_fptr);
			++start_from;
		}
		fclose(cache_fptr);
		++start_from;
		return 1;
	}
}

static long range(long *proc_list)
{
        return proc_list[proc_list[0]] - 1;
}

static double average(long *proc_list)
{
        long sum = 0;
        for (long i = 1; i <= proc_list[0]; ++i) {
                sum += proc_list[i];
        }
        return (double)sum / proc_list[0];
}

static double medium(long *proc_list)
{
        if (proc_list[0] & 1) {
                return proc_list[(proc_list[0] >> 1) + 1];
        } else {
                return (double)(proc_list[(proc_list[0] >> 1)] + proc_list[(proc_list[0] >> 1) + 1]) / 2;
        }
}
char *part(long n)
{
	rbtree = malloc(sizeof(struct rbtree));
	proc_array = malloc(n * sizeof(long *));
	setup_rbtree(rbtree);

	if (!cache_exist(n)) {
		proc_array[0] = malloc(2 * sizeof(long));
		(proc_array[0])[0] = 1;
		(proc_array[0])[1] = 1;
		insert_non_exist_node(rbtree, 1);
		start_from = 2;
	} else if (start_from > n) {
		sprintf(buffer, "Result: Range: %ld Average: %.2f Median: %.2f",
				range(proc_array[n-1]), average(proc_array[n-1]), medium(proc_array[n-1]));
		free(rbtree);
		free(proc_array);
		return buffer;
	} else {
		for (long i = 1; i <= (proc_array[start_from-2])[0]; ++i) {
			insert_non_exist_node(rbtree, (proc_array[start_from-2])[i]);
		}
	}
	for (long i = start_from; i <= n; ++i) {
		insert_non_exist_node(rbtree, i);
		for (long j = i-1; j > 1; --j) {
			long break_point;
			if (i-j-j-1 >= 0) {
				break_point = (proc_array[i-j-1])[0] - (proc_array[i-j-j-1])[0];
			} else {
				break_point = (proc_array[i-j-1])[0];
			}
			long *ptr = &(proc_array[i-j-1])[(proc_array[i-j-1])[0]];
			for (long k = 0; k < break_point; ++k) {
				insert_non_exist_node(rbtree, j * *ptr--);
			}
		}
		proc_array[i-1] = malloc((rbtree->node_count + 1)* sizeof(long));
		(proc_array[i-1])[0] = rbtree->node_count;
		copy_to_array(&(proc_array[i-1])[1], rbtree);

		long range = range_rbtree(rbtree);
		double medium = medium_rbtree(rbtree);
		double average = (double)sum_rbtree(rbtree) / rbtree->node_count;
		printf("N=%ld Range: %ld Average: %.2f Median: %.2f\n", i, range, average, medium);
	}
	long range = range_rbtree(rbtree);
	double medium = medium_rbtree(rbtree);
	double average = (double)sum_delete_rbtree(rbtree) / rbtree->node_count;
	sprintf(buffer, "Result: Range: %ld Average: %.2f Median: %.2f", range, average, medium);
	write_cache(n);
	for (long i = 0; i < n; ++i) {
		free(proc_array[i]);
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
	clock_t start = clock();
	puts(part(n));
	printf("Time usage: %f\n", (double)(clock()-start) / CLOCKS_PER_SEC);
	return 0;
}
