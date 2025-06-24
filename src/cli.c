#include "rsa.h"
#include "cli.h"
#include <stdio.h>
#include <string.h>

void print_bytes(const unsigned char *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X", data[i]);  // uppercase hex, 2 digits per byte
        if ((i + 1) % 16 == 0) printf("\n"); // optional: wrap every 16 bytes
        else printf(" ");
    }
    printf("\n");
}
static const char base64_std_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(const unsigned char *data, size_t input_len, size_t *output_len) {
    size_t enc_len = 4 * ((input_len + 2) / 3);  // output length including padding
    char *encoded = malloc(enc_len + 1);
    if (!encoded) return NULL;

    size_t i = 0, j = 0;

    while (i < input_len) {
        size_t remain = input_len - i;

        uint32_t octet_a = data[i++];
        uint32_t octet_b = remain > 1 ? data[i++] : 0;
        uint32_t octet_c = remain > 2 ? data[i++] : 0;

        uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        encoded[j++] = base64_std_chars[(triple >> 18) & 0x3F];
        encoded[j++] = base64_std_chars[(triple >> 12) & 0x3F];
        encoded[j++] = (remain > 1) ? base64_std_chars[(triple >> 6) & 0x3F] : '=';
        encoded[j++] = (remain > 2) ? base64_std_chars[triple & 0x3F] : '=';
    }

    encoded[j] = '\0';
    if (output_len) *output_len = j;
    return encoded;
}


static const unsigned char base64_decode_table[256] = {
    ['A'] =  0, ['B'] =  1, ['C'] =  2, ['D'] =  3,
    ['E'] =  4, ['F'] =  5, ['G'] =  6, ['H'] =  7,
    ['I'] =  8, ['J'] =  9, ['K'] = 10, ['L'] = 11,
    ['M'] = 12, ['N'] = 13, ['O'] = 14, ['P'] = 15,
    ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
    ['Y'] = 24, ['Z'] = 25,
    ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29,
    ['e'] = 30, ['f'] = 31, ['g'] = 32, ['h'] = 33,
    ['i'] = 34, ['j'] = 35, ['k'] = 36, ['l'] = 37,
    ['m'] = 38, ['n'] = 39, ['o'] = 40, ['p'] = 41,
    ['q'] = 42, ['r'] = 43, ['s'] = 44, ['t'] = 45,
    ['u'] = 46, ['v'] = 47, ['w'] = 48, ['x'] = 49,
    ['y'] = 50, ['z'] = 51,
    ['0'] = 52, ['1'] = 53, ['2'] = 54, ['3'] = 55,
    ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
    ['8'] = 60, ['9'] = 61, ['+'] = 62, ['/'] = 63,
};

unsigned char *base64_decode(const char *input, size_t input_len, size_t *output_len) {
    //printf("add %d",input);
    //printf("input = %s \n",input);
    if (input_len % 4 != 0) return NULL; // invalid base64 length
    // printf("inputlen = %zu \n",input_len);
    
    size_t padding = 0;
    if (input_len >= 2 && input[input_len - 1] == '=') padding++;
    if (input_len >= 2 && input[input_len - 2] == '=') padding++;
    // printf("paddinf = %zu \n",padding);
    
    size_t decoded_len = (input_len / 4) * 3 - padding;
    // printf("decoded len= %zu \n",decoded_len);
    unsigned char *decoded = malloc(decoded_len);
    if (!decoded) return NULL;
    
    size_t i = 0, j = 0;
    while (i < input_len) {
        //printf("Hello\n");
        uint32_t sextet_a = input[i] == '=' ? 0 & i++ : base64_decode_table[(unsigned char)input[i++]];
        uint32_t sextet_b = input[i] == '=' ? 0 & i++ : base64_decode_table[(unsigned char)input[i++]];
        uint32_t sextet_c = input[i] == '=' ? 0 & i++ : base64_decode_table[(unsigned char)input[i++]];
        uint32_t sextet_d = input[i] == '=' ? 0 & i++ : base64_decode_table[(unsigned char)input[i++]];

        uint32_t triple = (sextet_a << 18)
                        | (sextet_b << 12)
                        | (sextet_c << 6)
                        | sextet_d;
        // printf("triplr = %u\n",triple);
        if (j < decoded_len) {
            decoded[j++] = (triple >> 16) & 0xFF;
            //printf("1.) %d\n", (unsigned char) decoded[j-1]);
        }
        if (j < decoded_len) {
            decoded[j++] = (triple >> 8) & 0xFF;
            //printf("2.) %d\n",decoded[j-1]);
        }
        if (j < decoded_len) {
            decoded[j++] = triple & 0xFF;
            //printf("3.) %d\n",decoded[j-1]);
        }
        
    }

    if (output_len) *output_len = decoded_len;


    return decoded;
}

