
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "uthash-1.9.6/src/uthash.h"

#include <gmp.h>

#define BUF 5000

mp_exp_t t = 4;
void prepend(char* s, const char* t)
{
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

  int main(int argc, char* argv[])
  {
    if (argc != 6) return 1;
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

    strncpy(di, argv[1], BUF-1);
    if (strlen(di) == BUF-1) abort();
    decimal_exponent = atoi(argv[2]);
    strncpy(decimal_fraction, argv[3], BUF-1);
    if (strlen(decimal_fraction) == BUF-1) abort();
    strncpy(decimal_recurrence, argv[4], BUF-1);
    if (strlen(decimal_recurrence) == BUF-1) abort();
    decimal_recurrence_start = atoi(argv[5]);
//printf("drs %d\n", decimal_recurrence_start);

    mpz_t result;
    mpz_init(result);


    //adjust the working copy so that the exponent is zero
    while (decimal_exponent > 0)
    {
      if (strlen(decimal_fraction) < 1)
      {
        if (decimal_recurrence_start > -1)
        {
          strcpy(decimal_fraction, decimal_recurrence);
        }
        else
        {
          strcpy(decimal_fraction, "0");
        }
      }
      di[strlen(di)+1] = '\0';
      di[strlen(di)] = decimal_fraction[0];
      //decimal_fraction = decimal_fraction.substr(1);
      memmove(decimal_fraction, decimal_fraction+1, sizeof(decimal_fraction) - sizeof(*decimal_fraction));

      decimal_exponent--;
    }

    while (decimal_exponent < 0)
    {
      if (strlen(di) < 1) { strcpy(di, "0");}
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

    while ((mpz_cmp_d(decimal_integer, 0) > 0))
    {
      mpz_t temp_dec;
      mpz_init(temp_dec);
      mpz_set(temp_dec, decimal_integer);

      mpz_tdiv_q_ui(result, decimal_integer, 2);
      //printf("tdiv %s\n", mpz_get_str(NULL, 10, result));
      mpz_set(decimal_integer, result);

      mpz_mod_ui(result, temp_dec, 2);
      //printf("mod %s\n", mpz_get_str(NULL, 10, result));

      if(mpz_cmp_d(result, 1)==0){//there is a remainder
        prepend(binary_integer,"1");
      }
      else{//there is not a remainder
        prepend(binary_integer,"0");
      }
      precision_so_far++;
      //mpz_clear(temp_dec);
    }

    if (strlen(binary_integer)==0) { strcpy(binary_integer , "0"); }
    int binary_recurrence_start = -1;

    //  If there is a decimal recurrence, be sure there is one complete copy available
    if ((atoi((decimal_recurrence)) > -1) &&
        (strlen(decimal_fraction) < strlen(decimal_recurrence)))
    {
      strcat(decimal_fraction , decimal_recurrence);
    }

    if (strlen(decimal_fraction)==0) { strcpy(decimal_fraction , "0"); }
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

    for (int i = 0; i <=decimal_recurrence_start; i++)
    {
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
    for (int i = 0; i < lofr; i++)
    {
      mpf_div_ui(d, d, 10);
    }
    mpf_set(poft, d);
    // the variable drs tells us this, 0 is right next to the decimal, 1 is 1
    // away from this decimal and so on
    // need to locate the decimal recurrence string
    int drs = decimal_recurrence_start;
    // now we need to further move the decimal point to account for the distance
    // the recurrence starts from the decimal point
    for (int i =0 ; i < drs; i++)
    {
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
    for(int i = 0; i < drs; i++)
    {
      mpf_div_ui(e, e, 10);
    }
    mpf_t eCompare;
    mpf_init(eCompare);
    mpf_set(eCompare, e);

    if (decimal_recurrence_start> -1)
    {
      //so do this only for recurring numbers
      if (decimal_recurrence_start == 0)
      {
 //printf("df 11 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


        if (mpf_cmp(df, eCompare) >= 0)
        {
          mpf_add(df, df, poft);
        }
        else
        {
        }
 //printf("df 12 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


      }
      else
      {
        mpf_set(superR, df);
        mpf_mul(superR, superR, powToMul);
        superRL = mpf_get_ui(superR); // this should do away with the decimal
                                      // stuff leaving just the numbers to the
                                      // left of the decimal point
        superRL = superRL + 1;
        superRL = superRL % 10;
        rArr[rac] = superRL;
        if (rArr[rac] < rArr[rac-1])
        {
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
    while ((j<17000) && (mpf_cmp_ui(df, 0) > 0) && (binary_recurrence_start == -1))
    {
      mpf_set(dr_temp, df);
      mpf_set(temp_df, df);

 //printf("df 9 %s\n", mpf_get_str(NULL, &t, 10, 120, df));
      if (mpf_cmp_ui(temp_df, 1) >= 0)
      {
        mpf_sub_ui(temp_df, temp_df, 1);
     //printf("temp_df %s\n", mpf_get_str(NULL, &t, 10, 120, temp_df));

      }

      for (int i = 0; i < a; i++)
      {
	mpf_t* tmp = lazyget(i);
        if (mpf_eq(temp_df, *tmp, 112) != 0)
        {
          // Binary recurrence found
          binary_recurrence_start =i;
          strcpy(binary_recurrence, binary_fraction+binary_recurrence_start);
          break;
        }
      }

      if (mpf_cmp_ui(df, 1) >= 0)
      {
	strcpy(binary_fraction+strlen(binary_fraction), "1");
        mpf_sub_ui(df, df, 1);
     //printf("df 1 %s\n", mpf_get_str(NULL, &t, 10, 120, df));

	{
		mpf_t* tmp = lazyget(a);
        	mpf_set(*tmp, df);
	}
        a++;
      }
      else
      {
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


      if (decimal_recurrence_start> -1)
      {
        // do this only for recurring numbers
        float a = mpf_get_d(df);
        float b = mpf_get_d(eCompare);
        if (decimal_recurrence_start == 0)
        {
          if (mpf_cmp(df, eCompare) >= 0)
          {
            mpf_add(df, df, poft);
 //printf("df 4 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


          }
          else if (a == b)
          {
            mpf_add(df, df, poft);
 //printf("df 5 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


          }
          else
          {
          }
        }
        else
        {
          mpf_set(superR, df);
          mpf_mul(superR, superR, powToMul);
          // do away with the decimal stuff leaving just the numbers to the left
          // of the decimal point
          superRL = mpf_get_ui(superR);
          superRL = superRL + 1;
          superRL = superRL%10;
          rArr[rac] = superRL;
          if (rArr[rac] < rArr[rac-1])
          {
            rArr[rac] = rArr[rac] + 1; //add one to the value in the array
            mpf_add(df, df, poft);
 //printf("df 7 %s\n", mpf_get_str(NULL, &t, 10, 120, df));


          }
          rac++;
        }
      }

      //printf("df %s\n", mpf_get_str(NULL, &t, 10, 120, df));
      if (mpf_cmp_ui(df, 0)==0)
      {
	//printf("df = 0 break\n");
        break;
      }
      j++;
      if(binary_recurrence_start!=-1){
         strcat(binary_recurrence, binary_fraction+strlen(binary_fraction)-1);
      }
      if(strlen(binary_fraction) >= 120){
		if(j%120 == 0 && firstOne==NULL){
			firstOne = strchr(binary_fraction,'1');
			if(firstOne != NULL){ 
				limit = firstOne-binary_fraction + 120;
//printf("limit %d\n", limit);
				limitOK=true;
			}
		}
	  }
//printf("bf %s\n", binary_fraction);
     if(j == limit && limitOK ==true){
//printf("limit break\n");
		break;
	 }
    } //end of while
    mpz_clear(intSum);
    mpf_clear(df);

    // Normalize binary value
    while (strlen(binary_integer)>1)
    {
      strcpy(temp, binary_integer+strlen(binary_integer)-1);
      strcat(temp,binary_fraction);
      strcpy(binary_fraction, temp);
      binary_integer[strlen(binary_integer) - 1] = '\0';
      binary_exponent++;
      if (binary_recurrence_start > -1)
      {
        binary_recurrence_start++;
      }
    }

    while (strcmp(binary_integer, "0")==0)
    {
      binary_integer[0] = binary_fraction[0];
      binary_integer[1] = '\0';
      memmove(binary_fraction, binary_fraction+1, sizeof(binary_fraction) - sizeof(*binary_fraction));
      binary_exponent--;
      if (binary_recurrence_start > -1)
      {
        binary_recurrence_start--;
        if (strlen(binary_fraction) < strlen(binary_recurrence))
        {
          binary_recurrence_start = strlen(binary_fraction);
          strcat(binary_fraction,binary_recurrence);
        }
      }
    }


    if (strlen(binary_fraction)==0)
    {
      strcpy(binary_fraction, "0");
    }

    printf("%s\n", binary_integer);
    printf("%s\n", binary_fraction);
    printf("%d\n", binary_exponent);
    printf("%s\n", decimal_recurrence);
    printf("%d\n", decimal_recurrence_start);
    printf("%s\n", binary_recurrence);
    printf("%d\n", binary_recurrence_start);

    free_dfs();
    mpf_clear(temp_df);
    mpf_clear(dr_temp);
    mpf_clear(superR);
    mpf_clear(powToMul);
    mpf_clear(poft);
    mpf_clear(d);
    mpf_clear(e);
    mpf_clear(eCompare);
    return 0;
  }



