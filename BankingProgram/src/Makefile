# Makefile for compiling Banking Program
CC=gcc
CFLAGS= -g
LDFLAGS=-g -l sqlite3
SHELL=/bin/bash

PROG_CLIENT=client
PROG_SERVER=virtualbank_svc
PROG_BANK1=bank1_svc
PROG_BANK2=bank2_svc

all: $(PROG_CLIENT) $(PROG_SERVER) $(PROG_BANK1) $(PROG_BANK2)

$(PROG_CLIENT): $(PROG_CLIENT).o virtualbank_clnt.o virtualbank_xdr.o

$(PROG_CLIENT).o: virtualbank.h $(PROG_CLIENT).c
virtualbank_clnt.o: virtualbank_clnt.c

$(PROG_SERVER): $(PROG_SERVER).o virtualbank_proc.o virtualbank_xdr.o bank1_xdr.o bank1_clnt.o bank2_xdr.o bank2_clnt.o database.o
$(PROG_SERVER).o: virtualbank.h bank1.h bank2.h database.h $(PROG_SERVER).c
bank1_clnt.o: bank1_clnt.c
bank2_clnt.o: bank2_clnt.c

virtualbank_proc.o: virtualbank_proc.c
virtualbank_svc.o: virtualbank_svc.c
virtualbank_xdr.o: virtualbank.h virtualbank_xdr.c

virtualbank.h: virtualbank.x
	rpcgen virtualbank.x

$(PROG_BANK1):$(PROG_BANK1).o bank1_proc.o bank1_xdr.o database.o

$(PROG_BANK2):$(PROG_BANK2).o bank2_proc.o bank2_xdr.o database.o

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


database.o: database.c database.h


.PHONY: clean depend
clean:
	rm -f $(PROG_CLIENT) $(PROG_SERVER) *.o virtualbank_svc.c virtualbank.h virtualbank_clnt.c virtualbank_xdr.c make.depend
	rm -f $(PROG_BANK1) $(PROG_BANK2) bank1.h bank2.h bank1_clnt.c bank2_clnt.c bank1_svc.c bank2_svc.c bank1_xdr.c bank2_xdr.c bank1.db bank2.db virtualbank.db

depend: virtualbank.h
	touch make.depend
	$(CXX) -M $(CFLAGS) -M $(PROG_CLIENT).c $(PROG_CLIENT).c > make.depend

-include make.depend

.PHONY: test
test: all run_server run_bank1 run_bank2 run_client print_db1 print_db2 kill_services
run_server:
	./$(PROG_SERVER) &
run_bank1:
	./$(PROG_BANK1) > /dev/null &
run_bank2:
	./$(PROG_BANK2) > /dev/null &
run_client:
	sleep 1
	@echo -e "credit A12345 100\n\
	credit B12345 10000\n\
	transfer A12345 B12345 25\n\
	transfer B12345 A12345 10\n\
	quit" | ./$(PROG_CLIENT)
print_db1:
	echo -e "select * from transactions;" | sqlite3 bank1.db
print_db2:
	echo -e "select * from transactions;" | sqlite3 bank2.db

kill_services: kill_virt kill_bank1 kill_bank2
kill_virt:
	pkill $(PROG_SERVER)
kill_bank1:
	pkill $(PROG_BANK1)
kill_bank2:
	pkill $(PROG_BANK2)
