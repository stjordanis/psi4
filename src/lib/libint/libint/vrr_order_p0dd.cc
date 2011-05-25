#include <stdio.h>
#include "libint.h"
#include "vrr_header.h"

  /* Computes quartets necessary to compute (p0|dd) integrals */

void vrr_order_p0dd(Libint_t * Libint, prim_data *Data)
{
 REALTYPE *vrr_stack = Libint->vrr_stack;
 REALTYPE *tmp, *target_ptr;
 int i, am[2];
 _BUILD_00p0(Data,vrr_stack+0, Data->F+1, Data->F+2, NULL, NULL, NULL);
 _BUILD_00p0(Data,vrr_stack+3, Data->F+2, Data->F+3, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+6, vrr_stack+0, vrr_stack+3, Data->F+1, Data->F+2, NULL);
 _BUILD_00p0(Data,vrr_stack+12, Data->F+0, Data->F+1, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+15, vrr_stack+12, vrr_stack+0, Data->F+0, Data->F+1, NULL);
 _BUILD_00p0(Data,vrr_stack+21, Data->F+3, Data->F+4, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+24, vrr_stack+3, vrr_stack+21, Data->F+2, Data->F+3, NULL);
 _BUILD_00f0(Data,vrr_stack+30, vrr_stack+6, vrr_stack+24, vrr_stack+0, vrr_stack+3, NULL);
 _BUILD_00f0(Data,vrr_stack+40, vrr_stack+15, vrr_stack+6, vrr_stack+12, vrr_stack+0, NULL);
 _BUILD_00p0(Data,vrr_stack+12, Data->F+4, Data->F+5, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+50, vrr_stack+21, vrr_stack+12, Data->F+3, Data->F+4, NULL);
 _BUILD_00f0(Data,vrr_stack+56, vrr_stack+24, vrr_stack+50, vrr_stack+3, vrr_stack+21, NULL);
 _BUILD_00g0(Data,vrr_stack+66, vrr_stack+30, vrr_stack+56, vrr_stack+6, vrr_stack+24, NULL);
 _BUILD_00g0(Data,vrr_stack+50, vrr_stack+40, vrr_stack+30, vrr_stack+15, vrr_stack+6, NULL);
 _BUILD_p0d0(Data,vrr_stack+81, vrr_stack+15, vrr_stack+6, NULL, NULL, vrr_stack+0);
   tmp = vrr_stack + 81;
   target_ptr = Libint->vrr_classes[1][2];
   for(i=0;i<18;i++)
     target_ptr[i] += tmp[i];
 _BUILD_p0f0(Data,vrr_stack+99, vrr_stack+40, vrr_stack+30, NULL, NULL, vrr_stack+6);
   tmp = vrr_stack + 99;
   target_ptr = Libint->vrr_classes[1][3];
   for(i=0;i<30;i++)
     target_ptr[i] += tmp[i];
 _BUILD_p0g0(Data,vrr_stack+129, vrr_stack+50, vrr_stack+66, NULL, NULL, vrr_stack+30);
   tmp = vrr_stack + 129;
   target_ptr = Libint->vrr_classes[1][4];
   for(i=0;i<45;i++)
     target_ptr[i] += tmp[i];

}

