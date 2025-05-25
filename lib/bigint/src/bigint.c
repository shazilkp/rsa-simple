#include "bigint.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define MAXVAL 0xFFFFFFFFULL 

//stored as little endian
#include <stdint.h>

int count_leading_zeros(uint32_t x) {
    if (x == 0) return 32;

    int count = 0;
    if ((x >> 16) == 0) { count += 16; x <<= 16; }
    if ((x >> 24) == 0) { count += 8;  x <<= 8;  }
    if ((x >> 28) == 0) { count += 4;  x <<= 4;  }
    if ((x >> 30) == 0) { count += 2;  x <<= 2;  }
    if ((x >> 31) == 0) { count += 1; }

    return count;
}

BigInt big_int_copy(BigInt a){
	BigInt b;
	b.sign = a.sign;
	b.size = a.size;
	
	b.integer = malloc(sizeof(uint32_t) * a.size);
	if (b.integer == NULL) {
		fprintf(stderr, "Memory allocation failed in big_int_copy.\n");
		exit(EXIT_FAILURE);
	}
	memcpy(b.integer, a.integer, sizeof(uint32_t) * a.size);
	return b;
}

void big_int_swap(BigInt * a, BigInt * b){
	BigInt t = *a;
	*a = *b;
	*b = t;
}

int big_int_is_zero(BigInt *a){
	for(size_t i = 0 ; i < a->size ; i++){
		if(a->integer[i] != 0x0){
			return 0;
		}
	}
	return 1;
}

int big_int_compare(BigInt *a,BigInt *b){
	//a == b   ->   0
	//a > b    ->   +ve
	//a < b	   ->   -ve
	
	if(a->sign == 1 && b->sign == 0){
		return -1;
	}
	if(a->sign == 0 && b->sign == 1){
		return 1;
	}
	//same sign
	int sign = a->sign;
	size_t sizeA = a->size;
	size_t sizeB = b->size;	
	if(sizeA > sizeB){
		if(sign){
			return -1;
		}
		else{
			return 1;
		}
	}
	
	if(sizeA < sizeB){
		if(sign){
			return 1;
		}
		else{
			return -1;
		}
	}
	
	//sizeA = sizeB here
	for(int i = sizeA - 1 ; i >= 0 ; --i){
		if(a->integer[i] > b->integer[i]){
			if(sign){
				return -1;
			}
			else{
				return 1;
			}
		}
		if(a->integer[i] < b->integer[i]){
			if(sign){
				return 1;
			}
			else{
				return -1;
			}
		}
	} 
	return 0;
}

void big_int_print(BigInt *a,int mode){
	if(!a){
		return;
	}
	if(a->sign == 1){
		printf("-");
	}
	if(!mode){
		for(size_t i = 0; i < a->size ; i++){
			printf("0x%08X ",a->integer[i]);
		}
	}
	else{
		for(int i = (a->size)-1; i >=0 ; --i){
			printf("0x%08X ",a->integer[i]);
		}
	}
	printf("\n");
}


BigInt big_int_constructor(short sign, size_t size, ...){
	BigInt number;
	number.integer = malloc(sizeof(uint32_t) * size);
	va_list blocks;
	va_start(blocks,size);
	for(size_t i = 0; i < size ; i++){
		number.integer[i] = va_arg(blocks,uint32_t);
	}
	number.sign = sign;
	number.size = size;
	return number;
}

void big_int_destructor(BigInt * bignum){
	free(bignum->integer);
}

BigInt big_int_from_uint64_t(uint64_t num){
	return big_int_constructor(0,2,(uint32_t)(num & MAXVAL),(uint32_t)(num >> 32));
}

BigInt big_int_from_uint32_t(uint32_t num){
	return big_int_constructor(0,1,(uint32_t)(num & MAXVAL));
}

int big_int_count_leading_zeros(BigInt *a){
	return count_leading_zeros(a->integer[(a->size)-1]);
}

