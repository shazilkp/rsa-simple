#include "bigint.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>



BigInt big_int_constructor(short sign, size_t size, ...){
	BigInt number;
	number.integer = malloc(sizeof(uint64_t) * size);
	va_list blocks;
	va_start(blocks,size);
	for(size_t i = 0; i < size ; i++){
		number.integer[i] = va_arg(blocks,uint64_t);
	}
	number.sign = sign;
	number.size = size;
	number.hex = malloc((size * sizeof(char) * 16) + 1);
	
	for(size_t i = 0; i < size ; i++){
		char x[17];
		sprintf(x,"%lX",number.integer[i]);
		strcat(number.hex,x);
		
	}
	return number;
}
void big_int_destructor(BigInt * bignum){
	free(bignum->integer);
	free(bignum->hex);
}
