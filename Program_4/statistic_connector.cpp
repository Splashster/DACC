#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <zmq.h>
#include <time.h>
#include <assert.h>

#define MESSAGE_SIZE 1024

size_t adapter_vmstat_to_csv(char [], char*);
int free_memParser(char[]);

int main()
{
	int fd[2];
	int rc;
	char line[MESSAGE_SIZE];
	char* converted;
	size_t length = 0;
	char *const options[] = {"2", "-n", NULL};
	pid_t process;

	void *context = zmq_ctx_new();
	void *publisher = zmq_socket (context, ZMQ_PUB);
	rc = zmq_connect(publisher, "tcp://localhost:4444");
	assert(rc == 0);

	if(pipe(fd) < 0){
		printf("Problem with pipe\n");
		exit(1);
	}

	process = fork();

	if(process < 0){
		printf("Fork Failed\n");
		exit(1);
	}

	//Parent
	if(process > 0){
		close(fd[1]);
		read(fd[0], line, 1024);
		read(fd[0], line, 1024);
		read(fd[0], line, 1024);

		while(read(fd[0],line, 1024)){
			length = adapter_vmstat_to_csv(line, converted);
			zmq_send(publisher, &converted, length, 0);

		}
	}else{ 								//Child
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		close(fd[1]);
		execv("vmstat", options);
		printf("Execv failed!\n");
		exit(1);

	}

}

size_t adapter_vmstat_to_csv(char* line, char* converted){
	int length = 0;
	int free_mem = 0;
	time_t seconds;
	
	seconds = time(NULL);

	free_mem = free_memParser(line);

	sprintf(converted, "1,%i,%d\n\0", free_mem, seconds);

	length = strlen(converted);

	return length;
}

int free_memParser(char* line){
	char* token;
	char* tokArray[30];
	int i = 0;

	token = strtok(line, " ");
	tokArray[i] = token;
	i++;

	while(token!=NULL){
		tokArray[i] = token;
		printf("Current token: %s at :%i\n", token, i);
		i++;
	}

	return atoi(tokArray[10]);



}