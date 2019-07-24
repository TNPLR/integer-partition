CC=gcc
ifeq (${DEBUG},1)
CFLAGS=-fprofile-arcs -ftest-coverage -lm -pg -O3
else ifeq (${DEBUG}, 2)
CFLAGS=-fprofile-arcs -ftest-coverage -lm -pg -O0 -g -ggdb
else
CFLAGS=-O3
endif
.PHONY: clear speed_test all
all:rbtree_main 
rbtree_main: rbmain.c rbtree.c
	${CC} ${CFLAGS} $^ -o $@
test:
	./rbtree_main 70
	gcov *.c
	./rbtree_main 70
	gprof -b rbtree_main gmon.out > rbreport.txt
clean:
	rm -f rbtree_main *.gcda *.gcno *.gcov *report.txt gmon.out
