# Makefile for compiling a C++ program with the Lua library
CC=gcc
CFLAGS= -g
LDFLAGS=-g

PROG_CLIENT=client
PROG_SERVER=sumit_svc

all: $(PROG_CLIENT) $(PROG_SERVER)

$(PROG_CLIENT): $(PROG_CLIENT).o sumit_clnt.o sumit_xdr.o

$(PROG_CLIENT).o: sumit.h $(PROG_CLIENT).c
sumit_clnt.o: sumit_clnt.c

$(PROG_SERVER): $(PROG_SERVER).o sumit_proc.o sumit_xdr.o
$(PROG_SERVER).o: sumit.h $(PROG_SERVER).c
sumit_proc.o: sumit_proc.c
summit_svc.o: summit_svc.c

sumit_xdr.o: sumit.h sumit_xdr.c

sumit.h: sumit.x
	rpcgen sumit.x


.PHONY: clean depend
clean: 
	rm -f $(PROG_CLIENT) $(PROG_SERVER) *.o sumit_svc.c sumit.h sumit_clnt.c sumit_xdr.c make.depend

depend: sumit.h
	touch make.depend
	$(CXX) -M $(CFLAGS) -M $(PROG_CLIENT).c $(PROG_CLIENT).c > make.depend

-include make.depend
