#include "bigint.h"
#include <stdio.h>


int main(){
	BigInt a = big_int_constructor(0,2, 0x00000000,0x00000001);
	BigInt b = big_int_constructor(0,1, 0x00000002);
	BigInt c;
	
	big_int_sub(&b,&a,&c);
	
	
	big_int_print(&c,0);
	big_int_print(&c,1);
	
	big_int_destructor(&a);
	big_int_destructor(&b);
	
}
