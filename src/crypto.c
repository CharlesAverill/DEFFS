/*
* FILENAME: crypto.c
*
* DESCRIPTION: A collection of cryptography methods to utilize OpenSSL's
*              AES encryption of strings
*
* USAGE: char *plaintext = "Hello World!";
*
*        struct EncryptionData *cipher = get_ciphertext(plaintext);
*        printf("Cipher text: --%s--\n", cipher->ciphertext);
*
*        struct EncryptionData *plain = get_plaintext(cipher->ciphertext, cipher->key);
*        printf("Cipher text: --%s--\n", plain->plaintext);
*
* AUTHOR: Charles Averill
*/

#include "crypto.h"

struct EncryptionData *get_ciphertext(char *plaintext)
{
    // Initialize cipher variables
    AES_KEY AES_key;
    unsigned char key[SHARD_KEY_LEN];
    unsigned char ciphertext[16];

    // Extend plaintext if needed
    if (strlen(plaintext) < 15) {
        realloc(plaintext, 16);
    }

    // Allocate memory for returned struct
    struct EncryptionData *output = malloc(sizeof(struct EncryptionData));
    if (output == NULL) {
        return NULL;
    }

    // Set plaintext before any operations in case of mangling by OpenSSL
    output->plaintext = malloc(strlen(plaintext));
    if (output->plaintext == NULL) {
        free(output);
        return NULL;
    }
    strcpy(output->plaintext, plaintext);

    printf("Plaintext: %s\n", output->plaintext);

    // Fill the key with random values
    random_string(key, 17);

    AES_set_encrypt_key((const unsigned char *)key, 128, &AES_key);

    // Encrypt plaintext
    AES_encrypt(plaintext, ciphertext, &AES_key);

    printf("Ciphertext: %s\n", ciphertext);

    // Assign key and ciphertext to returned struct
    strcpy(output->key, key);

    output->ciphertext = malloc(strlen(ciphertext));
    if (output->ciphertext == NULL) {
        free(output);
        return NULL;
    }
    strcpy(output->ciphertext, ciphertext);

    return output;
}

struct EncryptionData *get_ciphertext_with_key(char *plaintext, unsigned char key[SHARD_KEY_LEN])
{
    // Initialize cipher variables
    AES_KEY AES_key;
    unsigned char ciphertext[16];

    // Allocate memory for returned struct
    struct EncryptionData *output = malloc(sizeof(struct EncryptionData));
    if (output == NULL) {
        return NULL;
    }

    // Set plaintext before any operations in case of mangling by OpenSSL
    output->plaintext = malloc(strlen(plaintext));
    if (output->plaintext == NULL) {
        free(output);
        return NULL;
    }
    strcpy(output->plaintext, plaintext);

    AES_set_encrypt_key((const unsigned char *)key, 128, &AES_key);

    // Encrypt plaintext
    AES_encrypt("bruh", ciphertext, &AES_key);

    // Assign key and ciphertext to returned struct
    strcpy(output->key, key);

    output->ciphertext = malloc(strlen(ciphertext));
    if (output->ciphertext == NULL) {
        free(output);
        return NULL;
    }
    strcpy(output->ciphertext, ciphertext);

    return output;
}

struct EncryptionData *get_plaintext(char ciphertext[], unsigned char key[SHARD_KEY_LEN])
{
    // Initialize cipher variables
    AES_KEY AES_key;
    unsigned char plaintext[16];

    AES_set_decrypt_key((const unsigned char *)key, 128, &AES_key);

    // Decrypt ciphertext
    AES_decrypt(ciphertext, plaintext, &AES_key);

    // Allocate memory for returned struct
    struct EncryptionData *output = malloc(sizeof(struct EncryptionData));
    if (output == NULL) {
        return NULL;
    }

    // Assign values to returned struct
    strcpy(output->key, key);

    output->plaintext = malloc(strlen(plaintext));
    if (output->plaintext == NULL) {
        free(output);
        return NULL;
    }

    strcpy(output->plaintext, plaintext);

    return output;
}

void get_sha256_hash(const char *plaintext, char obuf[SHA256_DIGEST_LENGTH])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    SHA256_Update(&sha256, plaintext, strlen(plaintext));
    SHA256_Final(hash, &sha256);

    int i = 0;
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(obuf + (i * 2), "%02x", hash[i]);
    }
    obuf[SHA256_DIGEST_LENGTH] = '\0';
}
