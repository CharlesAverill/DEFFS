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
    unsigned char key[17];
    unsigned char ciphertext[16 * ((strlen(plaintext) + 15) / 16)];

    printf("Plaintext: %s Len: %zu\n", plaintext, strlen(plaintext));

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

    // Fill the key with random values
    random_string(key, 17);

    AES_set_encrypt_key((const unsigned char *)key, 128, &AES_key);

    // Encrypt plaintext
    AES_encrypt(plaintext, ciphertext, &AES_key);

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

struct EncryptionData *get_ciphertext_with_key(char *plaintext, unsigned char key[16])
{
    // Initialize cipher variables
    AES_KEY AES_key;
    unsigned char ciphertext[16 * ((strlen(plaintext) + 15) / 16)];

    key[0] = 1;
    for (int i = 1; i <= 15; i++) {
        key[i] = 0;
    }

    AES_set_encrypt_key((const unsigned char *)key, 128, &AES_key);

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

    // Encrypt plaintext
    AES_encrypt(plaintext, ciphertext, &AES_key);

    // Assign values to returned struct
    strcpy(output->key, key);

    output->ciphertext = malloc(strlen(ciphertext));
    if (output->ciphertext == NULL) {
        free(output);
        return NULL;
    }

    strcpy(output->ciphertext, ciphertext);

    return output;
}

struct EncryptionData *get_plaintext(char ciphertext[], unsigned char key[16])
{
    // Initialize cipher variables
    AES_KEY AES_key;
    unsigned char plaintext[strlen(ciphertext)];

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

EncryptionData *get_encrypted_shards(char *plaintext)
{
    // Encrypt plaintext
    struct EncryptionData *cipher = get_ciphertext(plaintext);

    return cipher;
}

void get_sha256_hash(char *plaintext, char *obuf)
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
    obuf[64] = '\0';
}
