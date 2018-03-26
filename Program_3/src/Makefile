# Makefile for compiling a C++ program with the Lua library
CC=mpicc
CFLAGS= -g
LDFLAGS=-g

PROG=barrier_mpi
HOSTFILE=hostfile.txt
EXEC_CMD=mpirun -n 4 --hostfile

all: clean $(PROG)

$(PROG): $(PROG).o

$(PROG).o: $(PROG).c

run:
	$(EXEC_CMD) $(HOSTFILE) $(PROG)

.PHONY: clean depend
clean:
	rm -f $(PROG) $(PROG).o tstfile.txt

depend:
	touch make.depend
	$(CC) -M $(CFLAGS) -M $(PROG).c > make.depend

-include make.depend