void big_int_inc(BigInt *a){

	short int carry = 0;
	size_t sizeA = a->size;
	
	for(size_t i = 0; i < sizeA ; i++){
		uint64_t temp;
		if(i == 0){
			temp = (uint64_t)a->integer[i] + carry + (uint64_t)0x1;
		}
		else{
			temp = (uint64_t)a->integer[i] + carry;
		}
		carry = (temp > MAXVAL);
		a->integer[i] = (temp & MAXVAL);
	}
	
	if(carry){
		a->integer = realloc(a->integer,sizeof(uint32_t) * (sizeA + 1));
		a->integer[sizeA] = 0x1;
		a->size = sizeA + 1;
	}
}




void big_int_word_shift_r(BigInt *a,size_t s){
	
	if(a->size == 0 || s==0){
		return;
	}
	
	if(s >= a->size){
		a->integer[0] = 0x0;
		uint32_t *new_mem = realloc(a->integer,sizeof(uint32_t) * (1));
		if(!new_mem){
			return;
		}
		a->integer = new_mem;
		a->size = 1;
		return;
	}
	
	//s < a->size
	size_t sizeA = a->size; 
	
	memmove(a->integer,a->integer + s, (sizeA - s)*sizeof(uint32_t));
	
	uint32_t *new_mem = realloc(a->integer,sizeof(uint32_t) * (sizeA - s));
	if(!new_mem){
		return;
	}
	
	a->integer = new_mem;
	a->size = sizeA - s;
}

void big_int_word_shift_l(BigInt *a,size_t s){
	
	if(a->size == 0 || s==0){
		return;
	}
	size_t oldSize = a->size;
	
	uint32_t *new_mem = realloc(a->integer,sizeof(uint32_t) * (a->size + s));
	if(!new_mem){
		return;
	}
	a->integer = new_mem;
	a->size = a->size + s;
	
	
	memmove(a->integer+s,a->integer, oldSize*sizeof(uint32_t));
	memset(a->integer,0,s * sizeof(uint32_t));
}




void big_int_bit_shift_r(BigInt *a,size_t s){
	
	if(a->size == 0 || s==0){
		return;
	}
	
	if(s >= 32){		//shift word
		return;
		size_t s_word = s / 32;
		big_int_word_shift_r(a,s_word);
		s = s % 31;
		
	}
	
	size_t i = 0;
	size_t sizeA = a->size; 
	for(i = 0 ; i < sizeA - 1 ; ++i){
		uint32_t shifted = ((a->integer[i]) >> s);
		uint32_t carried = (a->integer[i+1]) << (32-s);
		
		a->integer[i] = shifted | carried;
	}
	a->integer[sizeA-1] = a->integer[sizeA-1] >> s;
	
	//shrinking
	i = sizeA-1;
	while( i > 0 && a->integer[i] == 0x0){
		i--;
	}
	a->integer = realloc(a->integer,sizeof(uint32_t) * (i+1));
	a->size = i + 1;
}

void big_int_bit_shift_l(BigInt *a,size_t s){
	if(a->size == 0 || s==0){
		return;
	}
	
	if(s >= 32){		//shift word
		return;
		size_t s_word = s / 32;
		big_int_word_shift_l(a,s_word);
		s = s % 31;
	}	
	
	a->integer = realloc(a->integer,sizeof(uint32_t) * (a->size + 1));
	a->size = a->size + 1;
	
	size_t i = 0;
	size_t sizeA = a->size; 
	
	a->integer[sizeA - 1] = a->integer[sizeA-2] >> (32-s);
	for(i = sizeA - 2 ; i > 0 ; --i){
		uint32_t shifted = ((a->integer[i]) << s);
		uint32_t carried = (a->integer[i-1]) >> (32-s);
		
		a->integer[i] = shifted | carried;
	}
	a->integer[0] = a->integer[0] << s;
	
	
	//shrinking
	i = sizeA-1;
	while( i > 0 && a->integer[i] == 0x0){
		i--;
	}
	a->integer = realloc(a->integer,sizeof(uint32_t) * (i+1));
	a->size = i + 1;
}




