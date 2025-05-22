#include "bigint.h"
#include <stdio.h>


int main(){
	BigInt u = big_int_constructor(0,4, 0,0,0x00008000,0x00007fff);
	BigInt v = big_int_constructor(0,3,  1,0,0x00008000);
	BigInt q,r;
	
	big_int_div(&q,&r,&u,&v);
	
	
	big_int_print(&q,0);
	big_int_print(&r,0);
	//big_int_print(&c,1);
	
	big_int_destructor(&u);
	big_int_destructor(&v);
	big_int_destructor(&q);
	big_int_destructor(&r);
}
