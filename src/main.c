#include "cli.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: rsa-simple <command> [options]\n");
        return 1;
    }

    char *command = argv[1];
    char *key_file = NULL;
    char *input_file = NULL;
    char *output_file = NULL;

    if (strcmp(command, "--help") == 0 || strcmp(command, "-h") == 0) {
        printf("Usage: rsa-simple <command> [options]\n\n");
        printf("Commands:\n");
        printf("  keygen [bits] [pubfile] [privfile]   Generate RSA key pair\n");
        printf("                                       (2048 bits by default)\n");
        printf("                                       (output filenames optional)\n\n");
        printf("  encrypt --pub PUBKEY -i INFILE -o OUTFILE\n");
        printf("                                       Encrypt file using public key\n\n");
        printf("  decrypt --priv PRIVKEY -i INFILE -o OUTFILE\n");
        printf("                                       Decrypt file using private key\n");
        return 0;
    }
    else if (strcmp(command, "keygen") == 0) {
        int bits = 2048;
        char *pub_file = "pubkey.txt";
        char *priv_file = "privkey.txt";

        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "--bits") == 0 && i + 1 < argc) {
                bits = atoi(argv[++i]);
            } else if (strcmp(argv[i], "--pub") == 0 && i + 1 < argc) {
                pub_file = argv[++i];
            } else if (strcmp(argv[i], "--priv") == 0 && i + 1 < argc) {
                priv_file = argv[++i];
            } else {
                fprintf(stderr, "Unknown or incomplete argument: %s\n", argv[i]);
                return 1;
            }
        }
        cli_generate(bits / 2, pub_file, priv_file);
    } else if (strcmp(command, "encrypt") == 0) {
            // Simple loop for key-value pairs
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "--pub") == 0 && i + 1 < argc) {
                key_file = argv[++i];
            } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
                input_file = argv[++i];
            } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                output_file = argv[++i];
            } else {
                fprintf(stderr, "Unknown or incomplete argument: %s\n", argv[i]);
                return 1;
            }
        }
        cli_encrypt(key_file,input_file,output_file);
    } else if (strcmp(command, "decrypt") == 0) {
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "--priv") == 0 && i + 1 < argc) {
                key_file = argv[++i];
            } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
                input_file = argv[++i];
            } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                output_file = argv[++i];
            } else {
                fprintf(stderr, "Unknown or incomplete argument: %s\n", argv[i]);
                return 1;
            }
        }
        cli_decrypt(key_file,input_file,output_file);
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }

    return 0;
}