void big_int_add(BigInt *a, BigInt *b,BigInt *c){
	if(b->size > a->size){
		big_int_add(b,a,c);
		return;
	}
	//a is always the larger number 
	size_t sizeA = a->size;
	size_t sizeB = b->size;
	
	c->integer = malloc(sizeof(uint32_t) * sizeA);
	c->size = sizeA;
	
	short int carry = 0;
	
	size_t i;
	for(i = 0 ; i < sizeB ; i++){
		uint64_t temp = (uint64_t)a->integer[i] + (uint64_t)b->integer[i] + carry;
		carry = (temp > MAXVAL);
		c->integer[i] = (temp & MAXVAL);	
		
	}
	
	for(i = sizeB ; i < sizeA ; i++){
		uint64_t temp = (uint64_t)a->integer[i] + carry;
		carry = (temp > MAXVAL);
		c->integer[i] = (temp & MAXVAL);
		
	}
	
	if(carry){
		c->integer = realloc(c->integer,sizeof(uint32_t) * (sizeA + 1));
		c->integer[sizeA] = 0x1;
		c->size = sizeA + 1;
	}
}

void big_int_sub(BigInt *a, BigInt *b,BigInt *c){
	size_t sizeA = a->size;
	size_t sizeB = b->size;
	size_t sizeC = (sizeA > sizeB ? sizeA : sizeB);
	
	c->integer = malloc(sizeof(uint32_t) * sizeC);
	c->size = sizeC;
	
	short int borrow = 0;
	
	size_t i;
	for(i = 0 ; i < sizeC ; i++){
		uint64_t temp1 = (i < sizeA ? (uint64_t)a->integer[i] : (uint64_t)0x0)  + (MAXVAL + 1);
		uint64_t temp2 = (i < sizeB ? (uint64_t)b->integer[i] : (uint64_t)0x0)  + (uint64_t)borrow;
		uint64_t res = temp1 - temp2;
		c->integer[i] = (res & MAXVAL);	
		borrow = (res <= MAXVAL);
	}
		
	if(borrow){
		c->sign = 1;
		for(i = 0 ; i < sizeC ; i++){
			c->integer[i] = ~c->integer[i];
		}
		big_int_inc(c);
	}
	else{
		c->sign = 0;
	}
	
	i = sizeC-1;
	while( i > 0 && c->integer[i] == 0x0){
		i--;
	}
	c->integer = realloc(c->integer,sizeof(uint32_t) * (i+1));
	c->size = i + 1;
	
}



void big_int_mult(BigInt *a, BigInt *b,BigInt *c){
	if(b->size > a->size){
		big_int_mult(b, a,c);
		return;
	}
	
	//a is always the larger number 
	size_t sizeA = a->size;
	size_t sizeB = b->size;
	size_t sizeC = sizeA + sizeB;
	
	c->sign = a->sign * b->sign;
	
	c->integer = malloc(sizeof(uint32_t) * sizeC);
	memset(c->integer, 0, sizeof(uint32_t) * sizeC); //
	c->size = sizeC;
	
	
	for(size_t i = 0 ; i < sizeB ; ++i){	//for all elemnets of b
		uint32_t carry = 0x0;

		size_t j;
		for(j = i ; j < sizeA + i ;  j++){//for all elements of a
			uint64_t product = (uint64_t)a->integer[j-i] * (uint64_t)b->integer[i];
			uint64_t sum = (uint64_t)c->integer[j] + product + carry;
			c->integer[j] = (uint32_t)(sum & MAXVAL);
			carry = (uint32_t)(sum >> 32);
		}
		
		if(carry){
			uint64_t sum = (uint64_t)c->integer[j] + carry;
			c->integer[j] = (uint32_t)(sum & MAXVAL);
		}
	}
	
	size_t i = sizeC-1;
	while( i > 0 && c->integer[i] == 0x0){
		i--;
	}
	c->integer = realloc(c->integer,sizeof(uint32_t) * (i+1));
	c->size = i + 1;
}

