#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

static size_t current_size = 0;
static long *proc_list = NULL;
static char buffer[64];

static int exist(int n)
{
	for (size_t i = 0; i < current_size; ++i) {
		if (proc_list[i] == n) {
			return 1;
		}
	}
	return 0;
}

static int compare(const void *p1, const void *p2)
{
	return (int)(*(long *)p1 - *(long *)p2);
}

static void append_list(long n)
{
	/*
	if (exist(n)) {
		return;
	}
	*/
	if (bsearch(&n, proc_list, current_size, sizeof(long), compare) != NULL) {
		return;
	}
	proc_list = realloc(proc_list, ++current_size * sizeof(long));
	//*(proc_list + current_size - 1) = n;
	register long i;
	for (i = 0; i < current_size - 1; ++i) {
		if (proc_list[i] > n) {
			break;
		}
	}
	for (long j = current_size - 2; j >= i; --j) {
		proc_list[j+1] = proc_list[j];
	}
	proc_list[i] = n;
}

static void rec_part(long now_proc, long last_i, long n)
{
	if (!n) {
		append_list(now_proc);
	}
	for (long i = last_i > n ? n : last_i; i > 0; --i) {
		rec_part(now_proc * i, i, n - i);
	}
}

static long range()
{
	return proc_list[current_size-1] - 1;
}

static double averange()
{
	long sum = 0;
	for (long i = 0; i < current_size; ++i) {
		sum += proc_list[i];
	}
	return (double)sum / current_size;
}

static double medium()
{
	if (current_size & 1) {
		return proc_list[(long)ceil((double)current_size / 2)-1];
	} else {
		return (double)(proc_list[(current_size >> 1)-1] + proc_list[(current_size >> 1)]) / 2;
	}
}

char *part(long n)
{
	rec_part(1, n, n);
	qsort(proc_list, current_size, sizeof(long), compare);
	sprintf(buffer, "Range: %ld Average: %.2f Median: %.2f", range(), averange(), medium());
	free(proc_list);
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
