# Makefile for compiling a C++ program with the Lua library
CC=gcc
CFLAGS= -g
LDFLAGS=-g -l sqlite3

PROG_CLIENT=client
PROG_SERVER=virtualbank_svc
PROG_BANK1=bank1_svc
PROG_BANK2=bank2_svc

all: $(PROG_CLIENT) $(PROG_SERVER) $(PROG_BANK1) $(PROG_BANK2)

$(PROG_CLIENT): $(PROG_CLIENT).o virtualbank_clnt.o virtualbank_xdr.o

$(PROG_CLIENT).o: virtualbank.h $(PROG_CLIENT).c
virtualbank_clnt.o: virtualbank_clnt.c

$(PROG_SERVER): $(PROG_SERVER).o virtualbank_proc.o virtualbank_xdr.o bank1_xdr.o bank1_clnt.o bank2_xdr.o bank2_clnt.o Database.o 
$(PROG_SERVER).o: virtualbank.h bank1.h bank2.h Database.h $(PROG_SERVER).c
bank1_clnt.o: bank1_clnt.c
bank2_clnt.o: bank2_clnt.c

virtualbank_proc.o: virtualbank_proc.c
virtualbank_svc.o: virtualbank_svc.c
virtualbank_xdr.o: virtualbank.h virtualbank_xdr.c

virtualbank.h: virtualbank.x
	rpcgen virtualbank.x

$(PROG_BANK1):$(PROG_BANK1).o bank1_proc.o bank1_xdr.o Database.o

$(PROG_BANK2):$(PROG_BANK2).o bank2_proc.o bank2_xdr.o Database.o

bank1_proc.o: bank1_proc.c
bank1_svc.o: bank1_svc.c
bank1_xdr.o: bank1.h bank1_xdr.c

bank1.h: bank1.x
	rpcgen bank1.x

bank2_proc.o: bank2_proc.c
bank2_svc.o: bank2_svc.c
bank2_xdr.o: bank2.h bank2_xdr.c

bank2.h: bank2.x
	rpcgen bank2.x


Database.o: Database.c Database.h




.PHONY: clean depend
clean: 
	rm -f $(PROG_CLIENT) $(PROG_SERVER) *.o virtualbank_svc.c virtualbank.h virtualbank_clnt.c virtualbank_xdr.c make.depend
	rm -f $(PROG_BANK1) $(PROG_BANK2) bank1.h bank2.h bank1_clnt.c bank2_clnt.c bank1_svc.c bank2_svc.c bank1_xdr.c bank2_xdr.c

depend: virtualbank.h
	touch make.depend
	$(CXX) -M $(CFLAGS) -M $(PROG_CLIENT).c $(PROG_CLIENT).c > make.depend

-include make.depend