int big_int_div(BigInt *u, BigInt *v,BigInt *q, BigInt *r){
	//Knuth-D implementation based on https://github.com/hcs0/Hackers-Delight/
	
	/*
      	1. Space q for the quotient, m - n + 1 words (at least one).
   	2. Space r for the remainder (optional), n words.
   	3. The dividend u, m words, m >= 1.
   	4. The divisor v, n words, n >= 2.
   	*/
   
   
	uint64_t BASE = 0x100000000;		// number base (2^32)
	uint32_t *un,*vn;			//normalised u,v
	
	uint64_t q_hat;
	uint64_t r_hat;
	uint64_t p;				//product of 2 digits(chunks) 
	
	size_t m = u->size;
	size_t n = v->size;
	
	size_t sizeQ = m - n + 1;
	size_t sizeR = n;
	
	long long t, k;
   	int i, j;
   	
   	
	q->integer = malloc(sizeof(uint32_t) * sizeQ);
	q->size = sizeQ;
	
	if(r){
		r->integer = malloc(sizeof(uint32_t) * sizeR);
		r->size = sizeR;
	}
	
	if(m < n || n <=0 || v->integer[n-1] == 0){
		return 1;
	}
	
	//easy casse hard coded
	if(n == 1){
		k = 0;
		for(j = m - 1 ; j >= 0 ; --j){
			q->integer[j] = (k * BASE + u->integer[j])/v->integer[0];
			k = (k * BASE + u->integer[j]) - q->integer[j]*v->integer[0];
		}
		if(r){
			r->integer[0] = k;
		}
		return 0;
	}
	
	
	//normalising v
	int s = big_int_count_leading_zeros(v);
	
	vn = (uint32_t*)malloc(sizeof(uint32_t) * n);
	un = (uint32_t*)malloc(sizeof(uint32_t) * (m + 1));
	
	for(i = n - 1; i > 0 ; i --){
		uint32_t shifted = v->integer[i] << s;
		uint32_t carried = (uint64_t)v->integer[i-1] >> (32 - s);
		
		vn[i] = shifted | carried;
	}
	vn[0] = v->integer[0] << s;
	
	
	
	//normalising u
	un[m] = (uint64_t)u->integer[m-1] >> (32 - s);
	
	for(i = m - 1 ; i > 0 ; --i){
		uint32_t shifted = u->integer[i] << s;
		uint32_t carried = (uint64_t)u->integer[i-1] >> (32 - s);
		
		un[i] = shifted | carried;
	}
	un[0] = u->integer[0] << s;
	
	//main loop
	for(j = m-n; j >= 0; j--){ //for each quotient digit
		//estimate qhat of q[j] using top 2 digits of dividend(u)
		uint64_t numerator = ((uint64_t)un[j+n] << 32) | un[j+n-1]; //top 2 digits of u(chunks)
		q_hat = numerator / vn[n-1];				//top 1 digit of v
		r_hat = numerator % vn[n-1];
		
		
		//refine qhat
		while(q_hat >= BASE || (q_hat * vn[n-2]) > ((r_hat << 32) + un[j+n-2])){
			q_hat = q_hat - 1;
			r_hat = r_hat + vn[n-1];
			if(r_hat >= BASE){
				break;
			}
		}
		
		
		//main procedure un[j..... j+n] - vn * qhat
		k = 0;
		for(i = 0 ; i < n  ; i++){
		
			p = q_hat * vn[i];
			t = (uint64_t)un[j+i] - k - (p & MAXVAL);
			un[j+i] = (uint32_t)(t & MAXVAL);
			// Borrow = 1 if upper 32 bits are non-zero, else 0
			k = (p >> 32) - (t >> 32);

		}
		t = (uint64_t)un[j+n] - k;
		un[j+n] = (uint32_t)(t & MAXVAL);
		
		
		//store q
		q->integer[j] = (uint32_t)(q_hat & MAXVAL);
		
		//subtraction underflowed implied q_hat is large or overestimated
		if ((int64_t)t < 0) {
			q->integer[j] -= 1;
			
			//add back vn to current slice of un to restore remainder for next iter
			uint64_t carry = 0;
			for (i = 0; i < n; i++) {
				uint64_t sum = (uint64_t)un[j+i] + vn[i] + carry;
			 	un[j+i] = (uint32_t)sum;
				carry = sum >> 32;
			}
			un[j+n] += carry;
		}
	}
	//main loop done
	
	if(r){
		//unnormalise(reverse what we did) to remainder
		for(i = 0 ; i < n - 1 ; ++i){
			uint32_t shifted = (un[i] >> s);
			uint32_t carried = (uint64_t)un[i+1] << (32-s);
			
			r->integer[i] = shifted | carried;
		}
		r->integer[n-1] = un[n-1] >> s;
	}
	
	i = sizeR-1;
	while( i > 0 && r->integer[i] == 0x0){
		i--;
	}
	r->integer = realloc(r->integer,sizeof(uint32_t) * (i+1));
	r->size = i + 1;
	
	i = sizeQ-1;
	while( i > 0 && q->integer[i] == 0x0){
		i--;
	}
	q->integer = realloc(q->integer,sizeof(uint32_t) * (i+1));
	q->size = i + 1;
	
	
	free(un);
	free(vn);
	return 0;
	
}

