#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <string.h>
#include <stdio.h>

typedef struct CIPHER_PARAMS
{
    unsigned char *key;
    unsigned char *iv;
    const EVP_CIPHER *cipher;
} CIPHER_PARAMS;

int encrypt_data(char *plaintext, int plaintext_len, char *ciphertext, CIPHER_PARAMS *params);
int decrypt_data(char *ciphertext, int ciphertext_len, char *plaintext, CIPHER_PARAMS *params);

void get_sha256_hash(const char *plaintext, char obuf[SHA256_DIGEST_LENGTH]);

#endif
