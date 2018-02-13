ARG1 = 10003

all: p1 run 

p1 : server.cpp
	g++ -g -I/usr/include/lua5.2 -g -L/usr/lib/x86_64-linux-gnu -llua5.2 -std=c++11 server.cpp httpserver.cpp -o server -lpthread

.PHONY run : server
	./server $(ARG1)

.PHONY:	all
