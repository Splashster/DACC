#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <zmq.h>
#include <time.h>
#include <deque>
#include <sstream>

#define MESSAGE_SIZE 2048

int adapter_csv_to_plot(char*, char*);

int main()
{
	int fd[2];
	int rc;
	int i = 0;
	int length = 0;
	
	std::deque<std::string> deq;

	void *context = zmq_ctx_new();
	void *subscriber = zmq_socket (context, ZMQ_SUB);
	pid_t process;

	rc = zmq_connect(subscriber, "tcp://localhost:5555");
	if(rc == -1){
		perror("Cannot connect to queue\n");
		exit(1);
	}

	zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE, "", 0);

	pipe(fd);

	process = fork();

	//Parent
	if(process > 0){
		close(fd[0]);
		FILE *fp = fdopen(fd[1], "w");

		fprintf(fp, "%s", "set term x11\n");
		fflush(fp);

		char line[MESSAGE_SIZE];
		while(1){
			zmq_recv(subscriber, line,MESSAGE_SIZE , 0);

			char converted[MESSAGE_SIZE];
			length = adapter_csv_to_plot(line,converted);
			deq.push_back(std::string(converted));
			if(deq.size() > 20){
				deq.pop_front();
			}

			fprintf(fp, "%s", "plot '-' with linespoints\n");

			for(i = 0; i < deq.size(); i++){
				fprintf(fp, "%s", deq.at(i).c_str());
			}
			fprintf(fp,"e\n");
			fflush(fp);
		}
	}else{					//Child
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		close(fd[0]);
		char* options[] = {(char*)"/usr/bin/gnuplot", (char*) 0};
		execv(options[0], options);
		perror("Execv failed!\n");
		exit(1);

	}

}

int adapter_csv_to_plot(char* line, char* converted){
	int length = 0;
	int free_mem = 0;
	int seconds = 0;
	time_t t;
	struct tm *time_info;
	char* token;
	std::stringstream ss(line);

	ss.getline(line, MESSAGE_SIZE, ',');
	ss >> free_mem;
	ss.getline(line, MESSAGE_SIZE, ',');
	ss >> seconds;

	t = seconds;

	time_info = gmtime(&t);

	seconds = time_info->tm_hour*3600+time_info->tm_min*60+time_info->tm_sec;

	sprintf(converted, "%ld %i\n\0", seconds, free_mem);

	length = strlen(converted);

	return length;
}
