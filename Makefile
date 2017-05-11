CFLAGS = -ggdb3 -Wall
all: coredump.o coredump_test coredump_run coredump_run1

coredump_test: coredump_test.c coredump.o

coredump_run: coredump_run.c coredump.o

coredump_run1: coredump_run1.c coredump.o

coredump.o: coredump.c
