/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _SUMIT_H_RPCGEN
#define _SUMIT_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct sum_in {
	int nums[100];
	int count;
};
typedef struct sum_in sum_in;

#define SUMIT 99
#define VER1 1

#if defined(__STDC__) || defined(__cplusplus)
#define sum_it 1
extern  int * sum_it_1(sum_in *, CLIENT *);
extern  int * sum_it_1_svc(sum_in *, struct svc_req *);
extern int sumit_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define sum_it 1
extern  int * sum_it_1();
extern  int * sum_it_1_svc();
extern int sumit_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_sum_in (XDR *, sum_in*);

#else /* K&R C */
extern bool_t xdr_sum_in ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_SUMIT_H_RPCGEN */
