#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "VirtualBank/virtualbank.h"

int main(int argc, char* argv[]){
	CLIENT *remote_client;
	char* input = NULL;
	char* transactionType;
	char* accountNum1;
	char* accountNum2;
	int amount = 0;
	int count = 0;
	size_t len = 0;
	int* result = 0;
	FILE *file;
	struct accountInfo input;

	remote_client = clnt_create(argv[1], VIRTUALBANK, VER1, "tcp");			
	if (remote_client == NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}
	

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
							printf("Invalid transaction type. Only Credit, Debit, or Transfer transactions allowed or type quit to exit.\n");
					}else{
							if(strcasecmp(transactionType, "credit") == 0){
								input.accountNum1 = strtok(NULL, " ");
								input.accountNum2 = NULL;
								input.amount = atoi(strtok(NULL, " "));
								result = vb_credit_1(&input, remote_client);
								if (result == NULL) {
									clnt_perror(remote_client, argv[1]);
									exit(1);
								}
								if(result != 0){
									printf("Added %i dollars to account %s\n", amount, accountNum1);
								}else{
									printf("Error: Unable to credit funds\n");
								}
								
							}else if(strcasecmp(transactionType, "debit") == 0){
								input.accountNum1 = strtok(NULL, " ");
								input.accountNum2 = NULL;
								input.amount = atoi(strtok(NULL, " "));
								result = vb_debit_1(&input,remote_client);
								if (result == NULL) {
									clnt_perror(remote_client, argv[1]);
									exit(1);
								}
								if(result != 0){
									printf("Subtracted %i dollars to account %s\n", amount, accountNum1);
								}else{
									printf("Error: not enough funds\n");
								}
								
							}else{
								input.accountNum1 = strtok(NULL, " ");
								input.accountNum2 = strtok(NULL, " ");
								input.amount = atoi(strtok(NULL, " "));
								result = vb_transfer_1(&input, remote_client);
								if (result == NULL) {
									clnt_perror(remote_client, argv[1]);
									exit(1);
								}
								if(result != 0){
									printf("Transfered %i dollars from account %s to account %s\n", amount, accountNum1, accountNum2);
								}else{
									printf("Error: not enough funds\n");
								}
								
							}

					     }
			}else if(input[0] != ' '){
					printf("Please enter perform a transaction or type quit to exit");
			}
		}
	}
}
