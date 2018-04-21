/***************************************************************
The message_queue program acts as a bridge between the statistic_connector
and the monitor_connector and gives them the ability to send and receive
messages to each other.
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>

int main(void)
{
	void *context = zmq_ctx_new();
	void *backend = zmq_socket (context, ZMQ_XPUB);
	void *frontend = zmq_socket (context, ZMQ_XSUB);
	zmq_bind(backend, "tcp://*:5555");
	zmq_bind(frontend, "tcp://*:4444");
	zmq_proxy(frontend, backend, NULL);
	return 0;
}
