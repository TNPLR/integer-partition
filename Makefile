CC=gcc
ifeq (${DEBUG},1)
CFLAGS+=-fprofile-arcs -ftest-coverage -pg -O3 -fno-stack-protector
else ifeq (${DEBUG}, 2)
CFLAGS+=-fprofile-arcs -ftest-coverage -pg -O0 -g -ggdb -DVERBOSE
else
CFLAGS+=-O3
endif
.PHONY: clear speed_test all
all:rbtree_main gen2
rbtree_main: rbmain.o rbtree.o
	${CC} ${CFLAGS} $^ -o $@
gen2: gen2.o rbtree.o
	${CC} ${CFLAGS} $^ -o $@
%.o: %.c
	${CC} ${CFLAGS} $< -c -o $@
test:
	./gen2 60
	gprof -a -b gen2 gmon.out > gen2report.txt
	gcov *.c
#./rbtree_main 60
#gprof -a -b rbtree_main gmon.out > rbreport.txt
clean:
	rm -f gen2 rbtree_main *.gcda *.gcno *.gcov *report.txt gmon.out *.o
