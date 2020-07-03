#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <stdlib.h>
#include <stdint.h>
#include <immintrin.h>
#include <time.h>
#include <math.h>
#define instancias 2048
#define  M PASSED_M
#define  b PASSED_b

const uint32_t B=((1<<b)-M);
const uint32_t mask= ((1<<b)-1);
const uint32_t S= ((M<<1) - (1<<b));
const uint32_t m = 65536/M + (65536%M? 1: 0);
const int offset = (2<< b); // NEW
const int offsetp = (2<< b-1); //NEW

double getP(double m){
    return 1-pow(2, -1.0/m);
}
int golomb_enc(uint32_t* value, size_t in_size,uint32_t* resArr) {
	uint64_t initialState = 0;
	int preLength = 0;
	uint32_t* ResArr = resArr;
	int q; 
	int r; 
	int rr;
	int code; 
	int size; 
	uint32_t Add=0,Multi=0,Branch=0,Bitwise=0;
	for (uint32_t* Val = value; Val < value + in_size; ++Val) {
		q = (*Val*m)>>16; //NEW 
		r = *Val-q*M; //NEW
		
		Multi+=2;
		Bitwise+=1;
		Add+=1;

	if(r<B){
		size = q+b; 
		rr=r-offsetp;
		initialState = ((initialState+1)<<size)+rr; 
		
		Branch+=1;
		Add+=4;
		Bitwise+=1;
	}
	else{
		size = q+b+1;
		rr=r+B-offset; 
		initialState = ((initialState+1)<<size)+rr; 
		
		Branch+=1;
		Add+=6;
		Bitwise+=1;	
	}
	preLength += size;
	Add+=1;
	
	if (preLength>32) {
		preLength -= 32;
		*ResArr++ = (uint32_t)(initialState>>preLength);
		initialState &= (1<<preLength)-1;
		
		Branch+=1;
		Add+=3;
		Bitwise+=3;		
	}
 }
	if(preLength) {
		*ResArr++ = (uint32_t)initialState<<32-preLength;
		
		Branch+=1;
		Add+=2;
		Bitwise+=1;	
	}
  printf("The average arithmetic count of Golomb_Encode:\n");
 printf("Add=%.2f, Multi=%.2f,Branch=%.2f,Bitwise=%.2f\n",(double)Add/in_size,(double)Multi/in_size,(double)Branch/in_size,(double)Bitwise/in_size);
 return ResArr - resArr;
}

int our_enc(uint32_t* value, size_t in_size,uint32_t* resArr) {
 uint64_t initialState = 0;
 int preLength = 0;

 uint32_t* ResArr = resArr;

 uint32_t Add=0,Multi=0,Branch=0,Bitwise=0;
 for (uint32_t* Val = value; Val < value + in_size; Val++) {
     int q = ((*Val+S) * m)>>16; 
 
     Add+=1;
     Multi+=1;
     Bitwise+=1;
	 
     switch(q){
         case 0: initialState = (initialState << b) + *Val; Branch+=1;Add+=1;Bitwise+=1;break;
         case 1: initialState = ((initialState << b) + *Val<<1) + 1;Branch+=1;Add+=2;Bitwise+=2;break;
         case 2: initialState = ((initialState << b) + *Val - M<<2) + 1;Branch+=1;Add+=3;Bitwise+=2;break;
         default: initialState = ((initialState << b) + *Val - (q-1)*M<<q) + 1;Branch+=1;Add+=4;Bitwise+=2;Multi+=1;
     }
     preLength += q+b;

     Add+=2;

     if (preLength>32) {
         preLength -= 32;
     	*ResArr++ = (uint32_t)(initialState>>preLength);
     	initialState &= (1<<preLength)-1;

         Branch+=1;
         Add+=3;
         Bitwise+=3;
     }
 }
     if(preLength) {
 	*ResArr++ = (uint32_t) initialState<<32-preLength;

     Branch+=1;
 	Add+=2;
 	Bitwise+=1;
 }

  printf("The average arithmetic count of Ours_Encode:\n");
 printf("Add=%.2f, Multi=%.2f,Branch=%.2f,Bitwise=%.2f\n",(double)Add/in_size,(double)Multi/in_size,(double)Branch/in_size,(double)Bitwise/in_size);
  printf("\n");

 return ResArr - resArr;
}

