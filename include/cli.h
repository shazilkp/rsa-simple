#include "bigint.h"

void read_rsa_key(const char *filename, size_t *mod_len, size_t *exp_len,BigInt * mod, BigInt * exp);
int cli_encrypt(char *key_file,char * input_file, char * output_file);
int cli_decrypt(char *key_file,char * input_file, char * output_file);
int cli_generate(size_t pq_size,char * pubkey_out, char * privkey_out);