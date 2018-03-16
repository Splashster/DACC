/*************************************************************************
The client.c file is the main driver of the Virtual Banking Program.
This file manages the users input to see which type of transaction the user
would like to perform as well as the amount for the transaction.
This file also establishes a connection with the VirtualBank rpc server.
**************************************************************************/

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

	//creates remote connection to the virtual bank
	remote_client = clnt_create("127.0.0.1", VIRTUALBANK, VER1, "tcp");
	if (remote_client == NULL) {
		clnt_pcreateerror("127.0.0.1");
		exit(1);
	}


	while(1){
		getline(&input, &len, stdin);
		printf("Ready> %s", input);

		if(strcasecmp(input,"quit\n") == 0){
			printf("Goodbye\n");
			return 0;
		}else{
			//Checks to make sure that the user input isn't empty
			if(input[0] != '\n' && input[0] != ' '){
					transactionType = strtok(input, " ");
					//Checks to make sure a valid transaction or command was entered
					if(strcasecmp(transactionType, "credit") != 0 && strcasecmp(transactionType, "debit") != 0  && strcasecmp(transactionType, "transfer") != 0){
							printf("Invalid transaction type. Only Credit, Debit, or Transfer transactions allowed or type quit to exit.\n");
					}else{
							//Checks to see if the transaction type if credit
							if(strcasecmp(transactionType, "credit") == 0){
								vals.accountNum1 = strtok(NULL, " ");
								vals.accountNum2 = "";
								vals.amount = atoi(strtok(NULL, " "));
								
								
								result = vb_credit_1(&vals, remote_client);
								if (result == NULL) {
									clnt_perror(remote_client, "127.0.0.1");
									exit(1);
								}
								if(*result == 1){
									printf("OK: Added %i dollars to account %s\n", vals.amount, vals.accountNum1);
								}else if(*result == 3){
									printf("Error: Unable to locate Account Number: %s\n", vals.accountNum1);
								}

							//Checks to see if the transaction type if debit
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
									printf("OK: Subtracted %i dollars from account %s\n", vals.amount, vals.accountNum1);
								}else if(*result == 2){
									printf("Error: not enough funds\n");
								}else if(*result == 3){
									printf("Error: Unable to locate Account Number: %s\n", vals.accountNum1);
								}
							//Assumes the transaction is a transfer	
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
									printf("OK: Transfered %i dollars from account %s to account %s\n", vals.amount, vals.accountNum1, vals.accountNum2);
								}else if(*result == 2){
									printf("Error: not enough funds\n");
								}else if(*result == 3){
									printf("Error: Unable to locate Account Number: %s\n", vals.accountNum1);
								}else if(*result == 4){
									printf("Error: Unable to locate Account Number: %s\n", vals.accountNum2);
								}else if(*result == 5){
									printf("Error: Unable to locate Account Number: %s and Account Number: %s\n", vals.accountNum1, vals.accountNum2);
								}
							}

					     }
			}else if(input[0] != ' '){
					printf("Please perform a transaction or type quit to exit");
			}
		}
	}
}
