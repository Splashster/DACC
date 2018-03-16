/*************************************************************************
The virtualbank_proc.c file acts and a middleware server and passes the users'
request to the approriate bank rpc servers.
**************************************************************************/

#include <stdio.h>
#include <sqlite3.h>
#include "database.h"
#include "bank1.h"
#include "bank2.h"
#include "virtualbank.h"

int setup = 0;

//Setup up the databases
void intializeDatabases(){
	setupDB(1);
	setupDB(2);
	setupDB(3);
	setup = 1;
}

//Stores and returns an error code
int *cantFind(int codeNumber){
	static int nothing = 0;
	nothing = codeNumber;

	return &nothing;
}

//Looks up which bank the account is in and
//calls the credit function for the bank the account
//was located in.
//If transaction succesful 1 is returned.
//If account number was not found a 3 is returned.
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

//Looks up which bank the account is in and
//calls the debit function for the bank the account
//was located in.
//If transaction succesful 1 is returned.
//If transaction failed due to insufficient funds a 2
//is returned 
//If account number was not found a 3 is returned
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

//Looks up which bank the accounts are in and
//calls the debit and credit function for the bank(s) 
//the accounts were located in.
//If transaction succesful 1 is returned.
//If transaction failed due to insufficient funds a 2
//is returned 
//If account number1 was not found a 3 is returned
//If account number2 was not found a 4 is returned
//If neither account number1 nor 2 are found a 5 is returned
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
