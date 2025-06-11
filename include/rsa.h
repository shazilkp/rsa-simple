#ifndef RSA_H
#define RSA_H

#include "bigint.h"

int fill_random_bytes(unsigned char *buff, size_t len);

int big_int_random(BigInt *a, size_t bits);
int big_int_random_between(BigInt *a, BigInt *b, BigInt *result);
int big_int_random_range(BigInt *a, BigInt * upper_bound);


int big_int_prime_check(BigInt * a,size_t iteration);
int big_int_generate_prime(size_t bits, BigInt * a);

//rsa
int generate(BigInt *n, BigInt *e, BigInt *d, size_t bits);
int encrypt(BigInt * m,BigInt * key, BigInt * n,BigInt * c);
int decrypt(BigInt * c,BigInt * key, BigInt * n,BigInt * m);
#endif
