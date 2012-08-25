
#include "gmp.h"
#include <string.h>
#include <stdio.h>

mp_limb_t
my__gmpn_add (mp_ptr __gmp_wp, mp_srcptr __gmp_xp, mp_size_t __gmp_xsize, mp_srcptr __gmp_yp, mp_size_t __gmp_ysize)
{
  mp_limb_t __gmp_c;
  do { mp_size_t __gmp_i; mp_limb_t __gmp_x; __gmp_i = (__gmp_ysize); if (__gmp_i != 0) { if (__gmpn_add_n (__gmp_wp, __gmp_xp, __gmp_yp, __gmp_i)) { do { if (__gmp_i >= (__gmp_xsize)) { (__gmp_c) = 1; goto __gmp_done; } __gmp_x = (__gmp_xp)[__gmp_i]; } while ((((__gmp_wp)[__gmp_i++] = (__gmp_x + 1) & ((~ ((mp_limb_t) (0))) >> 0)) == 0)); } } if ((__gmp_wp) != (__gmp_xp)) do { mp_size_t __gmp_j; ; for (__gmp_j = (__gmp_i); __gmp_j < (__gmp_xsize); __gmp_j++) (__gmp_wp)[__gmp_j] = (__gmp_xp)[__gmp_j]; } while (0); (__gmp_c) = 0; __gmp_done: ; } while (0);
  return __gmp_c;
}

void
my__gmpz_add (mpz_ptr w, mpz_srcptr u, mpz_srcptr v)
{
  mp_srcptr up, vp;
  mp_ptr wp;
  mp_size_t usize, vsize, wsize;
  mp_size_t abs_usize;
  mp_size_t abs_vsize;

  usize = u->_mp_size;
  vsize = v->_mp_size;
  abs_usize = ((usize) >= 0 ? (usize) : -(usize));
  abs_vsize = ((vsize) >= 0 ? (vsize) : -(vsize));

  wsize = abs_usize + 1;
  if (w->_mp_alloc < wsize)
    __gmpz_realloc (w, wsize);


  up = u->_mp_d;
  vp = v->_mp_d;
  wp = w->_mp_d;

    {

      mp_limb_t cy_limb = my__gmpn_add (wp, up, abs_usize, vp, abs_vsize);
      wp[abs_usize] = cy_limb;
      wsize = abs_usize + cy_limb;
      if (usize < 0)
 wsize = -wsize;
    }

  w->_mp_size = wsize;
}

int main() {

    char two1087[] =       "1658079259093488585543641880321370579349968074867852233579735924960709341741017881738939463282172923864572541864483323178105313176664420162494573772314529873277070739673631632297712908223227628267436176822048727601659965304215082587079502689477915085543915982949243040172715332527968276743670394950828083309016741815037909270528";
    char two1087minus1[] = "1658079259093488585543641880321370579349968074867852233579735924960709341741017881738939463282172923864572541864483323178105313176664420162494573772314529873277070739673631632297712908223227628267436176822048727601659965304215082587079502689477915085543915982949243040172715332527968276743670394950828083309016741815037909270527";
    char two1088[] = "3316158518186977171087283760642741158699936149735704467159471849921418683482035763477878926564345847729145083728966646356210626353328840324989147544629059746554141479347263264595425816446455256534872353644097455203319930608430165174159005378955830171087831965898486080345430665055936553487340789901656166618033483630075818541056";
    char two1088minus1[] = "3316158518186977171087283760642741158699936149735704467159471849921418683482035763477878926564345847729145083728966646356210626353328840324989147544629059746554141479347263264595425816446455256534872353644097455203319930608430165174159005378955830171087831965898486080345430665055936553487340789901656166618033483630075818541055";
    char* resultstr;

    mpz_t power_of_two;
    mpz_init(power_of_two);
    mpz_set_str(power_of_two, two1087, 10);

    mpz_t power_of_two_m1;
    mpz_init(power_of_two_m1);
    mpz_set_str(power_of_two_m1, two1087minus1, 10);

    mpz_t result;
    mpz_init(result);

#if 0
    mpz_add(result,power_of_two,power_of_two_m1);
    resultstr = mpz_get_str(NULL, 10, result);
    printf("%s\n", resultstr);

    if (strcmp(resultstr, two1088minus1) != 0) {
	puts("ERROR! 2^1087+2^1087-1 calculated incorrect.");
    } else {
	puts("2^1087+2^1087-1 calculated correct.");
    }
#endif

    my__gmpz_add(result,power_of_two,power_of_two);
    resultstr = mpz_get_str(NULL, 10, result);
    printf("%s\n", resultstr);

    if (strcmp(resultstr, two1088) != 0) {
	puts("ERROR! 2^1087+2^1087 calculated incorrect.");
    } else {
	puts("2^1087+2^1087 calculated correct.");
    }
}

