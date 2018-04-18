#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	char* options[] = {"2", "-n", NULL};
	void *context = zmq_ctx_new();
	void *publisher = zmq_socket (context, ZMQ_PUB);
	pid_t process;

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

	if(process > 0){
		close(fd[1]);
		read(fd[0], line, 1024);
		read(fd[0], line, 1024);
		read(fd[0], line, 1024);

		while(1){
			read(fd[0],line, 1024);
			length = adapter_vmstat_to_csv(line, convereted);
			zmq_send(publisher, &convereted, length, 0);

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

size_t adapter_vmstat_to_csv(char[] line, char* convereted){
	int length = 0;
	int free_mem = 0;
	time_t seconds;
	
	free_mem = free_memParser(line);

	sprintf(convereted, "1,%i,%i\n\0", free_mem, seconds);

	length = strlen(convereted);

	seconds = time(NULL)/3600;

	return length;
}

int free_memParser(char[] line){
	char* token;
	char[30] tokArray;
	int i = 0;

	token = strtok(line, " ");
	tokArray[i] = token;
	i++;

	while(token!=NULL){
		tokArray[i] = token;
		printf("Current token: %s at :%i\n", token, i);
		i++;
	}

	return itoa(tokArray[10]);



}