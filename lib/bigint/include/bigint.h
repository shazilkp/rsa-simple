#include <stdlib.h>
#include <stdint.h>

typedef struct {
	uint32_t * integer;
	size_t size;		//no of elements in array
	short sign;
} BigInt;


BigInt big_int_constructor(short sign, size_t size, ...);
void big_int_destructor(BigInt * bignum);

void big_int_add(BigInt *a, BigInt *b,BigInt *c);
void big_int_sub(BigInt *a, BigInt *b,BigInt *c);
void big_int_inc(BigInt *a);

void big_int_print(BigInt * a,int mode);
void big_int_swap(BigInt * a, BigInt * b);
