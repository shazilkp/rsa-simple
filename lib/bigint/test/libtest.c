#include "bigint.h"
#include <stdio.h>


int main(){
	BigInt a = big_int_constructor(0,2, (uint32_t)0xFFFFFFFF, (uint32_t)0xFFFFFFFF);
	BigInt b = big_int_constructor(0,1, (uint32_t)0x1);
	BigInt c = big_int_add(b,a);
	
	
	big_int_print(c,0);
	big_int_print(c,1);
	
}
