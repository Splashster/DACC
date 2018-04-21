/***************************************************************
The statistic_connector gets and sends the total amount of
free memory and the current time to the message queue.
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <zmq.h>
#include <time.h>


#define MESSAGE_SIZE 1024

int adapter_vmstat_to_csv(char* , char*);
int free_memParser(char*);

int main()
{
	int fd[2];
	int rc;
	int length = 0;

	pid_t process;

	void *context = zmq_ctx_new();
	void *publisher = zmq_socket (context, ZMQ_PUB);
	rc = zmq_connect(publisher, "tcp://localhost:4444");

	pipe(fd);
	process = fork();


	if(process > 0){      //Parent
		close(fd[1]);
		FILE *fp = fdopen(fd[0], "r");
		char line[MESSAGE_SIZE];

		fgets(line, MESSAGE_SIZE, fp);
		fgets(line, MESSAGE_SIZE, fp);

		while(fgets(line, MESSAGE_SIZE, fp)){
			char converted[MESSAGE_SIZE];
			length = adapter_vmstat_to_csv(line, converted);
			printf(" %s", converted);
			zmq_send(publisher, converted, length+1, 0);
		}
	}else{ 								//Child
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		close(fd[1]);
		char* args[] = {(char*) "/usr/bin/vmstat", (char*) "2", (char*) "-n", NULL};
		execv(args[0], args);
		perror("Execv failed!\n");
		exit(1);
	}
}

int adapter_vmstat_to_csv(char* line, char* converted){
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
	char* tokArray[2000];
	int i = 0;
	int length = 0;

	token = strtok(line, " ");
	tokArray[i] = token;
	i++;

	while(token!=NULL){
		tokArray[i] = token;
		token = strtok(NULL, " ");
		i++;
	}

	return atoi(tokArray[4]);
}
