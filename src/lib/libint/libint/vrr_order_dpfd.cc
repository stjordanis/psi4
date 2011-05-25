#include <stdio.h>
#include "libint.h"
#include "vrr_header.h"

  /* Computes quartets necessary to compute (dp|fd) integrals */

void vrr_order_dpfd(Libint_t * Libint, prim_data *Data)
{
 REALTYPE *vrr_stack = Libint->vrr_stack;
 REALTYPE *tmp, *target_ptr;
 int i, am[2];
 _BUILD_00p0(Data,vrr_stack+0, Data->F+3, Data->F+4, NULL, NULL, NULL);
 _BUILD_00p0(Data,vrr_stack+3, Data->F+2, Data->F+3, NULL, NULL, NULL);
 _BUILD_p0p0(Data,vrr_stack+6, vrr_stack+3, vrr_stack+0, NULL, NULL, Data->F+3);
 _BUILD_00d0(Data,vrr_stack+15, vrr_stack+3, vrr_stack+0, Data->F+2, Data->F+3, NULL);
 _BUILD_00p0(Data,vrr_stack+21, Data->F+1, Data->F+2, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+24, vrr_stack+21, vrr_stack+3, Data->F+1, Data->F+2, NULL);
 _BUILD_00p0(Data,vrr_stack+30, Data->F+4, Data->F+5, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+33, vrr_stack+0, vrr_stack+30, Data->F+3, Data->F+4, NULL);
 _BUILD_p0d0(Data,vrr_stack+39, vrr_stack+15, vrr_stack+33, NULL, NULL, vrr_stack+0);
 _BUILD_p0d0(Data,vrr_stack+57, vrr_stack+24, vrr_stack+15, NULL, NULL, vrr_stack+3);
 _BUILD_d0d0(Data,vrr_stack+75, vrr_stack+57, vrr_stack+39, vrr_stack+24, vrr_stack+15, vrr_stack+6);
 _BUILD_00f0(Data,vrr_stack+111, vrr_stack+15, vrr_stack+33, vrr_stack+3, vrr_stack+0, NULL);
 _BUILD_00f0(Data,vrr_stack+121, vrr_stack+24, vrr_stack+15, vrr_stack+21, vrr_stack+3, NULL);
 _BUILD_p0f0(Data,vrr_stack+131, vrr_stack+121, vrr_stack+111, NULL, NULL, vrr_stack+15);
 _BUILD_00p0(Data,vrr_stack+3, Data->F+0, Data->F+1, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+6, vrr_stack+3, vrr_stack+21, Data->F+0, Data->F+1, NULL);
 _BUILD_00f0(Data,vrr_stack+161, vrr_stack+6, vrr_stack+24, vrr_stack+3, vrr_stack+21, NULL);
 _BUILD_p0f0(Data,vrr_stack+171, vrr_stack+161, vrr_stack+121, NULL, NULL, vrr_stack+24);
 _BUILD_00p0(Data,vrr_stack+21, Data->F+5, Data->F+6, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+201, vrr_stack+30, vrr_stack+21, Data->F+4, Data->F+5, NULL);
 _BUILD_00f0(Data,vrr_stack+207, vrr_stack+33, vrr_stack+201, vrr_stack+0, vrr_stack+30, NULL);
 _BUILD_p0f0(Data,vrr_stack+217, vrr_stack+111, vrr_stack+207, NULL, NULL, vrr_stack+33);
 _BUILD_d0f0(Data,vrr_stack+247, vrr_stack+131, vrr_stack+217, vrr_stack+121, vrr_stack+111, vrr_stack+39);
 _BUILD_d0f0(Data,vrr_stack+307, vrr_stack+171, vrr_stack+131, vrr_stack+161, vrr_stack+121, vrr_stack+57);
   tmp = vrr_stack + 307;
   target_ptr = Libint->vrr_classes[2][3];
   for(i=0;i<60;i++)
     target_ptr[i] += tmp[i];
 _BUILD_00g0(Data,vrr_stack+39, vrr_stack+111, vrr_stack+207, vrr_stack+15, vrr_stack+33, NULL);
 _BUILD_00g0(Data,vrr_stack+54, vrr_stack+121, vrr_stack+111, vrr_stack+24, vrr_stack+15, NULL);
 _BUILD_p0g0(Data,vrr_stack+367, vrr_stack+54, vrr_stack+39, NULL, NULL, vrr_stack+111);
 _BUILD_00g0(Data,vrr_stack+412, vrr_stack+161, vrr_stack+121, vrr_stack+6, vrr_stack+24, NULL);
 _BUILD_p0g0(Data,vrr_stack+427, vrr_stack+412, vrr_stack+54, NULL, NULL, vrr_stack+121);
 _BUILD_00p0(Data,vrr_stack+24, Data->F+6, Data->F+7, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+69, vrr_stack+21, vrr_stack+24, Data->F+5, Data->F+6, NULL);
 _BUILD_00f0(Data,vrr_stack+0, vrr_stack+201, vrr_stack+69, vrr_stack+30, vrr_stack+21, NULL);
 _BUILD_00g0(Data,vrr_stack+472, vrr_stack+207, vrr_stack+0, vrr_stack+33, vrr_stack+201, NULL);
 _BUILD_p0g0(Data,vrr_stack+487, vrr_stack+39, vrr_stack+472, NULL, NULL, vrr_stack+207);
 _BUILD_d0g0(Data,vrr_stack+532, vrr_stack+367, vrr_stack+487, vrr_stack+54, vrr_stack+39, vrr_stack+217);
 _BUILD_d0g0(Data,vrr_stack+622, vrr_stack+427, vrr_stack+367, vrr_stack+412, vrr_stack+54, vrr_stack+131);
   tmp = vrr_stack + 622;
   target_ptr = Libint->vrr_classes[2][4];
   for(i=0;i<90;i++)
     target_ptr[i] += tmp[i];
 _BUILD_00h0(Data,vrr_stack+217, vrr_stack+39, vrr_stack+472, vrr_stack+111, vrr_stack+207, NULL);
 _BUILD_00h0(Data,vrr_stack+712, vrr_stack+54, vrr_stack+39, vrr_stack+121, vrr_stack+111, NULL);
 _BUILD_p0h0(Data,vrr_stack+733, vrr_stack+712, vrr_stack+217, NULL, NULL, vrr_stack+39);
 _BUILD_00h0(Data,vrr_stack+27, vrr_stack+412, vrr_stack+54, vrr_stack+161, vrr_stack+121, NULL);
 _BUILD_p0h0(Data,vrr_stack+796, vrr_stack+27, vrr_stack+712, NULL, NULL, vrr_stack+54);
 _BUILD_00p0(Data,vrr_stack+161, Data->F+7, Data->F+8, NULL, NULL, NULL);
 _BUILD_00d0(Data,vrr_stack+164, vrr_stack+24, vrr_stack+161, Data->F+6, Data->F+7, NULL);
 _BUILD_00f0(Data,vrr_stack+412, vrr_stack+69, vrr_stack+164, vrr_stack+21, vrr_stack+24, NULL);
 _BUILD_00g0(Data,vrr_stack+111, vrr_stack+0, vrr_stack+412, vrr_stack+201, vrr_stack+69, NULL);
 _BUILD_00h0(Data,vrr_stack+48, vrr_stack+472, vrr_stack+111, vrr_stack+207, vrr_stack+0, NULL);
 _BUILD_p0h0(Data,vrr_stack+859, vrr_stack+217, vrr_stack+48, NULL, NULL, vrr_stack+472);
 _BUILD_d0h0(Data,vrr_stack+922, vrr_stack+733, vrr_stack+859, vrr_stack+712, vrr_stack+217, vrr_stack+487);
 _BUILD_d0h0(Data,vrr_stack+1048, vrr_stack+796, vrr_stack+733, vrr_stack+27, vrr_stack+712, vrr_stack+367);
   tmp = vrr_stack + 1048;
   target_ptr = Libint->vrr_classes[2][5];
   for(i=0;i<126;i++)
     target_ptr[i] += tmp[i];
 _BUILD_f0f0(Data,vrr_stack+1174, vrr_stack+307, vrr_stack+247, vrr_stack+171, vrr_stack+131, vrr_stack+75);
   tmp = vrr_stack + 1174;
   target_ptr = Libint->vrr_classes[3][3];
   for(i=0;i<100;i++)
     target_ptr[i] += tmp[i];
 _BUILD_f0g0(Data,vrr_stack+0, vrr_stack+622, vrr_stack+532, vrr_stack+427, vrr_stack+367, vrr_stack+247);
   tmp = vrr_stack + 0;
   target_ptr = Libint->vrr_classes[3][4];
   for(i=0;i<150;i++)
     target_ptr[i] += tmp[i];
 _BUILD_f0h0(Data,vrr_stack+150, vrr_stack+1048, vrr_stack+922, vrr_stack+796, vrr_stack+733, vrr_stack+532);
   tmp = vrr_stack + 150;
   target_ptr = Libint->vrr_classes[3][5];
   for(i=0;i<210;i++)
     target_ptr[i] += tmp[i];

}

