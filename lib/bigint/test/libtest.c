#include "bigint.h"
#include <stdio.h>


int main(){
	BigInt a = big_int_constructor(0, 16,
	    0x97555800 , 0x6A321112 , 0xB9DB9D03 , 0x36A0D281 ,
	    0x1668B5B3 , 0x09E3345E , 0x5C4F2B9D , 0x5DD10C1A ,
	    0xB9B77BFC , 0xBEDA9538 , 0x3668A252 , 0x2A71BE5A ,
	    0x2030D449 , 0x0A12A85F , 0xA7A14B50 , 0x05BBCCE3);

	BigInt b = big_int_constructor(0, 16,
	    0x5C189D00 , 0xA3B2E4FB , 0xFCB93DA0 , 0x4453859B ,
	    0xA9222471 , 0x15E747A2 , 0x5D0B2B12 , 0x39DC45CB ,
	    0x669F8809 , 0xC1859E49 , 0xC5FEE768 , 0x20E4FDA8 ,
	    0x19960098 , 0x4F513BA2 , 0x65ACF9F7 , 0x2B5C3D79);
	    
	BigInt c = big_int_constructor(0,4, 0x33221100, 0x77665544, 0xBBAA9988, 0xFFEEDDCC);
	BigInt r;
	
	
	//0xE34A15CD 0xBB237AAD 0x99213458 0xAABBCCDD 0xEEFF0011 0x22334455 0x66778899 
	//0x66778899 , 0x22334455 , 0xEEFF0011 , 0xAABBCCDD,  0x99213458 , 0xBB237AAD ,0xE34A15CD
	
	//0xE34A15CD 0xBB237AAD 0x99213458 0xAABBCCDD 0x11223344 0x55667788 0x9900AABB 0xCCDDEEFF 
	//0xCCDDEEFF, 0x9900AABB,0x55667788,0x11223344 ,0xAABBCCDD,  0x99213458 , 0xBB237AAD ,0xE34A15CD
	
	big_int_gcd(&a,&b,&r);
	big_int_print(&r,1);
	
	//0x05DAB7FA 0xA150868B
	// 0x00000005 0x64B68791 
	
	/*
	BigInt t = big_int_constructor(0,2,  0xA150868B,0x05DAB7FA );
	big_int_mult(&t,&t,&m);
	big_int_print(&t,1);
	big_int_print(&m,1);
	
	*/
	
	/*
	BigInt f = big_int_constructor(0,7,  0x66778899 , 0x22334455 , 0xEEFF0011 , 0xAABBCCDD,  0x99213458 , 0xBB237AAD ,0xE34A15CD);
	BigInt g = big_int_constructor(0,8,  0xCCDDEEFF, 0x9900AABB,0x55667788,0x11223344 ,0xAABBCCDD,  0x99213458 , 0xBB237AAD ,0xE34A15CD);
	BigInt h;
	big_int_sub(&g,&f,&h);
	big_int_print(&h,1);
	*/
	
	
	big_int_destructor(&a);
	big_int_destructor(&b);
	big_int_destructor(&c);
	//big_int_destructor(&r);
}
