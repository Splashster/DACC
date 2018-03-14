#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "../Database/Database.h"

int b1_credit(char* accountNum, int amount){
	int result = 0;
	result = credit(1, accountNum, amount);
	return result;
}

int b1_debit(char* accountNum, int amount){
	int result = 0;
	result = debit(1, accountNum, amount);
	return result;
}
