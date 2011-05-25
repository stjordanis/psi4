#include <stdio.h>
#include <string.h>
#include "libint.h"
#include "hrr_header.h"

extern void vrr_order_g0g0(Libint_t*, prim_data*);

  /* Computes quartets of (g0|g0) integrals */

REALTYPE *hrr_order_g0g0(Libint_t *Libint, int num_prim_comb)
{
 prim_data *Data = Libint->PrimQuartet;
 REALTYPE *int_stack = Libint->int_stack;
 int i;

 Libint->vrr_classes[4][4] = int_stack + 0;
 memset(int_stack,0,225*sizeof(REALTYPE));

 Libint->vrr_stack = int_stack + 225;
 for(i=0;i<num_prim_comb;i++) {
   vrr_order_g0g0(Libint, Data);
   Data++;
 }
 /*--- compute (g0|g0) ---*/
 return int_stack+0;}
