#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <stdlib.h>
#include <stdint.h>
#include <immintrin.h>
#include <time.h>
#include <math.h>

#define instancias  2000
#define LOOP (1<<20)
#define  M PASSED_M
#define  b PASSED_b

const uint32_t m = 65536/M + (65536%M? 1: 0);
const uint32_t mask= ((1<<b)-1);
const uint32_t B=((1<<b)-M);
const uint32_t S= ((M<<1) - (1<<b));
const int offset = (2<< b); // NEW
const int offsetp = (2<< b-1); //NEW

double getP(double m){
    return 1-pow(2, -1.0/m);
}

int our_enc(uint32_t* value, size_t in_size,uint32_t* resArr) {
 uint64_t initialState = 0;
 int preLength = 0;

 uint32_t* ResArr = resArr;
 for (uint32_t* Val = value; Val < value + in_size; Val++) {
     int q = ((*Val+S) *m)>>16; 
     switch(q){
         case 0: initialState = (initialState << b) + *Val; break;
         case 1: initialState = ((initialState << b) + *Val<<1) + 1;break;
         case 2: initialState = ((initialState << b) + *Val - M<<2) + 1;break;
         default: initialState = ((initialState << b) + *Val - (q-1)*M<<q) + 1;
     }
     preLength += q+b;
     if (preLength>32) {
         preLength -= 32;
     	*ResArr++ = (uint32_t)(initialState>>preLength);
     	initialState &= (1<<preLength)-1;
     }
 }
     if(preLength) {
 	*ResArr++ = (uint32_t) initialState<<32-preLength;
 }
 return ResArr - resArr;
}

void our_dec(uint32_t* ptr, size_t in_size,uint32_t* res) {
    uint64_t initialstate = 0;
    int prelength = 0;
    uint32_t* Ptr = ptr;
    for(uint32_t* Res = res; Res < res + in_size; ++Res) {
        if (prelength < 32){
            initialstate += ((uint64_t)(*Ptr++))<<32-prelength;
            prelength += 32;
        }
        *Res = initialstate>>(64-b);
        initialstate <<= b;
        prelength -= b;
        if(*Res >= B){ 
            int q = _lzcnt_u64(initialstate);
            initialstate <<= q+1;
            prelength -= q+1;
            *Res += q*M;
        }
    }
   return;
}

int main (void)
{
  const gsl_rng_type * T;
  gsl_rng * r;

   int i, n = instancias;
   clock_t clockBegin;
   int ir = LOOP;
   double mu =getP(M+0.5);
   unsigned int value[instancias] = {0};
    uint32_t code[instancias]={0};
    unsigned int dec[instancias] = {0};

  gsl_rng_env_setup();
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);

  for (i = 0; i < n; i++)
    {
      unsigned int k = gsl_ran_geometric(r,mu);
      value[i]=k;
    }
    printf ("\n");
    printf("M= %d, P= %f\n", M,mu);
	
   //Ours
   ir = LOOP;
    clockBegin = clock();
    while(ir--){
    	our_enc(value, instancias,code);
    }
    double oe = (double)(clock() - clockBegin) / CLOCKS_PER_SEC;
    printf("Our encoding: %f MB/s\n", instancias*sizeof(unsigned int)*LOOP/(oe*1024*1024));
    ir = LOOP;
    clockBegin = clock();
    while(ir--){
    	our_dec(code,instancias,dec);
    }
    double od = (double)(clock() - clockBegin) / CLOCKS_PER_SEC;
    printf("Our decoding: %f MB/s\n", instancias*sizeof(unsigned int)*LOOP/(od*1024*1024));
    if(memcmp(value,dec,instancias*sizeof(unsigned int))==0)
    	printf("Decode Succeed!");
    else
    	printf("Decode Failed!");
    printf("\n");
	
  gsl_rng_free (r);
  return;
}

