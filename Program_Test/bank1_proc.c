#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "Database/Database.h"

int *b1_credit(struct accountInfo *vals, CLIENT *cl){
	static int result = 0;
	result = credit(1, vals->accountNum1, amount);
	return &result;
}

int *b1_debit(struct accountInfo *vals, CLIENT *cl){
	static int result = 0;
	if(input.accountNum2 == NULL){
		result = debit(1, vals->accountNum1, amount);
	}else{
		result = debit(1, vals->accountNum2, amount);
	}
	return &result;
}


int *b1_credit_2_svc(struct bank1AccountInfo *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(b1_credit_2(vals,client));
}

int *b1_debit_2_svc(struct bank1AccountInfo *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(b1_dedit_2(vals,client));
}