#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <zmq.h>
#include <time.h>

int main(void)
{
	void *context = zmq_ctx_new();
	void *backend = zmq_socket (context, ZMQ_XPUB);
	void *frontend = zmq_socket (context, ZMQ_XSUB);
	zmq_bind(backend, "tcp://*:4444");
	zmq_bind(frontend, "tcp://*:5555");
	zmq_proxy(frontend, backend, NULL);	
	return 0;
}