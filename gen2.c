#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MALLOC_STRUCT(x) malloc(sizeof(struct proc_struct) + (x) * sizeof(long))
#define REALLOC_STRUCT(ptr, x) realloc(ptr, sizeof(struct proc_struct) + ((x) + ptr->size) * sizeof(long))
struct proc_struct {
	long size;
	long array[];
};
struct proc_struct **array;

int compare(const void *p1, const void *p2)
{
	return *(long *)p1 < *(long *)p2 ? -1 : (*(long *)p1 > *(long *)p2);
}

static long range(long *arr, long size)
{
	return arr[size-1] - 1;
}

static double average(long *arr, long size)
{
	long sum = 0;
	for (long i = 0; i < size; ++i) {
		sum += arr[i];
	}
	return (double)sum / size;
}

static double medium(long *arr, long size)
{
	if (size & 1) {
		return arr[size >> 1];
	} else {
		return (double)(arr[(size >> 1)-1] + arr[(size >> 1)]) / 2;
	}
}

void clear_too_many(long *arr, long* size)
{
	qsort(arr, *size, sizeof(long), compare);
	for (long i = 0; i < *size - 1; ++i) {
		if (arr[i] == arr[i+1]) {
			long j = 1;
			while (arr[i+j] == arr[i+j+1] && (i+j+1) < *size - 1) {
				++j;
			}
			memcpy(&arr[i], &arr[i+j], (*size - j - i) * sizeof(long));
			*size -= j;
			arr[*size] = LONG_MAX;
		}
	}
}

void part(long n)
{
	array = malloc(n * sizeof(struct proc_struct));
	array[0] = MALLOC_STRUCT(1);
	array[0]->size = 1;
	array[0]->array[0] = 1;
	for (long i = 2; i <= n; ++i) {
		array[i-1] = MALLOC_STRUCT(1);
		array[i-1]->size = 1;
		array[i-1]->array[0] = i;
		for (long j = i-1; j > 0; --j) {
			array[i-1] = REALLOC_STRUCT(array[i-1], array[i-j-1]->size);
			for (long k = 0; k < array[i-j-1]->size; ++k) {
				array[i-1]->array[k+array[i-1]->size] = j * array[i-j-1]->array[k];
			}
			array[i-1]->size += array[i-j-1]->size;
			clear_too_many(array[i-1]->array, &array[i-1]->size);
		}
		//clear_too_many(array[i-1]->array, &array[i-1]->size);
	}
	printf("Range: %ld Average: %.2f Median: %.2f\n", range(array[n-1]->array, array[n-1]->size), average(array[n-1]->array, array[n-1]->size), medium(array[n-1]->array, array[n-1]->size));

	for (long i = 0; i < n; ++i) {
		free(array[i]);
	}
	free(array);
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
