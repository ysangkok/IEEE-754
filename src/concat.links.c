
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "uthash-1.9.6/src/uthash.h"

#include <gmp.h>

#define BUF 5000

char* repeat_char(const char c,const size_t n) {
    static char buf[BUF+1];
    size_t tn=n;
    if(tn>BUF)
        tn=BUF;
    memset(buf,c,tn);
    buf[tn]=0;
    return buf;
}

void substr1(char* x)  {
    if (x[0] != '\0') memmove(x, x+1, strlen(x)+1);
}

void prepend(char* s, const char* t) {
    size_t len = strlen(t);

    memmove(s + len, s, strlen(s)+1);

    memcpy(s,t,len);
}

struct my_struct {
    int id;
    mpf_t* second;
    UT_hash_handle hh;
};

//map<int,mpf_t*> dfs_backing;
struct my_struct *users = NULL;

void free_dfs() {
    struct my_struct *current_user, *tmp;

    HASH_ITER(hh, users, current_user, tmp) {
        HASH_DEL(users,current_user);
        mpf_clear(current_user->second[0]);
        free(current_user->second);
        free(current_user);
    }
}

mpf_t* lazyget(int key) {
    struct my_struct *s;

    HASH_FIND_INT( users, &key, s );

    mpf_t* valptr;
    if (s==NULL) {
        //printf("making new for %d\n", key);
        valptr = (mpf_t*) malloc(sizeof(mpf_t));
        mpf_init(*valptr);

        s = malloc(sizeof(struct my_struct));
        s->id = key;
        s->second = valptr;

        HASH_ADD_INT( users, id, s );
    } else {
        //printf("found old for %d\n", key);
        valptr = s->second;
    }
    return valptr;
}

