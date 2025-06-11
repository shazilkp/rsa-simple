#include "bigint.h"
#include <sys/random.h>
#include <errno.h>
#include <stdio.h>


int fill_random_bytes(unsigned char *buff, size_t len){
    size_t total = 0;
    while(total < len){
        ssize_t filled = getrandom(buff+total,len - total,0);
        if(filled < 0){
            if (errno == EINTR) continue;
            else return -1;
        }
        total+=filled;
    }
    return 0;
}

int big_int_random(BigInt *a, size_t bits){
    size_t len = (bits + 7) / 8;      //bits to byte
   // printf("Len in bytes =%zu\n",len);
    unsigned char * buff = malloc(len);
    if(!buff){
        return -1;
    }

    int result = fill_random_bytes(buff,len);
    if(result == -1){
        free(buff);
        return -1;
    }
    //BigInt raw = big_int_from_byte_array_le(buff,len);
    // printf("raw byte array generated = ");
    // big_int_print(&raw,0);
    // printf("\n");


    size_t unused_bits = (8 * len) - bits;

    // Clear unused high bits in MSB byte
    buff[len - 1] &= (0xFF >> unused_bits);

    // Set top bit to ensure correct bit length
    buff[len - 1] |= (1 << (7 - unused_bits));

    BigInt t = big_int_from_byte_array_le(buff,len);
    // printf(" byte array after processing generated = ");
    // big_int_print(&t,0);
    // printf("\n");

    *a = big_int_copy(t);
    return 0;

}

int big_int_random_range(BigInt *a, BigInt * upper_bound){
    // printf("upper bound/range = ");
    // big_int_print(upper_bound,0);
    // printf("\n");
    if(big_int_is_zero(upper_bound)){
        return -1;
    }
    size_t bits = big_int_bit_length(upper_bound);

   // printf("Bits in upper bound = %zu\n",bits);
    BigInt b = {NULL,0,0};
    do{
        if(big_int_random(&b,bits) == -1){
            return -1;
        }
        // printf("candidate = ");
        // big_int_print(&b,0);
        // printf("\n");
    }
    while (big_int_compare(&b,upper_bound,0) >= 0);
    *a = big_int_copy(b);

    return 0;
}

int big_int_random_between(BigInt *a, BigInt *b, BigInt *result){
    if (big_int_compare(a, b, 0) >= 0) return -1;

    BigInt one = big_int_from_uint32_t(1);
    BigInt range = {NULL , 0 ,0};
    BigInt r = {NULL , 0 , 0};
    BigInt t = {NULL , 0, 0};
    big_int_sub(b, a, &range);        // range = b - a
    big_int_add(&range, &one, &t);    // t = range + 1
    big_int_destructor(&range);
    range.sign = t.sign;
    range.integer = t.integer;
    range.size = t.size;

    int ok = big_int_random_range(&r, &range); // r in [0, range - 1]
    if (ok != 0) return -1;

    big_int_add(&r, a, result); // result = a + r

    // Clean up
    big_int_destructor(&r);
    big_int_destructor(&range);
    big_int_destructor(&one);

    return 0;
}


int big_int_prime_check(BigInt *n, size_t iterations) {
    //raber-miller primality est
    BigInt one = big_int_from_uint32_t(1);
    BigInt two = big_int_from_uint32_t(2);

    if (big_int_compare(n, &two, 0) < 0) return 0;         // n < 2 = not prime
    if (big_int_compare(n, &two, 0) == 0) return 1;        // n == 2 = prime
    if ((n->integer[0] & 1) == 0) return 0;                // even = not prime

    BigInt n_minus_one = {NULL,0,0};
    BigInt  d;
    big_int_sub(n, &one, &n_minus_one);
    d = big_int_copy(n_minus_one);

    size_t s = 0;
    while ((d.integer[0] & 1) == 0) {
        big_int_bit_shift_r(&d, 1);
        s++;
    }

    for (size_t i = 0; i < iterations; i++) {
        BigInt a = {NULL,0,0};
        big_int_random_between(&two, &n_minus_one, &a);

        BigInt x = {NULL , 0, 0};
        big_int_modpow(&a, &d, n, &x);  // x = a^d mod n

        if (big_int_compare(&x, &one, 0) == 0 || big_int_compare(&x, &n_minus_one, 0) == 0) {
            big_int_destructor(&a);
            big_int_destructor(&x);
            continue;
        }

        int found = 0;
        for (size_t r = 1; r < s; r++) {
            BigInt x_squared;
            big_int_mult(&x, &x, &x_squared);
            big_int_mod(&x_squared, n, &x);

            big_int_destructor(&x_squared);

            if (big_int_compare(&x, &n_minus_one, 0) == 0) {
                found = 1;
                break;
            }
        }

        big_int_destructor(&a);
        big_int_destructor(&x);

        if (!found) {
            big_int_destructor(&d);
            big_int_destructor(&n_minus_one);
            big_int_destructor(&one);
            big_int_destructor(&two);
            return 0;  // composite
        }
    }

    big_int_destructor(&d);
    big_int_destructor(&n_minus_one);
    big_int_destructor(&one);
    big_int_destructor(&two);
    return 1;  // probably prime
}

int big_int_generate_prime(size_t bits, BigInt * a){
    int found = 0;
    int c = 0;
    while(!found){
        BigInt random = {NULL, 0, 0};
        big_int_random(&random,bits);
        random.integer[0] |= 0x1;

        if(big_int_prime_check(&random,40)){
            found = 1;
            *a = big_int_copy(random);
        }
        c++;
    }
    printf("took %d trials\n",c);
    return 1;
}


int generate(BigInt *n, BigInt *e, BigInt *d, size_t bits){
    BigInt  p = {NULL , 0 , 0};
    BigInt  q = {NULL , 0 , 0};
    big_int_generate_prime(bits,&p);
    big_int_generate_prime(bits,&q);
    big_int_mult(&p,&q,n);

    BigInt phi_n = {NULL , 0 ,0};
    BigInt one = big_int_from_uint32_t(1);
    BigInt p_minus_one = {NULL,0,0};
    BigInt q_minus_one = {NULL,0,0};
    big_int_sub(&p,&one,&p_minus_one);
    big_int_sub(&q,&one,&q_minus_one);

    big_int_mult(&p_minus_one,&q_minus_one,&phi_n);         //phi(n) = (p-1)*(q-1)

    //d is any prime greater than max(p,q)
    BigInt *max;
    if (big_int_compare(&p, &q, 1) > 0) {
        max = &p;
    } else {
        max = &q;
    }

    int found = 0;
    BigInt d_candidate = {NULL, 0 ,0 };
    while(!found){
        big_int_generate_prime(bits,&d_candidate);
        if(big_int_compare(&d_candidate,max,1) > 0){
            found = 1;
        }
    }
    *d = big_int_copy(d_candidate);


    big_int_modinv(d,&phi_n,e);
    return 1;
}

//c is cipher text of message m
int encrypt(BigInt * m,BigInt * key, BigInt * n,BigInt * c){
    big_int_modpow(m,key,n,c);
    return 1;
}

//m is plain text of cipher  c
int decrypt(BigInt * c,BigInt * key, BigInt * n,BigInt * m){
    big_int_modpow(c,key,n,m);
    return 1;
}