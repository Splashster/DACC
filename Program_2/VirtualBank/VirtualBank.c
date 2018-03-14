#include <stdio.h>
#include <sqlite3.h>
#include "../Database/Database.h"
#include "../Bank1/Bank1.h"


void intializeDatabase(){
	setupDB();
}

int VB_credit(char* accountNum, int amount){
	int result = 0;
	int transactionResult = 0;

	result = accountLookUP(accountNum);

	if(result == 1){
		printf("Account: %s located at Bank 1\n", accountNum);
		transactionResult = b1_credit(accountNum,amount);
	}else if(result == 2){
		printf("Account: %s located at Bank 2\n", accountNum);
		
	}else{
		printf("Unable to locate account: %s\n", accountNum);
	}

	return transactionResult;
}

int VB_debit(char* accountNum, int amount){
	int result = 0;
	int transactionResult = 0;

	result = accountLookUP(accountNum);

	if(result == 1){
		printf("Account: %s located at Bank 1\n", accountNum);
		transactionResult = b1_debit(accountNum,amount);
	}else if(result == 2){
		printf("Account: %s located at Bank 2\n", accountNum);
		
	}else{
		printf("Unable to locate account: %s\n", accountNum);
	}

	return transactionResult;
}


int VB_transfer(char* accountNum1, char* accountNum2, int amount){
	int result = 0;
	return result;
}

void done(){
	//closeDB();
}