// unsigned char * decode_pubkey(char * pubkeyfile){
    
// }

void read_rsa_key(const char *filename, size_t *mod_len, size_t *exp_len,BigInt * mod, BigInt * exp) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }

    char line[4096];
    int line_count = 0;
    unsigned char *modulus_arr;
    unsigned char *exponent_arr;
    char *mod_b64 = NULL, *exp_b64 = NULL;

    while (fgets(line, sizeof(line), f)) {
        // Remove newline
        line[strcspn(line, "\r\n")] = '\0';

        if (strstr(line, "BEGIN") || strstr(line, "END")) {
            continue; // skip header/footer
        }

        if (line_count == 0) {
            mod_b64 = strdup(line);
        } else if (line_count == 1) {
            exp_b64 = strdup(line);
        }
        line_count++;
    }

    fclose(f);

    if (!mod_b64 || !exp_b64) {
        fprintf(stderr, "Invalid key file format.\n");
        exit(EXIT_FAILURE);
    }
    // printf("mod = %s\n",mod_b64);

    modulus_arr = base64_decode(mod_b64, strlen(mod_b64), mod_len);
    exponent_arr = base64_decode(exp_b64, strlen(exp_b64), exp_len);

    

    // printf("modlen out = %zu \n",*mod_len);
    // printf("exolen out = %zu \n",*exp_len);

    // for (size_t i = 0; i < *mod_len; i++) {
    //     printf("%02X", modulus_arr[i]);  // print two-digit hex
    // }
    // printf("\n");


    // for (size_t i = 0; i < *exp_len; i++) {
    //     printf("%02X", exponent_arr[i]);  // print two-digit hex
    // }
    // printf("\n");

    BigInt modT = big_int_from_byte_array_be(modulus_arr,*mod_len);
    BigInt expT = big_int_from_byte_array_be(exponent_arr,*exp_len);

    *mod = big_int_copy(modT);
    *exp = big_int_copy(expT);

    free(mod_b64);
    free(exp_b64);
}


void read_encrypted_file(const char *filename, size_t *data_len,BigInt * data) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }

    char line[4096];
    int line_count = 0;
    unsigned char *data_arr;
    char *data_b64 = NULL;

    while (fgets(line, sizeof(line), f)) {
        // printf("hello");
        // printf("%s\n",line);
        // Remove newline
        line[strcspn(line, "\r\n")] = '\0';

        if (strstr(line, "BEGIN") || strstr(line, "END")) {
            continue; // skip header/footer
        }

        data_b64 = strdup(line);
        line_count++;
    }

    fclose(f);

    if (!data_b64) {
        fprintf(stderr, "Invalid encrypted file format.\n");
        exit(EXIT_FAILURE);
    }
    

    data_arr = base64_decode(data_b64, strlen(data_b64), data_len);

    // printf("data len = %zu\n",*data_len)

    // printf("modlen out = %zu \n",*mod_len);
    // printf("exolen out = %zu \n",*exp_len);

    // for (size_t i = 0; i < *mod_len; i++) {
    //     printf("%02X", modulus_arr[i]);  // print two-digit hex
    // }
    // printf("\n");


    // for (size_t i = 0; i < *exp_len; i++) {
    //     printf("%02X", exponent_arr[i]);  // print two-digit hex
    // }
    // printf("\n");

    BigInt dataBigInt = big_int_from_byte_array_be(data_arr,*data_len);

    *data = big_int_copy(dataBigInt);

    free(data_b64);
}

