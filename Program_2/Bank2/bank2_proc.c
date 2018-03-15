#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "../Database/Database.h"

int *b2_credit(struct accountInfo *vals, CLIENT *cl){
	int result = 0;
	result = credit(2, vals->accountNum1, amount);
	return &result;
}

int *b2_debit(struct accountInfo *vals, CLIENT *cl){
	int result = 0;
	if(input.accountNum2 == NULL){
		result = debit(2, vals->accountNum1, amount);
	}else{
		result = debit(2, vals->accountNum2, amount);
	}
	return &result;
}


int *b2_credit_3_svc(struct sum_in *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(b2_credit_3(vals,client));
}

int *b2_debit_3_svc(struct sum_in *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(b1_debit_3(vals,client));
}