/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _BANK1_H_RPCGEN
#define _BANK1_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct accountInfo {
	char *accountNum1;
	char *accountNum2;
	int amount;
};
typedef struct accountInfo accountInfo;

#define BANK1 101
#define VER2 2

#if defined(__STDC__) || defined(__cplusplus)
#define b1_credit 3
extern  int * b1_credit_2(accountInfo *, CLIENT *);
extern  int * b1_credit_2_svc(accountInfo *, struct svc_req *);
#define b1_debit 4
extern  int * b1_debit_2(accountInfo *, CLIENT *);
extern  int * b1_debit_2_svc(accountInfo *, struct svc_req *);
extern int bank1_2_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define b1_credit 3
extern  int * b1_credit_2();
extern  int * b1_credit_2_svc();
#define b1_debit 4
extern  int * b1_debit_2();
extern  int * b1_debit_2_svc();
extern int bank1_2_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_accountInfo (XDR *, accountInfo*);

#else /* K&R C */
extern bool_t xdr_accountInfo ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_BANK1_H_RPCGEN */