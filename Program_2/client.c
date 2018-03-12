#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>

int main(int argc, char* argv[]){
	CLIENT *remote_client;
	
	if(argc < 3){
		printf("Missing arguments!\n");
	}else{
		if(strcasecmp(argv[1], "credit") != 0 && strcasecmp(argv[1], "transfer") != 0){
			printf("Invalid Transaction\n");
		}else{
			if(strcasecmp(argv[1], "credit") == 0){
				printf("You want to credit: %i to %s\n", atoi(argv[3]), argv[2]);
			}else{
				printf("You want to transfer: %i to %s\n", atoi(argv[3]), argv[2]);
			}
	
		     }
	}
}