int big_int_mod(BigInt *a,BigInt *b, BigInt * c){
	BigInt t;
	return big_int_div(a,b,&t,c);
}

int big_int_modpow(BigInt * a,BigInt *b, BigInt * c, BigInt * d){
	//mod pow by sauring,binary exponentiation
	//d = a^b mod c
	BigInt base = big_int_copy(*a);
	BigInt exponent = big_int_copy(*b);
	
	*d = big_int_constructor(0,1,0x1);
	
	
	while(!big_int_is_zero((&exponent))){
		if(exponent.integer[0] % 2 == 1){

			BigInt p;
			big_int_mult(d,&base,&p);		//p  = res * exponent
			big_int_mod(&p,c,d);				//res = p % c (res  = p % mod)	
			big_int_destructor(&p);
		}
		big_int_bit_shift_r(&exponent,1);		//exponent = exponent/2;
		
		BigInt baseSquare;
		big_int_mult(&base,&base,&baseSquare);		//baseSquare = base * base;
		
		big_int_mod(&baseSquare,c,&base);		//base = baseSquare % c .. base =\ base ** 2 % c;
		big_int_destructor(&baseSquare);
	}
	
	big_int_destructor(&base);
    	big_int_destructor(&exponent);
    	return 0;
}


void big_int_gcd(BigInt *a, BigInt *b, BigInt *c){
	//binary gcd algo(Stein's algorithm)
	if(big_int_is_zero(a)){
		*c = big_int_copy(*b);
		return;
	}
	if(big_int_is_zero(b)){
		*c = big_int_copy(*a);
		return;
	}
	
	BigInt x = big_int_copy(*a);
    	BigInt y = big_int_copy(*b);
	
	int count = 0;
	while(!(x.integer[0] & 1) && !(y.integer[0] & 1)){
		big_int_bit_shift_r(&x,1);
		big_int_bit_shift_r(&y,1);
		count++;
	}
	BigInt ZERO = big_int_constructor(0,1, 0x0);
	while((big_int_compare(&y,&ZERO) > 0)){
		while(!(x.integer[0] & 1)){
			big_int_bit_shift_r(&x,1);
		}
		while(!(y.integer[0] & 1)){
			big_int_bit_shift_r(&y,1);
		}
		
		if((big_int_compare(&x, &y)) > 0){
			//a = a-b
			BigInt t;
			big_int_sub(&x,&y,&t);
			big_int_destructor(&x);
			x = t;

			//a = a/2
			big_int_bit_shift_r(&x,1);
		}
		else{
			//b = b-a			
			BigInt t;
			big_int_sub(&y,&x,&t);
			big_int_destructor(&y);
			y = t;
			
			//b = b/2
			big_int_bit_shift_r(&y,1);
		}
	}
	big_int_bit_shift_l(&x,count); //gcd = a << count;
	*c = big_int_copy(x);
}




