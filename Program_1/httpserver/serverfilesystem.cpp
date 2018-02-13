extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

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

#include <map>

#include "fileserver.h"



#define PROTOCOL_MESSAGE_SIZE 100

#define MAX_BUFFER_SIZE 1024

#define NUM_SOCKETS_ALLOWED 10

static int html_out(lua_State *L);

const char *Header = "Message";

char *luaMessage;




using namespace std;



int writeSock(char *buff , FILE *write_fd){

	cerr << "Writing to socket: " << buff << endl;

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
	bool keepAlive = false;	

	if(strcasestr(buff, "HTTP/1.1") || strcasestr(buff,"Keep-Alive")){

		keepAlive = true;

		cerr << "keeping alive" << endl;

	}
	return keepAlive;
}

string getVersion(string request){
	string version = "";

	if(request.find("HTTP/1.1") != -1){
		version = "HTTP/1.1 ";
	}else{
		version = "HTTP/1.0 ";
	}
	return version;
}

int getFirstPosition(string request, string item){
	int first_pos = 0;
	first_pos = request.find(item);
	return first_pos;
}

int getLastPosition(string request, string item){
	int last_pos = 0;
	last_pos = request.find(item);
	return last_pos;
}

map<string,string> parseForm(string request){
	map<string,string> parsedItems;
	int first_pos, last_pos;

	first_pos = getFirstPosition(request, "firstname=");
	last_pos = getLastPosition(request, "&lastname");
	parsedItems["firstname"] = request.substr(first_pos+10,last_pos-first_pos-10);
	
	first_pos = getFirstPosition(request, "lastname=");
	last_pos = getLastPosition(request, " HTTP");
	parsedItems["lastname"] = request.substr(first_pos+9,last_pos-first_pos-9);

	return parsedItems;
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
	//cerr << "File size: " << fileSize << endl;
	while(getline(getContent,temp)){
		if(!getContent.eof()){
			theContents += temp + '\n';
			//cerr << "temp " << temp << endl;
		}
		else{
			theContents += temp;
		}
	}
	getContent.close();
	return theContents;
}

void runLua(map<string,string> items){
	int ret, result;
	string firstname = items["firstname"], lastname = items["lastname"];

	lua_State *L = luaL_newstate();

    	luaL_openlibs(L);

	ret = luaL_loadfile(L, "signup.lua");
	
	if (ret) {
		cerr << "Couldn't load file: " << lua_tostring(L, -1) << endl;
		exit(1);
   	}
	
	lua_newtable(L);

	lua_pushstring(L, "firstname");
    	lua_pushstring(L, firstname.c_str());

	lua_rawset(L, -3);

	lua_pushstring(L, "lastname");
    	lua_pushstring(L, lastname.c_str());

	lua_rawset(L, -3);
	lua_setglobal(L, "Info");

	int *ud = (int *) lua_newuserdata(L, sizeof(int));
    	*ud = 1;

	lua_pushcclosure(L, html_out, 1);
	
	lua_setglobal(L, "html_out");
	result = lua_pcall(L, 0, 1, 0);
	if (result) {
	    cerr << "Failed to run script: " << lua_tostring(L, -1) << endl;
	    exit(1);
	}
	lua_pop(L, 1);
    	lua_close(L);
}

static int html_out(lua_State *L) {
	  /* just to show that I can get the upvalue.  This is what makes this function
	     a closure. */
	  int *fd = (int *) lua_touserdata(L, lua_upvalueindex(1));
	  // Now get the single argument passed by the call in the Lua script.
	  char *msg = (char *) lua_tostring(L, -1);
	  // Now, write the argument using the file descriptor upvalue.
	  write(*fd, "C++ : Writing passed argument: ", 31);
	  write(*fd, msg, strlen(msg));
	  write(*fd, "\n", 1);
	  luaMessage = msg;


	  return 0;	
}

string generateHTML(char* message){
	string htmlMessage = "";
	string mess(message);
	
	htmlMessage = "<!DOCTYPE html>\r\n<html>\r\n<body>\r\n<h1 style='background-color:green;'>"+mess+"</h1>\r\n</body>\r\n</html>";

	return htmlMessage;
}

