# RSA CLI Tool with Custom BigInt Library
 
![Language](https://img.shields.io/badge/language-C-blue)

A command-line RSA tool written in C, using a **custom-built multiple-precision arithmetic (BigInt)** library. The RSA implementation is based on the original RSA paper.

---
## Disclaimer

This implementation is **strictly for educational use**.  
It is **not cryptographically secure** and should **not be used in production systems**.  
Use of this code in real-world applications may lead to **serious security vulnerabilities**.

---
## Project Structure

```
.
├── src/              # RSA logic and CLI interface
├── include/          # Header files
├── lib/
│   └── bigint/       # Custom BigInt arithmetic
├── Makefile          # Build rules
├── .gitignore
└── README.md
```

---

##  Features

- RSA key generation with configurable bit sizes  
- CLI interface for generation & encryption/decryption  
- Fully custom BigInt math 

---

## Building

```bash
make
```

To clean build artifacts:

```bash
make clean
```

---

##  Usage

###  Key Generation

```bash
./rsa-simple keygen --bits BITS --pub PUBFILE --priv PRIVFILE  //filenames are optional
```

Example:

```bash
./rsa-simple keygen 1024
```

#### Output Format

- Keys are output in **PEM like** format (Base64 with header)



#### Sample Output

```
Keys generated successfully and stored in pubkey.txt and privkey.txt
```
---
###  Encryption

```bash
./rsa-simple encrypt --pub PUBKEY -i INFILE -o OUTFILE
```
Example:

```bash
./rsa-simple encrypt --pub pubkey.txt -i message.txt -o encrypted.txt
```
---
###  Decryption

```bash
./rsa-simple decrypt --priv PRIVKEY -i INFILE -o OUTFILE
```
Example:

```bash
./rsa-simple decrypt --priv privkey.txt -i encrypted.txt -o decrypted.txt
```
---

## BigInt API


```C
BigInt big_int_constructor(short sign, size_t size, ...);
int big_int_safe_realloc(BigInt *a, size_t new_size_words);
void big_int_destructor(BigInt * bignum);

BigInt big_int_from_uint64_t(uint64_t num);
BigInt big_int_from_uint32_t(uint32_t num);
BigInt big_int_from_byte_array_le(unsigned char * buff,size_t buff_len);
BigInt big_int_from_byte_array_be(unsigned char * buff,size_t buff_len);

void big_int_uadd(BigInt *a, BigInt *b,BigInt *c);
void big_int_add(BigInt *a, BigInt *b,BigInt *c);
void big_int_usub(BigInt *a, BigInt *b,BigInt *c);
void big_int_sub(BigInt *a, BigInt *b,BigInt *c);
void big_int_mult(BigInt *a, BigInt *b,BigInt *c);
int big_int_div(BigInt *u, BigInt *v,BigInt *q, BigInt *r);		//divmod
int big_int_mod(BigInt *a,BigInt *b, BigInt * c);
int big_int_modpow(BigInt * a,BigInt *b, BigInt * c, BigInt * d);

void big_int_gcd(BigInt *a, BigInt *b, BigInt *c);
void big_int_xgcd(BigInt *a, BigInt *b, BigInt *gcd, BigInt *x_out, BigInt *y_out);
void big_int_modinv(BigInt *a, BigInt *b, BigInt *c);

void big_int_inc(BigInt *a);
void big_int_bit_shift_r(BigInt *a,size_t s);
void big_int_bit_shift_l(BigInt *a,size_t s);
void big_int_word_shift_r(BigInt *a,size_t s);
void big_int_word_shift_l(BigInt *a,size_t s);

int big_int_is_zero(BigInt *a);
int big_int_compare(BigInt *a,BigInt *b, int modulus);

void big_int_print(BigInt * a,int mode);
void big_int_swap(BigInt * a, BigInt * b);
int big_int_count_leading_zeros(BigInt *a);
BigInt big_int_copy(BigInt a);

size_t big_int_bit_length(BigInt *a);
unsigned char * big_int_to_byte_array_be(BigInt * a, size_t * len);
```

---

## TODO

- Optimize BigInt prime generation
- Add test suite with edge cases

---

## References

- RSA implementation based on the original RSA paper:  
  [Rivest, Shamir, and Adleman (1978) - "A Method for Obtaining Digital Signatures and Public-Key Cryptosystems"](https://people.csail.mit.edu/rivest/Rsapaper.pdf)

- Arbitrary-precision arithmetic inspired by `tiny-bignum-c`:  
  [https://github.com/kokke/tiny-bignum-c](https://github.com/kokke/tiny-bignum-c)

- Knuth's division (Algorithm D) based on Hacker's Delight:  
  [https://github.com/hcs0/Hackers-Delight](https://github.com/hcs0/Hackers-Delight)

- Primality testing using the Miller–Rabin algorithm:  
  [https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test](https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test)




---

## License

This project is licensed under the [MIT License](./LICENSE).

---

> Made with ❤️ and a lot of `malloc()` + `free()` debugging.