void write_pubkey(unsigned char *modulus_bytes, size_t mod_len,
                   unsigned char *exponent_bytes, size_t exp_len,
                   const char *output_file) {
    FILE *f = fopen(output_file, "w");
    if (!f) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "-----BEGIN RSA PUBLIC KEY-----\n");

    char *mod_b64 = base64_encode(modulus_bytes, mod_len, NULL);
    char *exp_b64 = base64_encode(exponent_bytes, exp_len, NULL);

    if (!mod_b64 || !exp_b64) {
        fprintf(stderr, "Base64 encoding failed.\n");
        fclose(f);
        free(mod_b64);
        free(exp_b64);
        exit(EXIT_FAILURE);
    }

    fprintf(f, "%s\n", mod_b64);
    fprintf(f, "%s\n", exp_b64);
    fprintf(f, "-----END RSA PUBLIC KEY-----\n");

    fclose(f);
    free(mod_b64);
    free(exp_b64);
}


void write_privkey(unsigned char *modulus_bytes, size_t mod_len,
                   unsigned char *exponent_bytes, size_t exp_len,
                   const char *output_file) {
    FILE *f = fopen(output_file, "w");
    if (!f) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "-----BEGIN RSA PRIVATE KEY-----\n");

    char *mod_b64 = base64_encode(modulus_bytes, mod_len, NULL);
    char *exp_b64 = base64_encode(exponent_bytes, exp_len, NULL);

    if (!mod_b64 || !exp_b64) {
        fprintf(stderr, "Base64 encoding failed.\n");
        fclose(f);
        free(mod_b64);
        free(exp_b64);
        exit(EXIT_FAILURE);
    }

    fprintf(f, "%s\n", mod_b64);
    fprintf(f, "%s\n", exp_b64);
    fprintf(f, "-----END RSA PRIVATE KEY-----\n");

    fclose(f);
    free(mod_b64);
    free(exp_b64);
}



int cli_encrypt(char *key_file,char * input_file, char * output_file){
    FILE *filePtr,*keyptr;
    unsigned char  *fileBuffer;
    long filelen,keylen;

    filePtr = fopen(input_file, "rb");  // Open the file in binary mode
    fseek(filePtr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(filePtr);             // Get the current byte offset in the file
    rewind(filePtr);                      // Jump back to the beginning of the file

    fileBuffer = (unsigned char *)malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(fileBuffer, 1, filelen, filePtr); // Read in the entire file
    fclose(filePtr);

  

    BigInt input = big_int_from_byte_array_be(fileBuffer,(size_t)filelen);


    BigInt mod,result;
    BigInt exp;
    size_t mod_len,exp_len;

    read_rsa_key(key_file,&mod_len,&exp_len,&mod,&exp);      //convert punlic key(e,n) into BigInt

    encrypt(&input,&exp,&mod,&result);                          //encrypt as BigInt



    size_t result_arr_len;
    unsigned char * result_array = big_int_to_byte_array_be(&result,&result_arr_len);   //bigInt to  byte array

  
    size_t b64_result_len;
    char * b64_result = base64_encode(result_array,result_arr_len,&b64_result_len);     //byte array encode to base64

    

    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("Failed to open output file");
        free(result_array);
        free(b64_result);
        exit(EXIT_FAILURE);
    }
    fprintf(out, "-----BEGIN RSA CIPHERTEXT-----\n");
    fprintf(out, "%s\n", b64_result);
    fprintf(out, "-----END RSA CIPHERTEXT-----\n");

    fflush(out);
    fclose(out);

    return 1;
}

