CFLAGS = -ggdb3 -Wall
CXXFLAGS = -ggdb3 -Wall -std=c++11
all: coredump.o coredump_test coredump_run coredump_run1 coredump_cpp_test

coredump_cpp_test: coredump_cpp_test.cc coredump.o

coredump_test: coredump_test.c coredump.o

coredump_run: coredump_run.c coredump.o

coredump_run1: coredump_run1.c coredump.o

coredump.o: coredump.c

clean: 
	rm coredump.o coredump_test coredump_run coredump_run1 coredump_cpp_test

.PHONY: clean
