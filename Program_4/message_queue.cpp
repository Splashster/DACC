#include <zmq.h>

int main()
{
	void *context = zmq_ctx_new();
	void *publisher = zmq_socket (context, ZMQ_XPUB);
	void *subscriber = zmq_socket (context, ZMQ_XSUB);
	zmq_bind(publisher, "tcp://localhost:4444");
	zmq_bind(subscriber, "tcp://localhost:5555");
	zmq_proxy(subscriber, publisher, NULL);	
	return 0;
}