#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <VirtualBank.h>
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
	char* input = NULL;
	char* transactionType;
	char* accountNum1;
	char* accountNum2;
	int amount = 0;
	int count = 0;
	size_t len = 0;
	FILE *file;

	while(1){
		printf("Ready> ");
		getline(&input, &len, stdin);

		if(strcasecmp(input,"quit\n") == 0){
			printf("Goodbye\n");
			return 0;
		}else{
			if(input[0] != '\n' && input[0] != ' '){
					transactionType = strtok(input, " ");
					if(strcasecmp(transactionType, "credit") != 0 && strcasecmp(transactionType, "debit") != 0  && strcasecmp(transactionType, "transfer") != 0){
							printf("Invalid transaction type. Only Credit, Debit, or Transfer transactions allowed.\n");
					}else{
							if(strcasecmp(transactionType, "credit") == 0){
								accountNum1 = strtok(NULL, " ");
								amount = atoi(strtok(NULL, " "));
								VB_credit(accountNum1, amount);
								printf("Added %i dollars to account %s\n", amount, accountNum1);
							}else if(strcasecmp(transactionType, "debit") == 0){
								accountNum1 = strtok(NULL, " ");
								amount = atoi(strtok(NULL, " "));
								VB_debit(accountNum1, amount);
								printf("Subtracted %i dollars to account %s\n", amount, accountNum1);
							}else{
								accountNum1 = strtok(NULL, " ");
								accountNum2 = strtok(NULL, " ");
								amount = atoi(strtok(NULL, " "));
								VB_transfer(accountNum1, accountNum2, amount);
								printf("Transfered %i dollars from account %s to account %s\n", amount, accountNum1, accountNum2);
							}

					     }
			}else if(input[0] != ' '){
					printf("Please enter perform a transaction or type quit to exit");
			}
		}
		

		
	}


	/*if(argc < 3){
		printf("Missing arguments!\n");
	}else{
		if(strcasecmp(transactionType, "credit") != 0 && strcasecmp(transactionType, "debit") != 0  && strcasecmp(transactionType, "transfer") != 0){
			printf("Invalid transaction type. Only Credit, Debit, or Transfer transactions allowed.\n");
		}else{
			if(strcasecmp(transactionType, "credit") == 0){
				accountNum1 = strtok(NULL, " ");
				amount = atoi(strtok(NULL, " "));
				printf("Ready> credit %s %i\n", accountNum1, amount);
			}else if(strcasecmp(transactionType, "debit") == 0){
				accountNum1 = strtok(NULL, " ");
				amount = atoi(strtok(NULL, " "));
				printf("Ready> debit %s %i\n", accountNum1, amount);
			}else{
				accountNum1 = strtok(NULL, " ");
				accountNum2 = strtok(NULL, " ");
				amount = atoi(strtok(NULL, " "));
				printf("Ready> transfer %s %i\n", accountNum1, accountNum2, amount);
			}
	}*/

	/*if(argc < 1){
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
								printf("Invalid transaction type. Only Credit, Debit, or Transfer transactions allowed.\n");
							}else{
								if(strcasecmp(transactionType, "credit") == 0){
									accountNum1 = strtok(NULL, " ");
									amount = atoi(strtok(NULL, " "));
									printf("Ready> credit %s %i\n", accountNum1, amount);
								}else if(strcasecmp(transactionType, "debit") == 0){
									accountNum1 = strtok(NULL, " ");
									amount = atoi(strtok(NULL, " "));
									printf("Ready> debit %s %i\n", accountNum1, amount);
								}else{
									accountNum1 = strtok(NULL, " ");
									accountNum2 = strtok(NULL, " ");
									amount = atoi(strtok(NULL, " "));
									printf("Ready> transfer %s %i\n", accountNum1, accountNum2, amount);
								}

			     			}
						}

						}
					}
					fclose(file);
				}
			}*/
}
