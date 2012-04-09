CC = g++
LDFLAGS = -Wall -lboost_program_options -pg

all: main

scattersearch.o: scattersearch.cpp scattersearch.hpp common.hpp
main.o: main.cpp
main: main.o scattersearch.o

clean:
	@rm -vf *.o main

distclean: clean
	@rm -vf gmon.out

.PHONY: all clean