char* dtb2(char* argv1, char* argv2, char* argv3, char* argv4, char* argv5) {
    char    di[BUF];
    int     decimal_exponent;
    char    decimal_recurrence[BUF];
    char    decimal_fraction[BUF];
    int     decimal_recurrence_start;
    char    binary_integer[BUF] = "";
    char    binary_fraction[BUF] = "";
    char    binary_recurrence[BUF] = "0";
    char    temp[BUF];
    int     precision_so_far = 0;
    int     binary_exponent = 0;

    strncpy(di, argv1, BUF-1);
    if (strlen(di) == BUF-1) return 0;
    decimal_exponent = atoi(argv2);
    strncpy(decimal_fraction, argv3, BUF-1);
    if (strlen(decimal_fraction) == BUF-1) return 0;
    strncpy(decimal_recurrence, argv4, BUF-1);
    if (strlen(decimal_recurrence) == BUF-1) return 0;
    decimal_recurrence_start = atoi(argv5);
//printf("drs %d\n", decimal_recurrence_start);

    mpz_t result;
    mpz_init(result);


    //adjust the working copy so that the exponent is zero
    while (decimal_exponent > 0) {
        if (strlen(decimal_fraction) < 1) {
            if (decimal_recurrence_start > -1) {
                strcpy(decimal_fraction, decimal_recurrence);
            } else {
                strcpy(decimal_fraction, "0");
            }
        }
        di[strlen(di)+1] = '\0';
        di[strlen(di)] = decimal_fraction[0];
        //decimal_fraction = decimal_fraction.substr(1);
        substr1(decimal_fraction);

        decimal_exponent--;
    }

    while (decimal_exponent < 0) {
        if (strlen(di) < 1) {
            strcpy(di, "0");
        }
        strcpy(temp, di+strlen(di)-1);
        //decimal_fraction = temp.append(decimal_fraction);
        strcat(temp, decimal_fraction);
        strcpy(decimal_fraction, temp);
        di[strlen(di)-1] = '\0';
        decimal_exponent++;
    }

    mpz_t decimal_integer;
    mpz_init(decimal_integer);
    //create local copy
    mpz_set_str(decimal_integer, di, 10);

    while ((mpz_cmp_d(decimal_integer, 0) > 0)) {
        mpz_t temp_dec;
        mpz_init(temp_dec);
        mpz_set(temp_dec, decimal_integer);

        mpz_tdiv_q_ui(result, decimal_integer, 2);
        //printf("tdiv %s\n", mpz_get_str(NULL, 10, result));
        mpz_set(decimal_integer, result);

        mpz_mod_ui(result, temp_dec, 2);
        //printf("mod %s\n", mpz_get_str(NULL, 10, result));

        if(mpz_cmp_d(result, 1)==0) { //there is a remainder
            prepend(binary_integer,"1");
        } else { //there is not a remainder
            prepend(binary_integer,"0");
        }
        precision_so_far++;
        //mpz_clear(temp_dec);
    }

    if (strlen(binary_integer)==0) {
        strcpy(binary_integer , "0");
    }
    int binary_recurrence_start = -1;

    //  If there is a decimal recurrence, be sure there is one complete copy available
    if ((atoi((decimal_recurrence)) > -1) &&
            (strlen(decimal_fraction) < strlen(decimal_recurrence))) {
        strcat(decimal_fraction , decimal_recurrence);
    }

    if (strlen(decimal_fraction)==0) {
        strcpy(decimal_fraction , "0");
    }
//printf("df %s\n", decimal_fraction);


    mpz_t intSum;
    mpz_init(intSum);

    mpf_set_default_prec(20000);
    mpf_t df;
    mpf_init(df);
//printf("str df %s\n", decimal_fraction);
    strcpy(temp, decimal_fraction);
    prepend(temp, "0.");
//printf("str df %s\n", temp);


    mpf_set_str(df, temp, 10);
//printf("df 18 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


    mpf_t temp_df;
    mpf_init(temp_df);

    mpf_t dr_temp;
    mpf_init(dr_temp);

    // adding another mpf here for recurrences that don't start at 0
    mpf_t superR; // stands for superRecurrence
    mpf_init(superR);
    mpf_set(superR, df);

    mpf_t powToMul; //  the power to multiply superR by to get decimal point to
    //  the right of the recurrence spot
    mpf_init(powToMul);
    mpf_set_ui(powToMul, 1);

    for (int i = 0; i <=decimal_recurrence_start; i++) {
        mpf_mul_ui(powToMul, powToMul, 10);
    }

    mpf_mul(superR, superR, powToMul);

    long superRL; // stands for superRLong
    superRL = mpf_get_ui(superR); //  do away with the decimal stuff, leaving
    //  just the numbers to the left of the
    //  decimal point
    superRL = superRL + 1;
    int rac = 0;  //  recurrence array count
    int rArr[1000];
    superRL = superRL%10;
    rArr[rac] = superRL;
    rac++;

    /*
        mpf_t dfs [50000];
        for (int i =0; i<50000; i++)
        {
          mpf_init(dfs[i]);
        }
    */

    int a = 0;
    {
        mpf_t* tmp = lazyget(a);
        mpf_set(*tmp, df);
    }
    a++;
//printf("df 16 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


    mpf_add(df, df, df);
//printf("df 15 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


    mpf_t poft; //  this is the power of ten variable, only used when there are
    //  decimal recurrences
    mpf_init(poft);
    int lofr = 0; // the length of the decimal recurrence
    lofr = strlen(decimal_recurrence);
    mpf_t d;
    mpf_init(d);
    mpf_set_ui(d, 1);
    for (int i = 0; i < lofr; i++) {
        mpf_div_ui(d, d, 10);
    }
    mpf_set(poft, d);
    // the variable drs tells us this, 0 is right next to the decimal, 1 is 1
    // away from this decimal and so on
    // need to locate the decimal recurrence string
    int drs = decimal_recurrence_start;
    // now we need to further move the decimal point to account for the distance
    // the recurrence starts from the decimal point
    for (int i =0 ; i < drs; i++) {
        // divide the poft by 10 for every spot the recurrence is away from the
        // decimal point example if drs is 1 you do poft/10 one time so 0.1 would
        // become 0.01
        mpf_div_ui(poft, poft, 10);
    }

    // now we need the power of ten that we will be comparing the "sum" to, to
    // decide whether to add anything to the "sum"
    // this is based on the drs, if drs = 0 then we compare to 1.0, if drs = 1
    // we compare to .1
    mpf_t e;
    mpf_init(e);
    mpf_set_ui(e, 1);
    for(int i = 0; i < drs; i++) {
        mpf_div_ui(e, e, 10);
    }
    mpf_t eCompare;
    mpf_init(eCompare);
    mpf_set(eCompare, e);

    if (decimal_recurrence_start> -1) {
        //so do this only for recurring numbers
        if (decimal_recurrence_start == 0) {
//printf("df 11 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


            if (mpf_cmp(df, eCompare) >= 0) {
                mpf_add(df, df, poft);
            } else {
            }
//printf("df 12 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


        } else {
            mpf_set(superR, df);
            mpf_mul(superR, superR, powToMul);
            superRL = mpf_get_ui(superR); // this should do away with the decimal
            // stuff leaving just the numbers to the
            // left of the decimal point
            superRL = superRL + 1;
            superRL = superRL % 10;
            rArr[rac] = superRL;
            if (rArr[rac] < rArr[rac-1]) {
                rArr[rac] = rArr[rac] + 1; // add one to the value in the array
                mpf_add(df, df, poft);
//printf("df 8 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


            }
            rac++;
        }
    }
//printf("df 10 %s\n", mpf_get_str(NULL, &t, 10, 120, df));

    int j = 0;
    int limit = 0;
    char* firstOne = NULL;
    bool limitOK = false;
    while ((j<17000) && (mpf_cmp_ui(df, 0) > 0) && (binary_recurrence_start == -1)) {
        mpf_set(dr_temp, df);
        mpf_set(temp_df, df);

//printf("df 9 %s\n", mpf_get_str(NULL, &t, 10, 120, df));
        if (mpf_cmp_ui(temp_df, 1) >= 0) {
            mpf_sub_ui(temp_df, temp_df, 1);
            //printf("temp_df %s\n", mpf_get_str(NULL, &t, 10, 120, temp_df));

        }

        for (int i = 0; i < a; i++) {
            mpf_t* tmp = lazyget(i);
            if (mpf_eq(temp_df, *tmp, 112) != 0) {
                // Binary recurrence found
                binary_recurrence_start =i;
                strcpy(binary_recurrence, binary_fraction+binary_recurrence_start);
                break;
            }
        }

        if (mpf_cmp_ui(df, 1) >= 0) {
            strcpy(binary_fraction+strlen(binary_fraction), "1");
            mpf_sub_ui(df, df, 1);
            //printf("df 1 %s\n", mpf_get_str(NULL, &t, 10, 120, df));

            {
                mpf_t* tmp = lazyget(a);
                mpf_set(*tmp, df);
            }
            a++;
        } else {
            strcpy(binary_fraction+strlen(binary_fraction), "0");
            {
                mpf_t* tmp = lazyget(a);
                mpf_set(*tmp, df);
            }
            a++;
        }
//printf("df 2 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


        mpf_add(df, df, df);
//printf("df 3 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


        if (decimal_recurrence_start> -1) {
            // do this only for recurring numbers
            float a = mpf_get_d(df);
            float b = mpf_get_d(eCompare);
            if (decimal_recurrence_start == 0) {
                if (mpf_cmp(df, eCompare) >= 0) {
                    mpf_add(df, df, poft);
//printf("df 4 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


                } else if (a == b) {
                    mpf_add(df, df, poft);
//printf("df 5 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


                } else {
                }
            } else {
                mpf_set(superR, df);
                mpf_mul(superR, superR, powToMul);
                // do away with the decimal stuff leaving just the numbers to the left
                // of the decimal point
                superRL = mpf_get_ui(superR);
                superRL = superRL + 1;
                superRL = superRL%10;
                rArr[rac] = superRL;
                if (rArr[rac] < rArr[rac-1]) {
                    rArr[rac] = rArr[rac] + 1; //add one to the value in the array
                    mpf_add(df, df, poft);
//printf("df 7 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


                }
                rac++;
            }
        }

        //printf("df %s\n", mpf_get_str(NULL, &t, 10, 120, df));
        if (mpf_cmp_ui(df, 0)==0) {
            //printf("df = 0 break\n");
            break;
        }
        j++;
        if(binary_recurrence_start!=-1) {
            strcat(binary_recurrence, binary_fraction+strlen(binary_fraction)-1);
        }
        if(strlen(binary_fraction) >= 120) {
            if(j%120 == 0 && firstOne==NULL) {
                firstOne = strchr(binary_fraction,'1');
                if(firstOne != NULL) {
                    limit = firstOne-binary_fraction + 120;
//printf("limit %d\n", limit);
                    limitOK=true;
                }
            }
        }
//printf("bf %s\n", binary_fraction);
        if(j == limit && limitOK ==true) {
//printf("limit break\n");
            break;
        }
    } //end of while
    mpz_clear(intSum);
    mpf_clear(df);

    // Normalize binary value
    while (strlen(binary_integer)>1) {
        strcpy(temp, binary_integer+strlen(binary_integer)-1);
        strcat(temp,binary_fraction);
        strcpy(binary_fraction, temp);
        binary_integer[strlen(binary_integer) - 1] = '\0';
        binary_exponent++;
        if (binary_recurrence_start > -1) {
            binary_recurrence_start++;
        }
    }

    while (strcmp(binary_integer, "0")==0) {
        binary_integer[0] = binary_fraction[0];
        binary_integer[1] = '\0';
        substr1(binary_fraction);
        binary_exponent--;
        if (binary_recurrence_start > -1) {
            binary_recurrence_start--;
            if (strlen(binary_fraction) < strlen(binary_recurrence)) {
                binary_recurrence_start = strlen(binary_fraction);
                strcat(binary_fraction,binary_recurrence);
            }
        }
    }


    if (strlen(binary_fraction)==0) {
        strcpy(binary_fraction, "0");
    }

    char* resstr = malloc(BUF);

    sprintf(resstr,
    "%s\n"
    "%s\n"
    "%d\n"
    "%s\n"
    "%d\n"
    "%s\n"
    "%d", 
    binary_integer,
    binary_fraction,
    binary_exponent,
    decimal_recurrence,
    decimal_recurrence_start,
    binary_recurrence,
    binary_recurrence_start
    );

    free_dfs();
    mpf_clear(temp_df);
    mpf_clear(dr_temp);
    mpf_clear(superR);
    mpf_clear(powToMul);
    mpf_clear(poft);
    mpf_clear(d);
    mpf_clear(e);
    mpf_clear(eCompare);
    return resstr;
}

