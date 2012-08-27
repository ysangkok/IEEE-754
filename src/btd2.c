//  btd: converts binary number to a decimal number
//  Michael Lubow
//  November 2011

#include "gmp.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF 17000
//#define BUF 5000

void substr1(char* x)  {
if (x[0] != '\0') memmove(x, x+1, strlen(x)+1);
}

char* repeat_char(const char c,const size_t n)
{
   static char buf[BUF+1];
   size_t tn=n;
   if(tn>BUF)
      tn=BUF;
   memset(buf,c,tn);
   buf[tn]=0;
   return buf;
}

void prepend(char* s, const char* t)
{
    size_t len = strlen(t);

    memmove(s + len, s, strlen(s)+1);

    memcpy(s,t,len);
}


//  main()
//  -------------------------------------------------------------------
/*  Command line arguments are three char[]s of digits:
 *    integer   (binary digits)
 *    exponent  (decimal value)
 *    fraction  (binary digits)
 *  Outputs are normalized decimal and binary representations
 *  of the input value, one part per line:
 *    decimal integer
 *    decimal fraction
 *    decimal exponent
 *    decimal recurrence digits
 *    decimal recurrence start position
 *    binary recurrence digits
 *    binary recurrence start
 *    binary integer
 *    binary fraction
 *    binary exponent
*/
  int main(int argc, char* argv[])
  {
	if (argc != 4) {
		puts("Need arguments!");
		exit(1);
	}

    //const int BINARY_PRECISION = 128;
    int       decimal_exponent;
    int       binary_exponent_v;
    char*    binary_integer_v = malloc(BUF);
    char*    binary_fraction_v = malloc(BUF);
    char*    decimal_fraction = malloc(BUF);
    char*    decimal_integer = malloc(BUF);
    int       decimal_recurrence_start = -1;
    int       binary_recurrence_start = -1;
    char*    decimal_recurrence = malloc(BUF);
    strcpy(decimal_recurrence, "0");
    char*    binary_recurrence = malloc(BUF);
    strcpy(binary_recurrence, "0");
    char*    temp = malloc(BUF);
    //int       precision_so_far = 0;

    strcpy(binary_integer_v, argv[1]);
    binary_exponent_v = atoi(argv[2]);
    strcpy(binary_fraction_v, argv[3]);
//printf("bfv %s\n", binary_fraction_v);

    mpz_t result;
    mpz_init(result);

    //char* binfractemp = binary_fraction_v;

    // The n versions of the variable are the ones that are normalized and used
    // for the binary ouput but are not used for generating the decimal values
    char* binary_integer_n = malloc(BUF);
    strcpy(binary_integer_n, binary_integer_v);
    char* binary_fraction_n = malloc(BUF);
    strcpy(binary_fraction_n, binary_fraction_v);
    int binary_exponent_n = binary_exponent_v;

    while(atoi(binary_integer_n) > 1)
    {
	temp[0] = binary_integer_n[strlen(binary_integer_n)-1];
	temp[1] = '\0';
//      strcpy(temp,&(binary_integer_n[strlen(binary_integer_n)-1]));
      strcpy(binary_fraction_n, strcat(temp, binary_fraction_n));
      //strcpy(binary_integer_n, substr(binary_integer_n,0, strlen(binary_integer_n) - 1));
      binary_integer_n[strlen(binary_integer_n)-1] = '\0';
      binary_exponent_n++;
    }

    while (strcmp(binary_integer_n,"0") == 0)
    {
      binary_integer_n[0] = binary_fraction_n[0];
      binary_integer_n[1] = '\0';
//	strcpy(binary_integer_n, binary_fraction_n);
//      strcpy(binary_fraction_n,  substr(binary_fraction_n,1,strlen(binary_fraction_n)-1));
	substr1(binary_fraction_n);
      binary_exponent_n--;
    }

    if (binary_fraction_n[0] == '\0')
    {
      strcpy(binary_fraction_n, "0");
    }
      //printf("bi %s\n",binary_integer_v);

    //these are copies of the originals
    char* binary_integer = malloc(BUF);
    strcpy(binary_integer, binary_integer_v);
    char* binary_fraction = malloc(BUF);
    strcpy(binary_fraction, binary_fraction_v);
    int binary_exponent     = binary_exponent_v;

      //printf("bi %s\n",binary_integer);
    while (binary_exponent > 0)
    {
      int a = strlen(binary_integer);

      binary_integer[a] = binary_fraction[0];
      binary_integer[a+1] = '\0';
      //strcpy(binary_fraction, substr(binary_fraction,1,strlen(binary_fraction-1)));
      substr1(binary_fraction);
      if (binary_fraction[0] == '\0')
      {
        strcpy(binary_fraction, "0");
      }
        binary_exponent--;
    }
    ////printf("space needed: %d\n", (int) binary_fraction - (int) binary_fraction_backing);


      //printf("bi %s\n",binary_integer);

    while (binary_exponent < 0)
    {
      //strcpy(temp, &(binary_integer[strlen(binary_integer) - 1]));
      temp[0] = binary_integer[strlen(binary_integer) - 1];
      temp[1] = '\0';
      strcpy(binary_fraction, strcat(temp, binary_fraction));
      //strcpy(binary_integer, substr(binary_integer,0, strlen(binary_integer) - 1));
      binary_integer[strlen(binary_integer)-1] = '\0';
      if (binary_integer[0] == '\0')
      {
        strcpy(binary_integer, "0");
      }
      binary_exponent++;
    }

    //  Decimal integer part
    strcpy(decimal_integer, "0");

    mpz_t power_of_two;
    mpz_init(power_of_two);
    mpz_set_ui(power_of_two, 1);

    mpz_t di;
    mpz_init(di);
    mpz_set_ui(di, 0);

    //printf("bin int len: %s\n", binary_integer);
    int aa = 0, b = 0;
    for (int i = strlen(binary_integer) - 1; i > -1; i--)
    {
      //  if it's a 1 you add the decimal integer to the power of two (power of
      //  two starts at 1)
      if (binary_integer[i] == '1')
      {
        mpz_add(di, di, power_of_two);
        //printf("di %s\n", mpz_get_str(NULL,10,power_of_two));
	aa++;
      }
      mpz_add(power_of_two, power_of_two, power_of_two); //double the power_of_two
      //printf("power_of_two b=%d %s\n", b, mpz_get_str(NULL,10,power_of_two));
      b++;
    }
    //printf("aa = %d\nb = %d\n",aa,b);

    //  Decimal fraction part
    //  reset the power_of_two back to 1
    mpz_set_ui(power_of_two, 1);

    mpz_t df;
    mpz_init(df);
    mpz_set_ui(df, 0);

    strcpy(decimal_fraction, "0");

    //printf("binary_fraction pre-add: %s\n", binary_fraction);
    int j = 0, k = 0;
    for (int i = strlen(binary_fraction) - 1; i > -1; i--)
    {
      //  if it is 1 add the decimal fraction to the power of two
      if (binary_fraction[i] == '1')
      {
	j++;
        mpz_add(df, df, power_of_two);
      }
      mpz_add(power_of_two, power_of_two, power_of_two); //double the power_of_two
      k++;
    }
    //printf("j = %d\nk = %d\n",j,k);

    //printf("df %s\n", mpz_get_str(NULL, 10, df));
    //printf("powtwo %s\n", mpz_get_str(NULL, 10, power_of_two));

    mpf_set_default_prec(1000);
    mpf_t decf;
    mpf_init(decf);
    mpf_set_z(decf, df);
    //int n = 10;

    mpf_t powtwo;
    mpf_init(powtwo);
    mpf_set_z(powtwo, power_of_two);

    mpf_div(decf, decf, powtwo);


    //  Normalize
    decimal_exponent = 0;

    char* ditemp = malloc(BUF);
    char* dftemp = malloc(BUF);

    mpz_get_str(ditemp, 10, di);
    //printf("ditemp %s\n", ditemp);
    strcpy(decimal_integer, ditemp);

    mp_exp_t a;

    mpf_get_str(dftemp, &a, 10, 10000, decf);
    strcpy(decimal_fraction, dftemp);

/*
    while( a <0)
    {
      strcpy(decimal_fraction, strcat("0", decimal_fraction));
      a++;
    }
*/
    prepend(decimal_fraction, repeat_char('0', abs(a)));


    while (strlen(decimal_integer)>1)
    {
      temp[0] = decimal_integer[strlen(decimal_integer)-1];
      temp[1] = '\0';
      //strcpy(temp, &(decimal_integer[strlen(decimal_integer)-1]));
      strcpy(decimal_fraction, strcat(temp, decimal_fraction));
      //strcpy(decimal_integer, substr(decimal_integer,0, strlen(decimal_integer) - 1));
      decimal_integer[strlen(decimal_integer)-1] = '\0';
      decimal_exponent++;
    }

    while (strcmp(decimal_integer, "0") == 0)
    {
      decimal_integer[0] = decimal_fraction[0];
      decimal_integer[1] = '\0';
      //strcpy(decimal_integer, decimal_fraction);
      //strcpy(decimal_fraction, substr(decimal_fraction,1,strlen(decimal_fraction)-1));
      substr1(decimal_fraction);
      decimal_exponent--;
    }

    if (decimal_integer[0] == '\0')
    {
      strcpy(decimal_integer, "0");
    }

    if (decimal_fraction[0] == '\0')
    {
      strcpy(decimal_fraction, "0");
    }

    char* tempDI = malloc(BUF); 
    strcpy( tempDI, decimal_integer);
    while (tempDI[0] == '0')
    {
      //strcpy(tempDI, substr(tempDI,1,strlen(tempDI)-1));
      substr1(tempDI);
    }
    if (tempDI[0] == '\0') strcpy(tempDI, "0");

    strcpy(decimal_integer, tempDI);

    printf("%s\n", decimal_integer);
    printf("%s\n", decimal_fraction);
    printf("%d\n", decimal_exponent);
    printf("%s\n", decimal_recurrence);
    printf("%d\n", decimal_recurrence_start);
    printf("%s\n", binary_recurrence);
    printf("%d\n", binary_recurrence_start);
    // These bottom three are the normalized binary values that also have to be
    // returned to the Javascript
    printf("%s\n", binary_integer_n);
    printf("%s\n", binary_fraction_n);
    printf("%d\n", binary_exponent_n);

    mpf_clear(decf);
    mpf_clear(powtwo);
    mpz_clear(result);
    mpz_clear(di);
    mpz_clear(df);

    free(binary_integer_n);
    free(binary_fraction_n);
    free(binary_integer_v);
    free(binary_fraction_v);
    free(binary_integer);
    free(binary_fraction);
    free(binary_recurrence);
    free(decimal_recurrence);
    free(decimal_integer);
    free(decimal_fraction);
    free(tempDI);
    free(ditemp);
    free(dftemp);
    free(temp);

    return 0;
  }