void golomb_dec(uint32_t* ptr, size_t in_size,uint32_t* res) {
    uint64_t initialstate = 0;
    int8_t prelength = 0;
    uint32_t* Ptr = ptr;
	
   uint32_t Add=0,Multi=0,Branch=0,Bitwise=0;
   for (uint32_t* Res = res; Res < res + in_size; ++Res) {
        if (prelength < 32) {
            initialstate += ((uint64_t)(*Ptr++))<<32-prelength;
            prelength += 32;

         Branch+=1;
         Add+=4;
         Bitwise+=1;
        }

        int q = _lzcnt_u64(~initialstate);
        initialstate <<= q+1;
        prelength -= q+1;
        int r = initialstate>>(65-b);
        initialstate <<= b-1;
        prelength -= b-1;

        Add+=7;
        Bitwise+=4;

        if(r>=B){ 
            r = (r<<1) + (initialstate>>63) - B; 
            initialstate <<= 1;
            prelength--;

        Branch+=1;
        Add+=3;
        Bitwise+=3;	
        }
        *Res = q*M + r;

      Multi+=1;
      Add+=1;
    }
   printf("The average arithmetic count of Golomb_Decode:\n");
   printf("Add=%.2f, Multi=%.2f, Branch=%.2f,Bitwise=%.2f\n",(double)Add/in_size,(double)Multi/in_size,(double)Branch/in_size,(double)Bitwise/in_size);
   printf("\n");
    return;
}


void our_dec(uint32_t* ptr, size_t in_size,uint32_t* res) {
    uint64_t initialstate = 0;
    int prelength = 0;

    uint32_t* Ptr = ptr;
  uint32_t Add=0,Multi=0,Branch=0,Bitwise=0;
    for(uint32_t* Res = res; Res < res + in_size; ++Res) {
        if (prelength < 32){
            initialstate += ((uint64_t)(*Ptr++))<<32-prelength;
            prelength += 32;

         Branch+=1;
         Add+=4;
         Bitwise+=1;
        }

        *Res = initialstate>>(64-b);
        initialstate <<= b;
        prelength -= b;

        Add+=2;
        Bitwise+=2;

        if(*Res >= B){  
            int q = _lzcnt_u64(initialstate);
            initialstate <<= q+1;
            prelength -= q+1;
            *Res += q*M;

         Branch+=1;
         Add+=4;
         Bitwise+=1;
         Multi+=1;
        }
    }
   printf("The average arithmetic count of Our_Decode:\n");
   printf("Add=%.2f, Multi=%.2f, Branch=%.2f,Bitwise=%.2f\n",(double)Add/in_size,(double)Multi/in_size,(double)Branch/in_size,(double)Bitwise/in_size);
  printf("\n");
   return;
}

void main (){
    clock_t clockBegin;
    int ir = LOOP;
    const gsl_rng_type * T;
    gsl_rng * r;

    int i, n = instancias;

    uint32_t value[instancias] = {0};
    uint32_t code[instancias];
    uint32_t dec[instancias] = {0};
    double mu =getP(M+0.5);
    printf("M= %d, P= %f\n", M,mu);
   
  gsl_rng_env_setup();
  T = gsl_rng_default;
  r = gsl_rng_alloc(T);
  for (i = 0; i < n; i++)
    {
      unsigned int k = gsl_ran_geometric(r,mu);
      value[i]=k;
    }
    golomb_enc(value, instancias, code);
    golomb_dec(code, instancias, dec);
    our_enc(value, instancias, code);
    our_dec(code, instancias, dec);
	
    gsl_rng_free (r);
    return;
}