#undef BUF
#define BUF 17000
//#define BUF 5000


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
char* btd2(char* argv1, char* argv2, char* argv3) {
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

    strcpy(binary_integer_v, argv1);
    binary_exponent_v = atoi(argv2);
    strcpy(binary_fraction_v, argv3);
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

    while(atoi(binary_integer_n) > 1) {
        temp[0] = binary_integer_n[strlen(binary_integer_n)-1];
        temp[1] = '\0';
//      strcpy(temp,&(binary_integer_n[strlen(binary_integer_n)-1]));
        strcpy(binary_fraction_n, strcat(temp, binary_fraction_n));
        //strcpy(binary_integer_n, substr(binary_integer_n,0, strlen(binary_integer_n) - 1));
        binary_integer_n[strlen(binary_integer_n)-1] = '\0';
        binary_exponent_n++;
    }

    while (strcmp(binary_integer_n,"0") == 0) {
        binary_integer_n[0] = binary_fraction_n[0];
        binary_integer_n[1] = '\0';
//	strcpy(binary_integer_n, binary_fraction_n);
//      strcpy(binary_fraction_n,  substr(binary_fraction_n,1,strlen(binary_fraction_n)-1));
        substr1(binary_fraction_n);
        binary_exponent_n--;
    }

    if (binary_fraction_n[0] == '\0') {
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
    while (binary_exponent > 0) {
        int a = strlen(binary_integer);

        binary_integer[a] = binary_fraction[0];
        binary_integer[a+1] = '\0';
        //strcpy(binary_fraction, substr(binary_fraction,1,strlen(binary_fraction-1)));
        substr1(binary_fraction);
        if (binary_fraction[0] == '\0') {
            strcpy(binary_fraction, "0");
        }
        binary_exponent--;
    }
    ////printf("space needed: %d\n", (int) binary_fraction - (int) binary_fraction_backing);


    //printf("bi %s\n",binary_integer);

    while (binary_exponent < 0) {
        //strcpy(temp, &(binary_integer[strlen(binary_integer) - 1]));
        temp[0] = binary_integer[strlen(binary_integer) - 1];
        temp[1] = '\0';
        strcpy(binary_fraction, strcat(temp, binary_fraction));
        //strcpy(binary_integer, substr(binary_integer,0, strlen(binary_integer) - 1));
        binary_integer[strlen(binary_integer)-1] = '\0';
        if (binary_integer[0] == '\0') {
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
    //int aa = 0, b = 0;
    for (int i = strlen(binary_integer) - 1; i > -1; i--) {
        //  if it's a 1 you add the decimal integer to the power of two (power of
        //  two starts at 1)
        if (binary_integer[i] == '1') {
            mpz_add(di, di, power_of_two);
            //printf("di %s\n", mpz_get_str(NULL,10,power_of_two));
            //aa++;
        }
        mpz_add(power_of_two, power_of_two, power_of_two); //double the power_of_two
        //printf("power_of_two b=%d %s\n", b, mpz_get_str(NULL,10,power_of_two));
        //b++;
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
    for (int i = strlen(binary_fraction) - 1; i > -1; i--) {
        //  if it is 1 add the decimal fraction to the power of two
        if (binary_fraction[i] == '1') {
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


    while (strlen(decimal_integer)>1) {
        temp[0] = decimal_integer[strlen(decimal_integer)-1];
        temp[1] = '\0';
        //strcpy(temp, &(decimal_integer[strlen(decimal_integer)-1]));
        strcpy(decimal_fraction, strcat(temp, decimal_fraction));
        //strcpy(decimal_integer, substr(decimal_integer,0, strlen(decimal_integer) - 1));
        decimal_integer[strlen(decimal_integer)-1] = '\0';
        decimal_exponent++;
    }

    while (strcmp(decimal_integer, "0") == 0) {
        decimal_integer[0] = decimal_fraction[0];
        decimal_integer[1] = '\0';
        //strcpy(decimal_integer, decimal_fraction);
        //strcpy(decimal_fraction, substr(decimal_fraction,1,strlen(decimal_fraction)-1));
        substr1(decimal_fraction);
        decimal_exponent--;
    }

    if (decimal_integer[0] == '\0') {
        strcpy(decimal_integer, "0");
    }

    if (decimal_fraction[0] == '\0') {
        strcpy(decimal_fraction, "0");
    }

    char* tempDI = malloc(BUF);
    strcpy( tempDI, decimal_integer);
    while (tempDI[0] == '0') {
        //strcpy(tempDI, substr(tempDI,1,strlen(tempDI)-1));
        substr1(tempDI);
    }
    if (tempDI[0] == '\0') strcpy(tempDI, "0");

    strcpy(decimal_integer, tempDI);

    char* resstr = malloc(BUF);
sprintf(
resstr,
"%s\n"
"%s\n"
"%d\n"
"%s\n"
"%d\n"
"%s\n"
"%d\n"

"%s\n"
"%s\n"
"%d", 

decimal_integer,
decimal_fraction,
decimal_exponent,
decimal_recurrence,
decimal_recurrence_start,
binary_recurrence,
binary_recurrence_start,
    // These bottom three are the normalized binary values that also have to be
    // returned to the Javascript
binary_integer_n,
binary_fraction_n,
binary_exponent_n
);


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

    return resstr;
}
