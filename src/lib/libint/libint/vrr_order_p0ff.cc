#include <stdio.h>
#include "libint.h"
#include "vrr_header.h"

  /* Computes quartets necessary to compute (p0|ff) integrals */

void vrr_order_p0ff(Libint_t * Libint, prim_data *Data)
{
 REALTYPE *vrr_stack = Libint->vrr_stack;
 REALTYPE *tmp, *target_ptr;
 int i, am[2];
 _BUILD_00p0(Data,vrr_stack+0, Data->F+2, Data->F+3, NULL, NULL, NULL);
 _BUILD_00p0(Data,vrr_stack+3, Data->F+1, Data->F+2, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+6, vrr_stack+3, vrr_stack+0, Data->F+1, Data->F+2, NULL);
 _BUILD_00p0(Data,vrr_stack+12, Data->F+3, Data->F+4, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+15, vrr_stack+0, vrr_stack+12, Data->F+2, Data->F+3, NULL);
 _BUILD_00f0(Data,vrr_stack+21, vrr_stack+6, vrr_stack+15, vrr_stack+3, vrr_stack+0, NULL);
 _BUILD_00p0(Data,vrr_stack+31, Data->F+0, Data->F+1, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+34, vrr_stack+31, vrr_stack+3, Data->F+0, Data->F+1, NULL);
 _BUILD_00f0(Data,vrr_stack+40, vrr_stack+34, vrr_stack+6, vrr_stack+31, vrr_stack+3, NULL);
 _BUILD_00p0(Data,vrr_stack+3, Data->F+4, Data->F+5, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+50, vrr_stack+12, vrr_stack+3, Data->F+3, Data->F+4, NULL);
 _BUILD_00f0(Data,vrr_stack+56, vrr_stack+15, vrr_stack+50, vrr_stack+0, vrr_stack+12, NULL);
 _BUILD_00g0(Data,vrr_stack+66, vrr_stack+21, vrr_stack+56, vrr_stack+6, vrr_stack+15, NULL);
 _BUILD_00g0(Data,vrr_stack+81, vrr_stack+40, vrr_stack+21, vrr_stack+34, vrr_stack+6, NULL);
 _BUILD_00p0(Data,vrr_stack+0, Data->F+5, Data->F+6, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+31, vrr_stack+3, vrr_stack+0, Data->F+4, Data->F+5, NULL);
 _BUILD_00f0(Data,vrr_stack+96, vrr_stack+50, vrr_stack+31, vrr_stack+12, vrr_stack+3, NULL);
 _BUILD_00g0(Data,vrr_stack+106, vrr_stack+56, vrr_stack+96, vrr_stack+15, vrr_stack+50, NULL);
 _BUILD_00h0(Data,vrr_stack+121, vrr_stack+66, vrr_stack+106, vrr_stack+21, vrr_stack+56, NULL);
 _BUILD_00h0(Data,vrr_stack+142, vrr_stack+81, vrr_stack+66, vrr_stack+40, vrr_stack+21, NULL);
 _BUILD_00p0(Data,vrr_stack+37, Data->F+6, Data->F+7, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+12, vrr_stack+0, vrr_stack+37, Data->F+5, Data->F+6, NULL);
 _BUILD_00f0(Data,vrr_stack+163, vrr_stack+31, vrr_stack+12, vrr_stack+3, vrr_stack+0, NULL);
 _BUILD_00g0(Data,vrr_stack+173, vrr_stack+96, vrr_stack+163, vrr_stack+50, vrr_stack+31, NULL);
 _BUILD_00h0(Data,vrr_stack+188, vrr_stack+106, vrr_stack+173, vrr_stack+56, vrr_stack+96, NULL);
 _BUILD_00i0(Data,vrr_stack+209, vrr_stack+121, vrr_stack+188, vrr_stack+66, vrr_stack+106, NULL);
 _BUILD_00i0(Data,vrr_stack+163, vrr_stack+142, vrr_stack+121, vrr_stack+81, vrr_stack+66, NULL);
 _BUILD_p0f0(Data,vrr_stack+237, vrr_stack+40, vrr_stack+21, NULL, NULL, vrr_stack+6);
   tmp = vrr_stack + 237;
   target_ptr = Libint->vrr_classes[1][3];
   for(i=0;i<30;i++)
     target_ptr[i] += tmp[i];
 _BUILD_p0g0(Data,vrr_stack+267, vrr_stack+81, vrr_stack+66, NULL, NULL, vrr_stack+21);
   tmp = vrr_stack + 267;
   target_ptr = Libint->vrr_classes[1][4];
   for(i=0;i<45;i++)
     target_ptr[i] += tmp[i];
 _BUILD_p0h0(Data,vrr_stack+0, vrr_stack+142, vrr_stack+121, NULL, NULL, vrr_stack+66);
   tmp = vrr_stack + 0;
   target_ptr = Libint->vrr_classes[1][5];
   for(i=0;i<63;i++)
     target_ptr[i] += tmp[i];
 _BUILD_p0i0(Data,vrr_stack+312, vrr_stack+163, vrr_stack+209, NULL, NULL, vrr_stack+121);
   tmp = vrr_stack + 312;
   target_ptr = Libint->vrr_classes[1][6];
   for(i=0;i<84;i++)
     target_ptr[i] += tmp[i];

}

