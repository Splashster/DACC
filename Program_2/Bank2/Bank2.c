#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "../Database/Database.h"

int b2_credit(char* accountNum, int amount){
	int result = 0;
	result = credit(2, accountNum, amount);
	return result;
}

int b2_debit(char*accountNum, int amount){
	int result = 0;
	result = debit(2, accountNum, amount);
	return result;
}
