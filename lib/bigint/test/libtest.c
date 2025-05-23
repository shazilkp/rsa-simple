#include "bigint.h"
#include <stdio.h>


int main(){
	BigInt a = big_int_constructor(0,4,  0xCCDDEEFF, 0x9900AABB, 0x55667788, 0x11223344);
	BigInt b = big_int_constructor(0,1,  0x00010001);
	BigInt c = big_int_constructor(0,4, 0x33221100, 0x77665544, 0xBBAA9988, 0xFFEEDDCC);
	BigInt r,m;
	
	big_int_modpow(&a,&b,&c,&r);
	big_int_print(&r,1);
	//0x05DAB7FA 0xA150868B
	// 0x00000005 0x64B68791 
	
	/*
	BigInt t = big_int_constructor(0,2,  0xA150868B,0x05DAB7FA );
	big_int_mult(&t,&t,&m);
	big_int_print(&t,1);
	big_int_print(&m,1);
	
	*/
	
	big_int_destructor(&a);
	big_int_destructor(&b);
	big_int_destructor(&c);
	big_int_destructor(&r);
}
