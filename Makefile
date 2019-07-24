CC=gcc
CFLAGS=-fprofile-arcs -ftest-coverage -lm -pg
.PHONY: clear speed_test all
all: a.out
a.out: main.c
	${CC} ${CFLAGS} $^ -o $@
test:
	gcov main.c
	gprof -b a.out gmon.out > report.txt
clean:
	rm -f a.out main.gcda main.gcno main.c.gcov report.txt gmon.out
