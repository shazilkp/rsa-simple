#include "bigint.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define MAXVAL 0xFFFFFFFFULL  // 32-bit max, must be 64-bit to avoid overflow
//stored as little endian

void big_int_swap(BigInt * a, BigInt * b){
	BigInt t = *a;
	*a = *b;
	*b = t;
}

void big_int_print(BigInt a,int mode){
	if(!mode){
		for(size_t i = 0; i < a.size ; i++){
			printf("%X ",a.integer[i]);
		}
	}
	else{
		for(int i = a.size-1; i >=0 ; --i){
			printf("%X ",a.integer[i]);
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
	free(bignum->hex);
}


BigInt big_int_add(BigInt a, BigInt b){
	if(b.size > a.size){
		big_int_swap(&a,&b);
	}
	//a is always the larger number 
	size_t sizeA = a.size;
	size_t sizeB = b.size;
	
	BigInt c;
	c.integer = malloc(sizeof(uint32_t) * sizeA);
	c.size = sizeA;
	
	short int carry = 0;
	
	size_t i;
	for(i = 0 ; i < sizeB ; i++){
		uint64_t temp = (uint64_t)a.integer[i] + (uint64_t)b.integer[i] + carry;
		carry = (temp > MAXVAL);
		c.integer[i] = (temp & MAXVAL);	
		
	}
	
	for(i = sizeB ; i < sizeA ; i++){
		uint64_t temp = (uint64_t)a.integer[i] + carry;
		carry = (temp > MAXVAL);
		c.integer[i] = (temp & MAXVAL);
		
	}
	
	if(carry){
		c.integer = realloc(c.integer,sizeof(uint32_t) * (sizeA + 1));
		c.integer[sizeA] = 0x1;
		c.size = sizeA + 1;
	}
	return c;
	
}

