#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <string.h>

typedef struct EncryptionData {
    AES_KEY AES_key;
    unsigned char key[128];

    unsigned char *plaintext;
    unsigned char *ciphertext;
} EncryptionData;

struct EncryptionData *get_ciphertext(char plaintext[]);
struct EncryptionData *get_plaintext(char ciphertext[], unsigned char key[16]);

#endif
