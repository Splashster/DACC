# Makefile for compiling ini ESB program
CC=g++
CFLAGS= -g
LDFLAGS=-g -lzmq

STATS=statistic_conn
MESSAGE_QUEUE=message_queue
MONITOR=monitor_conn

all: $(STATS) $(MESSAGE_QUEUE) $(MONITOR)

$(STATS): $(STATS).o
$(STATS).o: $(STATS).cpp

$(MESSAGE_QUEUE): $(MESSAGE_QUEUE).o
$(MESSAGE_QUEUE).o: $(MESSAGE_QUEUE).cpp

$(MONITOR): $(MONITOR).o
$(MONITOR).o: $(MONITOR).cpp

run: run_queue run_monitor run_stats
run_queue:
	./$(MESSAGE_QUEUE) &
run_monitor:
	./$(MONITOR) &
run_stats:
	./$(STATS)

killall: kill_stat kill_queue kill_monitor
kill_stat:
	-pkill $(STATS)
kill_queue:
	-pkill $(MESSAGE_QUEUE)
kill_monitor:
	-pkill $(MONITOR)


.PHONY: clean depend
clean:
	rm -f $(STATS) $(STATS).o $(MESSAGE_QUEUE) $(MESSAGE_QUEUE).o $(MONITOR) $(MONITOR).o

depend:
	touch make.depend
	$(CC) -M $(CFLAGS) -M $(PROG).c > make.depend

-include make.depend
