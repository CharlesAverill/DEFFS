#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <string.h>
#include <stdio.h>

typedef struct AES_PARAMS
{
    unsigned char *key;
    unsigned char *iv;
    const EVP_CIPHER *cipher;
} AES_PARAMS;

int encrypt_data(char *plaintext, int plaintext_len, char *ciphertext, AES_PARAMS *params);
int decrypt_data(char *ciphertext, int ciphertext_len, char *plaintext, AES_PARAMS *params);

// struct EncryptionData *get_encrypted_shards(char *plaintext);
void get_sha256_hash(char *plaintext, char *obuf);

#endif
