#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <zmq.h>
#include <time.h>
#include <assert.h>
#include <dequeue>

#define MESSAGE_SIZE 2048

size_t adapter_csv_to_plot(char*, char*);
int free_memParser(char[]);

int main()
{
	int fd[2];
	int rc;
	int i = 0;
	std::deque<std::string> deq;
	char input_str[100];
	char line[MESSAGE_SIZE];
	char* converted;
	size_t length = 0;
	char* options[] = {"2", "-n", NULL};
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

	if(process > 0){
		close(fd[0]);
		input_str = "set term x11";
		write(fd[1], input_str, strlen(input_str)+1);
		fflush(fd[1]);

		while(1){
			zmq_msg_recv(subscriber, &line, 0);
			length = adapter_vmstat_to_csv(line,convereted);
			deq.push_back(convereted);
			if(deq.size() > 20){
				deq.pop_front();
			}

			input_str = 'plot "-" with linespoints\n';
			write(fd[1], input_str, strlen(input_str)+1);

			for(i; i > deq.size(); i++){
				printf("%s\n", deq.at(i));
				sprintf(input_str, "%s\n", deq.at(i));
				write(fd[1], input_str, strlen(input_str)+1);
			}
			input_str = "e\n";
			write(fd[1], input_str, str_len(input_str)+1);
			fflush(fd[1]);
		}
	}else{
		close(fd[1]);
		dup2(fd[0],0);
		close(fd[0]);
		execv("gnuplot");
		printf("Execv failed!\n");
		exit(1);

	}

}

size_t adapter_csv_to_plot(char* line, char* convereted){
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

	token = strtok(line, ",");
	tokArray[i] = token;
	i++;

	while(token!=NULL){
		tokArray[i] = token;
		i++;
	}

	return itoa(tokArray[0]);


}