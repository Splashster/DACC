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
	char* accountNum1;
	char* accountNum2;
	int amount = 0;
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
							if(strcasecmp(transactionType, "credit") != 0 && strcasecmp(transactionType, "debit") != 0  && strcasecmp(transactionType, "transfer") != 0){
								printf("Invalid Transaction\n");
							}else{
								if(strcasecmp(transactionType, "credit") == 0){
									accountNum1 = strtok(NULL, " ");
									//amount = atoi(strtok(NULL, " "));
									printf("Ready> credit %s %i\n", accountNum1, amount);
								}else if(strcasecmp(transactionType, "debit") == 0){
									accountNum1 = strtok(NULL, " ");
									//amount = atoi(strtok(NULL, " "));
									printf("Ready> debit %s %i\n", accountNum1, amount);
								}else{
									accountNum1 = strtok(NULL, " ");
									accountNum2 = strtok(NULL, " ");
									//amount = atoi(strtok(NULL, " "));
									printf("Ready> transfer %s %i\n", accountNum1, accountNum2, amount);
								}

			     			}
						}

						}
					}
					fclose(file);
				}
			}
}
