#include <stdlib.h>
#include <stdint.h>

typedef struct {
	uint64_t * integer;
	size_t size;		//no of elements in array
	short sign;
	char * hex;
} BigInt;


BigInt big_int_constructor(short sign, size_t size, ...);
void big_int_destructor(BigInt * bignum);