int cli_decrypt(char *key_file,char * input_file, char * output_file){
    size_t input_len;
    BigInt data = {NULL, 0 , 0};
    read_encrypted_file(input_file,&input_len,&data);       //base64 encoded encrypted file decoded and made BigInt

    //  return 0;
    BigInt n,key;
    size_t n_len,key_len;
    read_rsa_key(key_file,&n_len,&key_len,&n,&key);        //base64 encoded key file decoded and made BigInt

    BigInt result;
    size_t result_len;

    decrypt(&data,&key,&n,&result);                         //got bigInt of encrypted data



    unsigned char * result_arr =  big_int_to_byte_array_be(&result,&result_len);


    FILE *f = fopen(output_file, "wb");
    if (!f) {
        perror("Output File open failed");
        exit(EXIT_FAILURE);
    }

    fwrite(result_arr, 1, result_len, f);
    fclose(f);
    return 1;

}

int cli_generate(size_t pq_size,char * pubkey_out, char * privkey_out){
    BigInt modulus = {NULL,0,0};
    BigInt privatekey = {NULL,0,0};
    BigInt publickey = {NULL,0,0};

    generate(&modulus,&publickey,&privatekey,pq_size);
    // printf("mod = ");
	// big_int_print(&modulus,2);

	// printf("exp = ");
	// big_int_print(&publickey,2);
    size_t mod_byte_len,pubkey_byte_len,privkey_byte_len;
    unsigned char * mod_byte = big_int_to_byte_array_be(&modulus,&mod_byte_len);
    for (size_t i = 0; i < mod_byte_len; i++) {
        printf("%02X", mod_byte[i]);  // print two-digit hex
    }
    printf("\n");
    unsigned char * pubkey_byte = big_int_to_byte_array_be(&publickey,&pubkey_byte_len);
    unsigned char * privkey_byte = big_int_to_byte_array_be(&privatekey,&privkey_byte_len);

    // char * b64_mod = base64_encode(mod_byte,mod_byte_len,NULL);
    // char * b64_pubkey = base64_encode(mod_byte,mod_byte_len,NULL);
    // char * b64_privkey = base64_encode(mod_byte,mod_byte_len,NULL);

    write_pubkey(mod_byte,mod_byte_len,pubkey_byte,pubkey_byte_len,pubkey_out);
    write_privkey(mod_byte,mod_byte_len,privkey_byte,privkey_byte_len,privkey_out);
    
    return 1;
}

// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         fprintf(stderr, "Usage: rsa-simple <command> [options]\n");
//         return 1;
//     }

//     char *command = argv[1];
//     char *key_file = NULL;
//     char *input_file = NULL;
//     char *output_file = NULL;

//     if (strcmp(command, "keygen") == 0) {
//         int bits = (argc >= 3) ? atoi(argv[2]) : 2048;
//         rsa_keygen(bits);
//     } else if (strcmp(command, "encrypt") == 0) {
//             // Simple loop for key-value pairs
//         for (int i = 2; i < argc; i++) {
//             if (strcmp(argv[i], "--pub") == 0 && i + 1 < argc) {
//                 key_file = argv[++i];
//             } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
//                 input_file = argv[++i];
//             } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
//                 output_file = argv[++i];
//             } else {
//                 fprintf(stderr, "Unknown or incomplete argument: %s\n", argv[i]);
//                 return 1;
//             }
//         }
//         cli_encrypt(key_file,input_file,output_file);
//     } else if (strcmp(command, "decrypt") == 0) {
//         // parse options like -k, -i, -o
//     } else {
//         fprintf(stderr, "Unknown command: %s\n", command);
//         return 1;
//     }

//     return 0;
// }

