#include <stdio.h>
#include <sqlite3.h>
#include "../Database/Database.h"
#include "../Bank1/Bank1.h"
#include "../Bank2/Bank2.h"


void intializeDatabase(){
	setupDB();
}

int VB_credit(char* accountNum, int amount){
	int result = 0;
	int transactionResult = 0;

	printf("Before result:%i\n", result);
	result = accountLookUP(accountNum);
	printf("The result: %i\n", result);

	if(result == 1){
		printf("Account: %s located at Bank 1\n", accountNum);
		transactionResult = b1_credit(accountNum,amount);
	}else if(result == 2){
		printf("Account: %s located at Bank 2.....\n", accountNum);
		printf("I should be working....\n");
		transactionResult = b2_credit(accountNum,amount);
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
		transactionResult = b2_debit(accountNum,amount);
	}else{
		printf("Unable to locate account: %s\n", accountNum);
	}

	return transactionResult;
}


int VB_transfer(char* accountNum1, char* accountNum2, int amount){
	int acc1_location = 0;
	int acc2_location = 0;
	int transactionResult = 0;

	acc1_location = accountLookUP(accountNum1);
	printf("working1\n");
	acc2_location = accountLookUP(accountNum2);
	printf("working2\n");

	if((acc1_location == 1 || acc1_location == 2) && (acc2_location == 1 || acc2_location == 2)){
		if(acc1_location == 1){
			printf("Account1: %s located at Bank 1\n", accountNum1);
			transactionResult = b1_debit(accountNum1,amount);
			if(transactionResult != 0){
				if(acc2_location == 1){
						transactionResult = b1_credit(accountNum2,amount);
				}else{
					transactionResult = b2_credit(accountNum2,amount);
				}
			}
		}else if(acc1_location == 2){
			printf("Account1: %s located at Bank 2\n", accountNum1);
			transactionResult = b2_debit(accountNum1,amount);
			if(transactionResult != 0){
				if(acc2_location == 2){
						transactionResult = b2_credit(accountNum2,amount);
				}else{
					transactionResult = b1_credit(accountNum2,amount);
				}
		}
	}
	}else{
			printf("Unable to locate account: %s\n", accountNum1);
			if((acc2_location != 1 && acc2_location != 2)){
				printf("Unable to locate account: %s\n", accountNum2);
			}
	}

	return transactionResult;
}
