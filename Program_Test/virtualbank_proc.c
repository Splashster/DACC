#include <stdio.h>
#include <sqlite3.h>
#include "Database.h"
#include "bank1.h"
#include "bank2.h"
#include "virtualbank.h"


void intializeDatabases(){
	setupDB(1);
	setupDB(2);
	setupDB(3);
}

int *vb_credit_1(accountInfo *vals, CLIENT *cl){
	int result = 0;
	int* transactionResult = 0;
	intializeDatabases();

	result = accountLookUP(vals->accountNum1);

	printf("The account was:%s", vals->accountNum1);

	if(result == 1){
		cl = clnt_create("127.0.0.1", BANK1, VER2, "tcp");			
		if (cl == NULL) {
			clnt_pcreateerror("127.0.0.1");
			exit(1);
		}
		bank1AccountInfo *b1_info = (bank1AccountInfo*) &vals;
		printf("Account: %s located at Bank 1\n", b1_info->accountNum1);
		transactionResult = b1_credit_2(b1_info, cl);
	}else if(result == 2){
		cl = clnt_create("127.0.0.1", BANK2, VER3, "tcp");			
		if (cl == NULL) {
			clnt_pcreateerror("127.0.0.1");
			exit(1);
		}
		printf("Account: %s located at Bank 2\n", vals->accountNum1);
		bank2AccountInfo *b2_info = (bank2AccountInfo*) &vals;
		transactionResult = b2_credit_3(b2_info, cl);
	}else{
		printf("Unable to locate account: %s\n", vals->accountNum1);
	}

	return transactionResult;
}

int *vb_debit_1(accountInfo *vals, CLIENT *cl){
	int result = 0;
	int* transactionResult = 0;
	intializeDatabases();

	result = accountLookUP(vals->accountNum1);

	if(result == 1){
		cl = clnt_create("127.0.0.1", BANK1, VER2, "tcp");			
		if (cl == NULL) {
			clnt_pcreateerror("127.0.0.1");
			exit(1);
		}
		printf("Account: %s located at Bank 1\n", vals->accountNum1);
		bank1AccountInfo *b1_info = (bank1AccountInfo*) &vals;
		transactionResult = b1_debit_2(b1_info, cl);
	}else if(result == 2){
		cl = clnt_create("127.0.0.1", BANK2, VER3, "tcp");			
		if (cl == NULL) {
			clnt_pcreateerror("127.0.0.1");
			exit(1);
		}
		printf("Account: %s located at Bank 2\n", vals->accountNum1);
		bank2AccountInfo *b2_info = (bank2AccountInfo*) &vals;
		transactionResult = b2_debit_3(b2_info, cl);
	}else{
		printf("Unable to locate account: %s\n", vals->accountNum1);
	}

	return transactionResult;
}


int *vb_transfer_1(accountInfo *vals, CLIENT *cl){
	int acc1_location = 0;
	int acc2_location = 0;
	intializeDatabases();
	bank1AccountInfo *b1_info = (bank1AccountInfo*) &vals;
	bank2AccountInfo *b2_info = (bank2AccountInfo*) &vals;
	int* transactionResult = 0;

	acc1_location = accountLookUP(vals->accountNum1);
	acc2_location = accountLookUP(vals->accountNum2);

	if((acc1_location == 1 || acc1_location == 2) && (acc2_location == 1 || acc2_location == 2)){
		if(acc1_location == 1){
			cl = clnt_create("127.0.0.1", BANK1, VER2, "tcp");			
			if (cl == NULL) {
				clnt_pcreateerror("127.0.0.1");
				exit(1);
			}
			printf("Account1: %s located at Bank 1\n", vals->accountNum1);
			transactionResult = b1_debit_2(b1_info, cl);
			if(transactionResult != 0){
				if(acc2_location == 1){
						printf("Account1: %s located at Bank 1\n", vals->accountNum2);
						transactionResult = b1_credit_2(b1_info, cl);
				}else{
					cl = clnt_create("127.0.0.1", BANK2, VER3, "tcp");			
					if (cl == NULL) {
						clnt_pcreateerror("127.0.0.1");
						exit(1);
					}
					printf("Account2: %s located at Bank 1\n", vals->accountNum2);
					transactionResult = b2_credit_3(b2_info, cl);
				}
			}
		}else if(acc1_location == 2){
			cl = clnt_create("127.0.0.1", BANK2, VER3, "tcp");			
			if (cl == NULL) {
				clnt_pcreateerror("127.0.0.1");
				exit(1);
			}
			printf("Account1: %s located at Bank 2\n", vals->accountNum1);
			transactionResult = b2_debit_3(b2_info, cl);
			if(transactionResult != 0){
				if(acc2_location == 2){
						printf("Account2: %s located at Bank 2\n", vals->accountNum2);
						transactionResult = b2_credit_3(b2_info, cl);
				}else{
					cl = clnt_create("127.0.0.1", BANK1, VER2, "tcp");			
					if (cl == NULL) {
						clnt_pcreateerror("127.0.0.1");
						exit(1);
					}
					printf("Account2: %s located at Bank 1\n", vals->accountNum2);
					transactionResult = b1_credit_2(b1_info, cl);
				}
		}
	}
	}else{
			printf("Unable to locate account: %s\n", vals->accountNum1);
			if((acc2_location != 1 && acc2_location != 2)){
				printf("Unable to locate account: %s\n", vals->accountNum2);
			}
	}

	return transactionResult;
}

int *vb_credit_1_svc(struct accountInfo *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(vb_credit_1(vals,client));
}

int *vb_debit_1_svc(struct accountInfo *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(vb_debit_1(vals,client));
}

int *vb_transfer_1_svc(struct accountInfo *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(vb_transfer_1(vals,client));
}