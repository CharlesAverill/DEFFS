#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <string.h>
#include <stdio.h>

typedef struct EncryptionData {
    AES_KEY key;

    unsigned char *plaintext;
    unsigned char *ciphertext;
} EncryptionData;

struct EncryptionData *get_ciphertext(char plaintext[]);
struct EncryptionData *get_plaintext(char ciphertext[], unsigned char key[16]);

EncryptionData *get_encrypted_shards(char *plaintext);

#endif
