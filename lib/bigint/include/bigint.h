#include <stdlib.h>
#include <stdint.h>

typedef struct {
	uint32_t * integer;
	size_t size;		//no of elements in array
	short sign;
} BigInt;


BigInt big_int_constructor(short sign, size_t size, ...);
void big_int_destructor(BigInt * bignum);

BigInt big_int_from_uint64_t(uint64_t num);
BigInt big_int_from_uint32_t(uint32_t num);

void big_int_add(BigInt *a, BigInt *b,BigInt *c);
void big_int_sub(BigInt *a, BigInt *b,BigInt *c);
void big_int_mult(BigInt *a, BigInt *b,BigInt *c);
int big_int_div(BigInt *u, BigInt *v,BigInt *q, BigInt *r);		//divmod
int big_int_mod(BigInt *a,BigInt *b, BigInt * c);
int big_int_modpow(BigInt * a,BigInt *b, BigInt * c, BigInt * d);


void big_int_inc(BigInt *a);
void big_int_bit_shift_r(BigInt *a,size_t s);

int big_int_is_zero(BigInt *a);

void big_int_print(BigInt * a,int mode);
void big_int_swap(BigInt * a, BigInt * b);
int big_int_count_leading_zeros(BigInt *a);
