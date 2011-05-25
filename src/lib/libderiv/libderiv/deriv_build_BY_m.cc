#include <libint/libint.h>
#include "libderiv.h"

void deriv_build_BY_m(prim_data *Data, const int a_num, const int cd_num, double *vp, const double *I0, const double *I1)
{
  const double twotzeta = Data->twozeta_b;
  const double *i0, *i1;
  int a,cd;

  for(a=0;a<a_num;a++) {
  i0 = I0 + 1*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 3*cd_num;
  i1 = I1;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 4*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 6*cd_num;
  i1 = I1 + 1*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 7*cd_num;
  i1 = I1 + 2*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 8*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 10*cd_num;
  i1 = I1 + 3*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 11*cd_num;
  i1 = I1 + 4*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 12*cd_num;
  i1 = I1 + 5*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 13*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 15*cd_num;
  i1 = I1 + 6*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 16*cd_num;
  i1 = I1 + 7*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 17*cd_num;
  i1 = I1 + 8*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 18*cd_num;
  i1 = I1 + 9*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 19*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 21*cd_num;
  i1 = I1 + 10*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 22*cd_num;
  i1 = I1 + 11*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 23*cd_num;
  i1 = I1 + 12*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 24*cd_num;
  i1 = I1 + 13*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 25*cd_num;
  i1 = I1 + 14*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 26*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 28*cd_num;
  i1 = I1 + 15*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 6.000000*(*(i1++));
  i0 = I0 + 29*cd_num;
  i1 = I1 + 16*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 30*cd_num;
  i1 = I1 + 17*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 31*cd_num;
  i1 = I1 + 18*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 32*cd_num;
  i1 = I1 + 19*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 33*cd_num;
  i1 = I1 + 20*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 34*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 36*cd_num;
  i1 = I1 + 21*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 7.000000*(*(i1++));
  i0 = I0 + 37*cd_num;
  i1 = I1 + 22*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 6.000000*(*(i1++));
  i0 = I0 + 38*cd_num;
  i1 = I1 + 23*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 39*cd_num;
  i1 = I1 + 24*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 40*cd_num;
  i1 = I1 + 25*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 41*cd_num;
  i1 = I1 + 26*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 42*cd_num;
  i1 = I1 + 27*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 43*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 45*cd_num;
  i1 = I1 + 28*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 8.000000*(*(i1++));
  i0 = I0 + 46*cd_num;
  i1 = I1 + 29*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 7.000000*(*(i1++));
  i0 = I0 + 47*cd_num;
  i1 = I1 + 30*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 6.000000*(*(i1++));
  i0 = I0 + 48*cd_num;
  i1 = I1 + 31*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 49*cd_num;
  i1 = I1 + 32*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 50*cd_num;
  i1 = I1 + 33*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 51*cd_num;
  i1 = I1 + 34*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 52*cd_num;
  i1 = I1 + 35*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 53*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  i0 = I0 + 55*cd_num;
  i1 = I1 + 36*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 9.000000*(*(i1++));
  i0 = I0 + 56*cd_num;
  i1 = I1 + 37*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 8.000000*(*(i1++));
  i0 = I0 + 57*cd_num;
  i1 = I1 + 38*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 7.000000*(*(i1++));
  i0 = I0 + 58*cd_num;
  i1 = I1 + 39*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 6.000000*(*(i1++));
  i0 = I0 + 59*cd_num;
  i1 = I1 + 40*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 5.000000*(*(i1++));
  i0 = I0 + 60*cd_num;
  i1 = I1 + 41*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 4.000000*(*(i1++));
  i0 = I0 + 61*cd_num;
  i1 = I1 + 42*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 3.000000*(*(i1++));
  i0 = I0 + 62*cd_num;
  i1 = I1 + 43*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 2.000000*(*(i1++));
  i0 = I0 + 63*cd_num;
  i1 = I1 + 44*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) - 1.000000*(*(i1++));
  i0 = I0 + 64*cd_num;
  for(cd=0;cd<cd_num;cd++)
    *(vp++) = twotzeta*(*(i0++)) ;
  I0 += 66*cd_num;  I1 += 45*cd_num;
  }
}
