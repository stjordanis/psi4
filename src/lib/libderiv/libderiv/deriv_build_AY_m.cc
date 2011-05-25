#include <libint/libint.h>
#include "libderiv.h"

void deriv_build_AY_m(prim_data *Data, const int bcd_num, double *vp, const double *I0, const double *I1)
{
  const double twotzeta = Data->twozeta_a;
  const double *i0, *i1;
  int bcd;

  i0 = I0 + 1*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 3*bcd_num;
  i1 = I1;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 4*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 6*bcd_num;
  i1 = I1 + 1*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 7*bcd_num;
  i1 = I1 + 2*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 8*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 10*bcd_num;
  i1 = I1 + 3*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 11*bcd_num;
  i1 = I1 + 4*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 12*bcd_num;
  i1 = I1 + 5*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 13*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 15*bcd_num;
  i1 = I1 + 6*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 16*bcd_num;
  i1 = I1 + 7*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 17*bcd_num;
  i1 = I1 + 8*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 18*bcd_num;
  i1 = I1 + 9*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 19*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 21*bcd_num;
  i1 = I1 + 10*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 22*bcd_num;
  i1 = I1 + 11*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 23*bcd_num;
  i1 = I1 + 12*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 24*bcd_num;
  i1 = I1 + 13*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 25*bcd_num;
  i1 = I1 + 14*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 26*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 28*bcd_num;
  i1 = I1 + 15*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 6.000000*(*(i1++));
  i0 = I0 + 29*bcd_num;
  i1 = I1 + 16*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 30*bcd_num;
  i1 = I1 + 17*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 31*bcd_num;
  i1 = I1 + 18*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 32*bcd_num;
  i1 = I1 + 19*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 33*bcd_num;
  i1 = I1 + 20*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 34*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 36*bcd_num;
  i1 = I1 + 21*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 7.000000*(*(i1++));
  i0 = I0 + 37*bcd_num;
  i1 = I1 + 22*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 6.000000*(*(i1++));
  i0 = I0 + 38*bcd_num;
  i1 = I1 + 23*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 39*bcd_num;
  i1 = I1 + 24*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 40*bcd_num;
  i1 = I1 + 25*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 41*bcd_num;
  i1 = I1 + 26*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 42*bcd_num;
  i1 = I1 + 27*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 43*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 45*bcd_num;
  i1 = I1 + 28*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 8.000000*(*(i1++));
  i0 = I0 + 46*bcd_num;
  i1 = I1 + 29*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 7.000000*(*(i1++));
  i0 = I0 + 47*bcd_num;
  i1 = I1 + 30*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 6.000000*(*(i1++));
  i0 = I0 + 48*bcd_num;
  i1 = I1 + 31*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 49*bcd_num;
  i1 = I1 + 32*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 50*bcd_num;
  i1 = I1 + 33*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 51*bcd_num;
  i1 = I1 + 34*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 52*bcd_num;
  i1 = I1 + 35*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 53*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 55*bcd_num;
  i1 = I1 + 36*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 9.000000*(*(i1++));
  i0 = I0 + 56*bcd_num;
  i1 = I1 + 37*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 8.000000*(*(i1++));
  i0 = I0 + 57*bcd_num;
  i1 = I1 + 38*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 7.000000*(*(i1++));
  i0 = I0 + 58*bcd_num;
  i1 = I1 + 39*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 6.000000*(*(i1++));
  i0 = I0 + 59*bcd_num;
  i1 = I1 + 40*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 60*bcd_num;
  i1 = I1 + 41*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 61*bcd_num;
  i1 = I1 + 42*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 62*bcd_num;
  i1 = I1 + 43*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 63*bcd_num;
  i1 = I1 + 44*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 64*bcd_num;
  for(bcd=0;bcd<bcd_num;bcd++)
    *(vp++) = twotzeta*(*(i0++)) ;
}
