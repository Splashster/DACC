#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <zmq.h>
#include <time.h>
#include <assert.h>
#include <deque>

#define MESSAGE_SIZE 2048

size_t adapter_csv_to_plot(char*, char*);

int main()
{
	int fd[2];
	int rc;
	int i = 0;
	char* input_str;
	char line[MESSAGE_SIZE];
	char* converted;
	size_t length = 0;
	char *const options[] = {NULL};

	std::deque<std::string> deq;

	void *context = zmq_ctx_new();
	void *subscriber = zmq_socket (context, ZMQ_SUB);
	pid_t process;

	rc = zmq_connect(subscriber, "tcp://localhost:5555");
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
		close(fd[0]);
		FILE *fp = fdopen(fd[1], "w");

		strcpy(input_str, "set term x11");
		write(fd[1], input_str, strlen(input_str));
		fflush(fp);

		while(1){
			zmq_recv(subscriber, &line,MESSAGE_SIZE , 0);
			length = adapter_csv_to_plot(line,converted);
			deq.push_back(converted);
			if(deq.size() > 20){
				deq.pop_front();
			}

			strcpy(input_str, "plot '-' with linespoints\n");
			write(fd[1], input_str, strlen(input_str));

			for(i = 0; i < deq.size(); i++){
				printf("%s\n", deq.at(i).c_str());
				sprintf(input_str, "%s\n", deq.at(i).c_str());
				write(fd[1], input_str, strlen(input_str));
			}
			strcpy(input_str, "e\n");
			write(fd[1], input_str, strlen(input_str));
			fflush(fp);
		}
	}else{					//Child
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		close(fd[0]);
		execv("gnuplot", options);
		printf("Execv failed!\n");
		exit(1);

	}

}

size_t adapter_csv_to_plot(char* line, char* converted){
	int length = 0;
	int free_mem = 0;
	int seconds = 0;
	time_t t;
	struct tm *time_info;
	char* token;

	token = strtok(line, ",");
	token = strtok(line, NULL);
	t = atoi(strtok(line, NULL));
	
	time_info = gmtime(&t);

	seconds = time_info->tm_hour*3600+time_info->tm_min*60+time_info->tm_sec;

	sprintf(converted, "%s %d\n\0", free_mem, seconds);

	length = strlen(converted);

	seconds = time(NULL)/3600;

	return length;
}