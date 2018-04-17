#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>
#include <time.h>

#define MESSAGE_SIZE 1024

int adapter_vmstat_to_csv(char [], char*)

int main()
{
	int fd[2];
	char line[MESSAGE_SIZE];
	char* converted;
	int length = 0;
	char* options[] = {"2", "-n", NULL};
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
		dup2(fd[1],1);
		close(fd[1]);
		execv("vmstat", options);
		printf("Execv failed!\n");
		exit(1);

	}

}

int adapter_vmstat_to_csv(char[] line, char* convereted){
	int length = 0;
	time_t seconds;
	char* token;
	char[30] tokArray;
	int i = 0;

	token = strtok(line, " ");
	while(token!=NULL){
		tokArray[i] = token;
		i++;
	}

	sprintf(convereted, "")

	seconds = time(NULL)/3600;


	return length;
}