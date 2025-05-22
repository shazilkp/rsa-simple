#include "bigint.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define MAXVAL 0xFFFFFFFFULL 

//stored as little endian

void big_int_swap(BigInt * a, BigInt * b){
	BigInt t = *a;
	*a = *b;
	*b = t;
}

void big_int_print(BigInt *a,int mode){
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

void big_int_add(BigInt *a, BigInt *b,BigInt *c){
	if(b->size > a->size){
		big_int_swap(a,b);
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
	printf("%zu %zu %zu\n",sizeA,sizeB,sizeC);
	
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
	
	i = sizeC-1;
	while( i > 0 && c->integer[i] == 0x0){
		i--;
	}
	c->integer = realloc(c->integer,sizeof(uint32_t) * (i+1));
	c->size = i + 1;
	
}


