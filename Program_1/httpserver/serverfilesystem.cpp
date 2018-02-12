#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h> 

#include <iostream>

#include <thread> 

#include <fstream>

#include <string>

#include <cstring>

#include "fileserver.h"



#define PROTOCOL_MESSAGE_SIZE 100

#define MAX_BUFFER_SIZE 1024

#define NUM_SOCKETS_ALLOWED 10







using namespace std;



//Deallocates allocated memory

void dealocate(char* mem){

	delete[] mem;

}



int writeSock(char *buff , FILE *write_fd){

	

	fprintf(write_fd, "%s", buff);

	fflush(write_fd);

}



bool isGetRequest(char *buff){

	bool found = false;

	if(strstr(buff,"GET")){

		found = true;

		cerr << "get request incoming" << endl;

	}

	return found;

}

bool shouldKeepAlive(char *buff){

	bool keep = false;

	if(strcasestr(buff, "HTTP/1.1") || strcasestr(buff,"Keep-Alive")){

		keep = true;

		cerr << "keeping alive" << endl;

	}

	return keep;

}


//Gets size of a specified file.
int getFileSize(string fileName){
	int size = 0;
	ifstream theFile (fileName,ios::binary | ios::ate);
	
	theFile.seekg(0, ios::end);
	size = theFile.tellg();
	theFile.close();
	return size;
}

//Reads and returns contents of a file
string readFile(string fileName){
	int fileSize = getFileSize(fileName);
	ifstream getContent(fileName, ios::in);
	string theContents = "";
	string temp = "";
	cerr << "File size: " << fileSize << endl;
	while(getline(getContent,temp)){
		if(!getContent.eof()){
			theContents += temp + '\n';
			cerr << "temp " << temp << endl;
		}
		else{
			theContents += temp;
		}
	}
	getContent.close();
	return theContents;
}


void serviceRequest(char *buff, FILE *write_fd){
	
	string getRequest(buff);
	string version = "";
	string content_type = "Content-Type: text/html\r\n";
	string content_length = "";
	string date = "";
	string rep = "";
	time_t now = time(0);
	char* dt = ctime(&now);
	int first_pos = getRequest.find("GET");
	int last_pos;
	int fileSize = 0;
	
	if(getRequest.find("HTTP/1.1") == -1){
		last_pos = getRequest.find(" HTTP/1.0");
		version = "HTTP/1.0";
	}else{
		last_pos = getRequest.find(" HTTP/1.1");
		version = "HTTP/1.1";
	}

	cerr << "Current buff: " << buff << "First: " << first_pos << "Last: " << last_pos << endl;

	string contentRequest = getRequest.substr(first_pos + 5,last_pos-first_pos - 5);
	fileSize = getFileSize(contentRequest);

	if(!ifstream(contentRequest)){
		cerr << contentRequest << "Throw dat 404" << endl;
	}else{
		char response[MAX_BUFFER_SIZE];
		version += "200 OK\r\n";
		date = "Date: " + dt + "\r\n";
		content_length = "Content-Length: itoa(fileSize)\r\n\r\n"; 
		rep = version + date + content_type + content_length + readFile(contentRequest);
		
		strcpy(contents, rep.c_str());
		
		cerr << "Contents: " << contents << endl;
		writeSock(contents, write_fd);
	}
}



int readSock(int clisock){

	char buff[MAX_BUFFER_SIZE];

	int read_sock = clisock;

	FILE *read_fd = fdopen(read_sock, "r");

	FILE *write_fd = fdopen(read_sock, "w");



	int done = 0;

	int result = 1;

	bool getRequest = false;

	bool keepAlive = false;



	//cerr << "Going in while" << endl;

	while(!done) {

		if(fgets(buff, MAX_BUFFER_SIZE, read_fd) == NULL){

			done = 1;
			result = 0;
			break;

		}

		cerr << "Reading " << buff << endl;

		

		if(getRequest == false){
			getRequest = isGetRequest(buff);
			if(getRequest == true){
				serviceRequest(buff, write_fd);
			}
		}

		if(keepAlive == false){keepAlive = shouldKeepAlive(buff);}

		if(strcmp(buff, "quit\n") == 0 ){

			done = 1;

		}else if (getRequest == false) {

		 	done = 1;

			result = 0;

			cerr << "It me" << endl;

		}	

	}

	
	



	fclose(read_fd);

	fclose(write_fd);



	return result;

	

}


void* connectHandler(void* args){

	int clisock = (intptr_t) args;

	while(readSock(clisock));

	close(clisock);

	pthread_exit(NULL);

}

	

//Handles connection between client and server. Also performs all the work of receiving (STOR) and transferring (RTRV) files from the client.

void handleConnection(int clisock){

	while(1){

		pthread_attr_t attribs;

		pthread_t thread;

		pthread_attr_init(&attribs);

		pthread_attr_setdetachstate(&attribs, PTHREAD_CREATE_DETACHED);

		pthread_create(&thread, &attribs, connectHandler, (void*)(intptr_t)clisock);

	}	

}


//Setups and starts the server. Returns 0 if sucessful and 1 if not.

int run_server(int port){

	struct sockaddr_in server_addr, cli_addr;

	unsigned int clilen;

	int result = 0;

	int client_num = 0;

	int newsock_con,serv_socket_num;

	

	if((serv_socket_num = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

		result = 1;

		return result;

	}

	

	memset((void *) &server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;

	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	server_addr.sin_port = htons(port);

	

	if(bind(serv_socket_num, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {

		cerr << "Bind error.";

		result = 1;

		return result;

	}

	

	listen(serv_socket_num, NUM_SOCKETS_ALLOWED);

	while(1){

		clilen = sizeof(cli_addr);

		newsock_con = accept(serv_socket_num, (struct sockaddr *) &cli_addr, &clilen);

		if(newsock_con < 0) {

			cerr << "Accept error." << endl;

			result = 1;

			return result;

		}

		cerr << "Listening to client " << client_num << " on port number " << to_string(port) << endl;

      	client_num++;

		handleConnection(newsock_con);

	}

	return result;

}
