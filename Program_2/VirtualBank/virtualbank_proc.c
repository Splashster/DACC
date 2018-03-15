#include <stdio.h>
#include <sqlite3.h>
#include "../Database/Database.h"
#include "bank1.h"
#include "bank2.h"


void intializeDatabase(){
	setupDB();
}

int *vb_credit_1(struct accountInfo *vals, CLIENT *cl){
	int result = 0;
	static int transactionResult = 0;

	result = accountLookUP(vals->accountNum1);

	if(result == 1){
		printf("Account: %s located at Bank 1\n", vals->accountNum1);
		transactionResult = b1_credit(&vals, cl);
	}else if(result == 2){
		printf("Account: %s located at Bank 2\n", vals->accountNum1);
		transactionResult = b2_credit(&vals, cl);
	}else{
		printf("Unable to locate account: %s\n", vals->accountNum1);
	}

	return &transactionResult;
}

int *vb_debit_1(struct accountInfo *vals, CLIENT *cl){
	int result = 0;
	static int transactionResult = 0;

	result = accountLookUP(vals->accountNum1);

	if(result == 1){
		printf("Account: %s located at Bank 1\n", vals->accountNum1);
		transactionResult = b1_debit(&vals, cl);
	}else if(result == 2){
		printf("Account: %s located at Bank 2\n", vals->accountNum1);
		transactionResult = b2_debit(&vals, cl);
	}else{
		printf("Unable to locate account: %s\n", vals->accountNum1);
	}

	return &transactionResult;
}


int *vb_transfer_1(struct accountInfo *vals, CLIENT *cl){
	int acc1_location = 0;
	int acc2_location = 0;
	static int transactionResult = 0;

	acc1_location = accountLookUP(vals->accountNum1);
	acc2_location = accountLookUP(vals->accountNum2);

	if((acc1_location == 1 || acc1_location == 2) && (acc2_location == 1 || acc2_location == 2)){
		if(acc1_location == 1){
			printf("Account1: %s located at Bank 1\n", vals->accountNum1);
			transactionResult = b1_debit(&vals, cl);
			if(transactionResult != 0){
				if(acc2_location == 1){
						printf("Account1: %s located at Bank 1\n", vals->accountNum2);
						transactionResult = b1_credit(&vals, cl);
				}else{
					printf("Account2: %s located at Bank 1\n", vals->accountNum2);
					transactionResult = b2_credit(&vals, cl);
				}
			}
		}else if(acc1_location == 2){
			printf("Account1: %s located at Bank 2\n", vals->accountNum1);
			transactionResult = b2_debit(&vals, cl);
			if(transactionResult != 0){
				if(acc2_location == 2){
						printf("Account2: %s located at Bank 2\n", vals->accountNum2);
						transactionResult = b2_credit(&vals, cl);
				}else{
					printf("Account2: %s located at Bank 1\n", vals->accountNum2);
					transactionResult = b1_credit(&vals, cl);
				}
		}
	}
	}else{
			printf("Unable to locate account: %s\n", vals->accountNum1);
			if((acc2_location != 1 && acc2_location != 2)){
				printf("Unable to locate account: %s\n", vals->accountNum2);
			}
	}

	return &transactionResult;
}

int *vb_credit_1_svc(struct sum_in *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(vb_credit_1(vals,client));
}

int *vb_debit_1_svc(struct sum_in *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(vb_debit_1(vals,client));
}

int *vb_transfer_1_svc(struct sum_in *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(vb_transfer_1(vals,client));
}