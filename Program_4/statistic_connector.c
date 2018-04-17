#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>

#define MESSAGE_SIZE 1024

int adapter_vmstat_to_csv(char [], char*)

int main()
{
	int fd[2];
	char line[MESSAGE_SIZE];
	char* converted;
	int length = 0;
	void *context = zmq_ctx_new();
	void *publisher = zmq_socket (context, ZMQ_PUB);
	zmq_bind(publisher, "tcp://localhost:4444");
	pid_t process;

	if(pipe(fd) < 0){
		printf("Problem with pipe\n");
		exit(1);
	}

	process = fork();

	if(process < 0){
		printf("Fork Failed\n");
		exit(1);
	}

	if(process > 0){
		close(fd[1]);
		read(fd[0], line, 1024);
		read(fd[0], line, 1024);
		read(fd[0], line, 1024);

		while(1){
			read(fd[0],line, 1024);
			length = adapter_vmstat_to_csv(line, convereted);
			zmq_send(publisher, &convereted, 0);

		}
	}else{
		close(fd[0]);
		
	}

}

int adapter_vmstat_to_csv(char[] line, char* convereted){

}