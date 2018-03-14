#include <stdio.h>
#include <sqlite3.h>
#include "../Database/Database.h"

sqlite3 *bank1_db;
sqlite3 *bank2_db;

void establishConnection(){
	startDB(bank1_db, bank2_db);
}

int VB_credit(char* accountNum, int amount){
	int result = 0;

	result = accountLookUP(bank1_db, bank2_db, accountNum);

	if(result == 1){
		printf("Account: %s located at Bank 1\n", accountNum);
	}else{
		printf("Account: %s located at Bank 2\n", accountNum);
	}

	return result;
}

int VB_debit(char* accountNum, int amount){
	int result = 0;
	return result;
}


int VB_transfer(char* accountNum1, char* accountNum2, int amount){
	int result = 0;
	return result;
}

void done(){
	closeDB(bank1_db, bank2_db);
}