#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <zmq.h>
#include <time.h>


#define MESSAGE_SIZE 1024

size_t adapter_vmstat_to_csv(char* , char*);
int free_memParser(char*);

int main()
{
	int fd[2];
	int rc;
	
	size_t length = 0;
	char *options[] = {"2", "-n", 0};
	pid_t process;

	void *context = zmq_ctx_new();
	void *publisher = zmq_socket (context, ZMQ_PUB);
	rc = zmq_connect(publisher, "tcp://localhost:4444");

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
		FILE *fp = fdopen(fd[0], "r");
		char line[MESSAGE_SIZE];
		//read(fd[0], line, 1024);
		//read(fd[0], line, 1024);

		fgets(line, MESSAGE_SIZE, fp);
		fgets(line, MESSAGE_SIZE, fp);

		while(1){
			fgets(line, MESSAGE_SIZE, fp);
			char converted[MESSAGE_SIZE];
			length = adapter_vmstat_to_csv(line, converted);
			printf("converted: %s\n", converted);
			zmq_send(publisher, converted, length+1, 0);
			printf("Sent\n");

		}
	}else{ 								//Child
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		close(fd[1]);
		execv("/usr/bin/vmstat", options);
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
	

	sprintf(converted, "1,%i,%ld\n\0",free_mem,seconds);

	length = strlen(converted);

	return length;
}

int free_memParser(char* line){
	char* token;
	char* tempToken;
	char* tokArray[2000];
	int i = 0;
	int length = 0;

	//printf("Current Line: %s\n", line);

	token = strtok(line, " ");
	tokArray[i] = token;
	i++;


	while(token!=NULL){
		tokArray[i] = token;
		//printf("Current token: %s at :%i\n", tokArray[i], i);
		token = strtok(NULL, " ");
		i++;
	}

	return atoi(tokArray[4]);



}