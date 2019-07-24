CC?=gcc
ifeq (${DEBUG},1)
CFLAGS=-fprofile-arcs -ftest-coverage -pg -O3
else ifeq (${DEBUG}, 2)
CFLAGS=-fprofile-arcs -ftest-coverage -pg -O0 -g -ggdb
else
CFLAGS=-O3
endif
.PHONY: clear speed_test all
all:rbtree_main gen2
rbtree_main: rbmain.c rbtree.c
	${CC} ${CFLAGS} $^ -o $@
gen2: gen2.c rbgen2.c
	${CC} ${CFLAGS} $^ -o $@
test:
	./rbtree_main 40
	gprof -b rbtree_main gmon.out > rbreport.txt
	./gen2 40
	gprof -b gen2 gmon.out > rbreport.txt
	gcov *.c
clean:
	rm -f gen2 rbtree_main *.gcda *.gcno *.gcov *report.txt gmon.out
