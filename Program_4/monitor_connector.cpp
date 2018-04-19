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
	
	
	size_t length = 0;
	char *const options[] = {0};

	std::deque<std::string> deq;

	void *context = zmq_ctx_new();
	void *subscriber = zmq_socket (context, ZMQ_SUB);
	pid_t process;

	rc = zmq_connect(subscriber, "tcp://localhost:5555");
	if(rc == -1){
		printf("Cannot connect to queue\n");
		exit(1);
	}

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

		//strcpy(input_str, "set term x11");
		//printf("input_str: %s\n",input_str);
		//write(fd[1], input_str, strlen(input_str));
		fprintf(fp, "%s", "set term x11\n");
		fflush(fp);

		char line[MESSAGE_SIZE];
		while(1){
			printf("Waiting....\n");
			int count = zmq_recv(subscriber, line,MESSAGE_SIZE , 0);
			if (count <= 0) {continue;}
			char converted[MESSAGE_SIZE];
			printf("Recevied: %s\n", line);
			length = adapter_csv_to_plot(line,converted);
			deq.push_back(std::string(converted));
			if(deq.size() > 20){
				deq.pop_front();
			}

			strcpy(input_str, "plot '-' with linespoints\n");
			//write(fd[1], input_str, strlen(input_str));
			fprintf(fp, "%s", input_str);

			for(i = 0; i < deq.size(); i++){
				printf("%s\n", deq.at(i).c_str());
				sprintf(input_str, "%s\n", deq.at(i).c_str());
				//write(fd[1], input_str, strlen(input_str));
				fprintf(fp, "%s", input_str);
			}
			strcpy(input_str, "e\n");
			fprintf(fp, "%s", input_str);
			//write(fd[1], input_str, strlen(input_str));
			fflush(fp);
		}
	}else{					//Child
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		close(fd[0]);
		execv("/usr/bin/gnuplot", options);
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