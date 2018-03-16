#include <stdio.h>
#include <sqlite3.h>
#include "Database.h"
#include "bank1.h"
#include "bank2.h"
#include "virtualbank.h"

int setup = 0;

void intializeDatabases(){
	setupDB(1);
	setupDB(2);
	setupDB(3);
	setup = 1;
}

int *cantFind(int codeNumber){
	static int nothing = 0;
	nothing = codeNumber;

	return &nothing;
}

int *vb_credit_1(accountInfo *vals, CLIENT *cl){
	int result = 0;
	int* transactionResult = 0;
	struct bank1AccountInfo b1_info;
	struct bank2AccountInfo b2_info;

	b1_info.accountNum = vals->accountNum1;
	b1_info.amount = vals->amount;

	b2_info.accountNum = vals->accountNum1;
	b2_info.amount = vals->amount;

	if(setup == 0){intializeDatabases();}

	result = accountLookUP(vals->accountNum1);

	if(result == 1){
		cl = clnt_create("127.0.0.1", BANK1, VER2, "tcp");
		if (cl == NULL) {
			clnt_pcreateerror("127.0.0.1");
			exit(1);
		}
		transactionResult = b1_credit_2(&b1_info, cl);
	}else if(result == 2){
		cl = clnt_create("127.0.0.1", BANK2, VER3, "tcp");
		if (cl == NULL) {
			clnt_pcreateerror("127.0.0.1");
			exit(1);
		}
		transactionResult = b2_credit_3(&b2_info, cl);
	}else{
		transactionResult = cantFind(3);
	}

	return transactionResult;
}

int *vb_debit_1(accountInfo *vals, CLIENT *cl){
	int result = 0;
	int* transactionResult = 0;

	struct bank1AccountInfo b1_info;
	struct bank2AccountInfo b2_info;

	b1_info.accountNum = vals->accountNum1;
	b1_info.amount = vals->amount;

	b2_info.accountNum = vals->accountNum1;
	b2_info.amount = vals->amount;

	if(setup == 0){intializeDatabases();}

	result = accountLookUP(vals->accountNum1);

	if(result == 1){
		cl = clnt_create("127.0.0.1", BANK1, VER2, "tcp");
		if (cl == NULL) {
			clnt_pcreateerror("127.0.0.1");
			exit(1);
		}
		transactionResult = b1_debit_2(&b1_info, cl);
	}else if(result == 2){
		cl = clnt_create("127.0.0.1", BANK2, VER3, "tcp");
		if (cl == NULL) {
			clnt_pcreateerror("127.0.0.1");
			exit(1);
		}
		transactionResult = b2_debit_3(&b2_info, cl);
	}else{
		transactionResult = cantFind(3);
	}
	return transactionResult;
}


int *vb_transfer_1(accountInfo *vals, CLIENT *cl){
	int acc1_location = 0;
	int acc2_location = 0;
	int* transactionResult = 0;

	if(setup == 0){intializeDatabases();}
	

	struct bank1AccountInfo b1_info;
	struct bank2AccountInfo b2_info;
	
	b1_info.amount = vals->amount;
	b2_info.amount = vals->amount;

	
	acc1_location = accountLookUP(vals->accountNum1);
	acc2_location = accountLookUP(vals->accountNum2);

	if((acc1_location == 1 || acc1_location == 2) && (acc2_location == 1 || acc2_location == 2)){
		if(acc1_location == 1){
			cl = clnt_create("127.0.0.1", BANK1, VER2, "tcp");
			if (cl == NULL) {
				clnt_pcreateerror("127.0.0.1");
				exit(1);
			}
			b1_info.accountNum = vals->accountNum1;
			transactionResult = b1_debit_2(&b1_info, cl);
			if(*transactionResult == 1){
				if(acc2_location == 1){
						b1_info.accountNum = vals->accountNum2;
						transactionResult = b1_credit_2(&b1_info, cl);
				}else{
					cl = clnt_create("127.0.0.1", BANK2, VER3, "tcp");
					if (cl == NULL) {
						clnt_pcreateerror("127.0.0.1");
						exit(1);
					}
					b2_info.accountNum = vals->accountNum2;
					transactionResult = b2_credit_3(&b2_info, cl);
				}
			}
		}else if(acc1_location == 2){
			cl = clnt_create("127.0.0.1", BANK2, VER3, "tcp");
			if (cl == NULL) {
				clnt_pcreateerror("127.0.0.1");
				exit(1);
			}
			b2_info.accountNum = vals->accountNum1;
			transactionResult = b2_debit_3(&b2_info, cl);
			if(*transactionResult == 1){
				if(acc2_location == 2){
						b2_info.accountNum = vals->accountNum2;
						transactionResult = b2_credit_3(&b2_info, cl);
				}else{
					cl = clnt_create("127.0.0.1", BANK1, VER2, "tcp");
					if (cl == NULL) {
						clnt_pcreateerror("127.0.0.1");
						exit(1);
					}
					b1_info.accountNum = vals->accountNum2;
					transactionResult = b1_credit_2(&b1_info, cl);
				}
		}
	}
	}else{
			if((acc1_location != 1 && acc1_location != 2) && (acc2_location == 1 || acc2_location == 2)){
				transactionResult = cantFind(3);
			}else if((acc1_location == 1 || acc1_location == 2) && (acc2_location != 1 && acc2_location != 2)){
				transactionResult = cantFind(4);
			}else{
				transactionResult = cantFind(5);
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
