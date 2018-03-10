#include <stdio.h>
#include <rpc/rpc.h>
#include "sumit.h"

int *sum_it_1(struct sum_in *vals, CLIENT *cl) {
	static int all = 0;
	int i;
	all = 0;
	for (i = 0; i < vals->count; i++) {
		all += vals->nums[i];
	}	
	return &all;
}

int *sum_it_1_svc(struct sum_in *vals,
   struct svc_req *svc) {
  CLIENT *client;
  return(sum_it_1(vals,client));
}

