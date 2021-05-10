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

struct EncryptionData *get_ciphertext(char *plaintext){
    // Initialize cipher variables
    AES_KEY AES_key;
    unsigned char key[16];
    unsigned char ciphertext[(strlen(plaintext) + 127) / 128];

    key[0]=1;
    for(int i = 1; i <= 15; i++) {
        key[i] = 0;
    }

    AES_set_encrypt_key((const unsigned char *) key, 128, &AES_key);

    // Encrypt plaintext
    AES_encrypt(plaintext, ciphertext, &AES_key);

    // Allocate memory for returned struct
    struct EncryptionData *output = malloc(sizeof(struct EncryptionData));
    if(output == NULL){
        return NULL;
    }

    // Assign values to returned struct
    output->key = AES_key;

    output->plaintext = malloc(strlen(plaintext));
    if (output->plaintext == NULL) {
        free (output);
        return NULL;
    }

    output->ciphertext = malloc(strlen(ciphertext));
    if (output->ciphertext == NULL) {
        free (output);
        return NULL;
    }

    strcpy(output->plaintext, plaintext);
    strcpy(output->ciphertext, ciphertext);

    return output;
}


struct EncryptionData *get_plaintext(char ciphertext[], unsigned char key[16]){
    // Initialize cipher variables
    AES_KEY AES_key;
    unsigned char plaintext[strlen(ciphertext)];

    AES_set_decrypt_key((const unsigned char *) key, 128, &AES_key);

    // Decrypt ciphertext
    AES_decrypt(ciphertext, plaintext, &AES_key);

    // Allocate memory for returned struct
    struct EncryptionData *output = malloc(sizeof(struct EncryptionData));
    if(output == NULL){
        return NULL;
    }

    // Assign values to returned struct
    output->key = AES_key;

    output->plaintext = malloc(strlen(plaintext));
    if (output->plaintext == NULL) {
        free (output);
        return NULL;
    }

    output->ciphertext = malloc(strlen(ciphertext));
    if (output->ciphertext == NULL) {
        free (output);
        return NULL;
    }

    strcpy(output->plaintext, plaintext);
    strcpy(output->ciphertext, ciphertext);

    return output;
}

EncryptionData *get_encrypted_shards(char *plaintext){
    // Encrypt plaintext
    struct EncryptionData *cipher = get_ciphertext(plaintext);

    return cipher;
}
