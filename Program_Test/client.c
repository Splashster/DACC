#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "virtualbank.h"

int main(int argc, char* argv[]){
	CLIENT *remote_client;
	char* input = NULL;
	int count;
	char* transactionType;
	size_t len = 0;
	int* result = 0;
	FILE *file;
	struct accountInfo vals;

	remote_client = clnt_create("127.0.0.1", VIRTUALBANK, VER1, "tcp");
	if (remote_client == NULL) {
		clnt_pcreateerror("127.0.0.1");
		exit(1);
	}


	while(1){

		printf("Ready> ");
		getline(&input, &len, stdin);
		count = 0;

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
								vals.accountNum1 = strtok(NULL, " ");
								vals.accountNum2 = "";
								vals.amount = atoi(strtok(NULL, " "));

								printf("Vals:%i\n", vals.amount);
								
								
								result = vb_credit_1(&vals, remote_client);
								if (result == NULL) {
									clnt_perror(remote_client, "127.0.0.1");
									exit(1);
								}
								if(*result == 1){
									printf("Added %i dollars to account %s\n", vals.amount, vals.accountNum1);
								}else if(*result == 3){
									printf("Unable to locate Account Number: %s\n", vals.accountNum1);
								}


							}else if(strcasecmp(transactionType, "debit") == 0){
								vals.accountNum1 = strtok(NULL, " ");
								vals.accountNum2 = "";
								vals.amount = atoi(strtok(NULL, " "));

								result = vb_debit_1(&vals,remote_client);
								if (result == NULL) {
									clnt_perror(remote_client, "127.0.0.1");
									exit(1);
								}
								if(*result == 1){
									printf("Subtracted %i dollars to account %s\n", vals.amount, vals.accountNum1);
								}else if(*result == 2){
									printf("Error: not enough funds\n");
								}else if(*result == 3){
									printf("Unable to locate Account Number: %s\n", vals.accountNum1);
								}

							}else{

								vals.accountNum1 = strtok(NULL, " ");
								vals.accountNum2 = strtok(NULL, " ");
								vals.amount = atoi(strtok(NULL, " "));
								
								result = vb_transfer_1(&vals, remote_client);
								if (result == NULL) {
									clnt_perror(remote_client, "127.0.0.1");
									exit(1);
								}
								if(*result == 1){
									printf("Transfered %i dollars from account %s to account %s\n", vals.amount, vals.accountNum1, vals.accountNum2);
								}else if(*result == 2){
									printf("Error: not enough funds\n");
								}else if(*result == 3){
									printf("Unable to locate Account Number: %s\n", vals.accountNum1);
								}else if(*result == 4){
									printf("Unable to locate Account Number: %s\n", vals.accountNum2);
								}else if(*result == 5){
									printf("Unable to locate Account Number: %s and Account Number: %s\n", vals.accountNum1, vals.accountNum2);
								}
							}

					     }
			}else if(input[0] != ' '){
					printf("Please perform a transaction or type quit to exit");
			}
		}
	}
}
