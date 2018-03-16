#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include "Database.h"
#include "bank1.h"

int *b1_credit_2(struct bank1AccountInfo *vals, CLIENT *cl){
	static int result = 0;
	result = credit(1, vals->accountNum, vals->amount);
	
	return &result;
}

int *b1_debit_2(struct bank1AccountInfo *vals, CLIENT *cl){
	static int result = 0;
	result = debit(1, vals->accountNum, vals->amount);
	
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
  return(b1_debit_2(vals,client));
}
