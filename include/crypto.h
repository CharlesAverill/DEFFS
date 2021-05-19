#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"

typedef struct EncryptionData {
    char key[17];
    unsigned char *plaintext;
    unsigned char *ciphertext;
} EncryptionData;

struct EncryptionData *get_ciphertext(char plaintext[]);
struct EncryptionData *get_ciphertext_with_key(char *plaintext, unsigned char key[16]);
struct EncryptionData *get_plaintext(char ciphertext[], unsigned char key[16]);

struct EncryptionData *get_encrypted_shards(char *plaintext);
void get_sha256_hash(char *plaintext, char *obuf);

#endif