void goodRequest(string version, string dt, string fileName, bool generatehtml, FILE *write_fd){
	char response[MAX_BUFFER_SIZE];
	string content_type = "Content-Type: text/html\r\n";
	string content_length = "";
	string date = "";
	string html = generateHTML(luaMessage);
	string rep = "";

	
	if(generatehtml){
		cerr << "HERE 1" << luaMessage << endl;
		version += "200 OK\r\n";
		date = "Date: " + dt;
		content_length = "Content-Length: " + to_string(html.length()) + "\r\n\r\n"; 
		rep = version + date + content_type + content_length + html;
		strcpy(response, rep.c_str());	
	}else{
		cerr << "HERE 2 " << endl;
		version += "200 OK\r\n";
		date = "Date: " + dt;
		content_length = "Content-Length: " + to_string(getFileSize(fileName)) + "\r\n\r\n"; 
		rep = version + date + content_type + content_length + readFile(fileName);
		strcpy(response, rep.c_str());	
		cerr << "ReSPONSE: " << response << endl;
	}

	writeSock(response, write_fd);
}

void badRequest(string version, string dt, FILE* write_fd){
	char response[MAX_BUFFER_SIZE];	
	string content_type = "Content-Type: text/html\r\n";
	string content_length = "";
	string date = "";
	string rep = "";
	

	version += "404 Not Found\r\n";
	content_length = "Content-Length: " + to_string(getFileSize("notFound.txt")) + "\r\n\r\n"; 	rep = version + "Date: " + date + "Connection: Closed\r\n" + content_type + content_length + readFile("notFound.txt");
	strcpy(response, rep.c_str());

	writeSock(response, write_fd);
}

void serviceRequest(char *buff, FILE *write_fd){
	
	string getRequest(buff);
	string version = "";
	time_t now = time(0);
	string dt = ctime(&now);
	map<string,string> items;
	char response[MAX_BUFFER_SIZE];
	int first_pos = getRequest.find("GET");
	int last_pos;
	//int fileSize = 0;
		
	if(getRequest.find("HTTP/1.1") != -1){
		last_pos = getRequest.find(" HTTP/1.1");
		version = getVersion(getRequest);
	}else{
		last_pos = getRequest.find(" HTTP/1.0");
		version = getVersion(getRequest);
	}

	cerr << "Current buff: " << buff << "First: " << first_pos << "Last: " << last_pos << endl;

	string contentRequest = getRequest.substr(first_pos + 5,last_pos-first_pos - 5);

	cerr << "HERE Ri " << endl;
	//fileSize = getFileSize(contentRequest);
	if(getRequest.find("singup.lua?") != -1){
		try{
			first_pos = getFirstPosition(getRequest,"?firstname");
			map<string,string> items;
			first_pos = getRequest.find("?firstname");
			items = parseForm(getRequest.substr(first_pos+1, last_pos-first_pos-1));
			cerr << "Firstname: " << items["firstname"] << "Lastname: " << items["lastname"] << endl;			
			runLua(items);
			goodRequest(version, dt, "", true, write_fd);
		}catch(exception& e){}
	}else if(contentRequest == ""){
		cerr << "next" << endl;
		goodRequest(version, dt, "index.html", false, write_fd);
	}else{
		if(!ifstream(contentRequest)){
			badRequest(version, dt, write_fd);
		}else{
			goodRequest(version, dt, contentRequest, false, write_fd);
		}
	}

	cerr << "Contents: " << response << endl;
	
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
			cerr << "I QUIT" << endl;
			break;

		}
		
		cerr << "Reading " << buff << endl;
		if(strlen(buff) == 2){
			getRequest = false;	
		}

		if(getRequest == false && strlen(buff) != 2){
			getRequest = isGetRequest(buff);
			if(getRequest == true){
				serviceRequest(buff, write_fd);
			}
			//cerr << "Get Request is: " << getRequest << endl;
		}

		if(keepAlive == false && getRequest == true){
			keepAlive = shouldKeepAlive(buff);
		}

		cerr << "Get Request is: " << getRequest << "Keep is: " << keepAlive << endl;

		if (getRequest == false && keepAlive == false) {

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
	
	int i = 0;

	int clisock = (intptr_t) args;

	while(readSock(clisock));

	cerr << "Closing socket: " << i++ << endl;

	close(clisock);

	cerr << "Socket Closed: " << i++ << endl; 

	pthread_exit(NULL);

}

	

//Handles connection between client and server. Also performs all the work of receiving (STOR) and transferring (RTRV) files from the client.

void handleConnection(int clisock){


		int i = 0;
		pthread_attr_t attribs;

		pthread_t thread;

		pthread_attr_init(&attribs);
		
		cerr << "Setting up thread: " << i++ << endl;

		pthread_attr_setdetachstate(&attribs, PTHREAD_CREATE_DETACHED);

		pthread_create(&thread, &attribs, connectHandler, (void*)(intptr_t)clisock);
		

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
