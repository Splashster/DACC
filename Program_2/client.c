#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>


int doesFileExist(char* filename){
	FILE *file;
	if(file = fopen(filename, "r")){
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[]){
	CLIENT *remote_client;
	char* line = NULL;
	char* transactionType;
	char* accountNum;
	char* ammount;
	size_t len = 0;
	FILE *file;

	if(argc < 1){
			printf("Missing filename");
	}else{
			if(!doesFileExist(argv[1])){
				printf("Unable to find file");
				return 0;
			}else{
				file = fopen(argv[1], "r");
				if(file){
					while(getline(&line, &len, file) != -1)
					{
						if(line[0] != '\n'){
							transactionType = strtok(line, " ");
							if(strcasecmp(transactionType, "credit") != 0 && strcasecmp(transactionType, "transfer") != 0){
								printf("Invalid Transaction\n");
							}else{
								accountNum = strtok(NULL, " ");
								ammount = strtok(NULL, " ");
								if(strcasecmp(transactionType, "credit") == 0){
									printf("Ready> credit %s %i\n", accountNum, ammount);
								}else{
									printf("Ready> transfer %s %i\n", accountNum, ammount);
								}
		
			     			}
						}
							
						}
					}
					fclose(file);
				}
			}
}
