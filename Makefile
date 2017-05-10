CFLAGS = -ggdb3 -Wall
all: coredump.o coredump_test

coredump_test: coredump_test.c coredump.o

coredump.o: coredump.c
