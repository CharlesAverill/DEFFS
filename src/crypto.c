/*
* FILENAME: crypto.c
*
* DESCRIPTION: A collection of cryptography methods to utilize OpenSSL's
*              AES encryption of strings
*
* USAGE: char *plaintext = "Hello World!";
*        char ciphertext[128];
*        char decrypted[128];
*        
*        CIPHER_PARAMS *params = (CIPHER_PARAMS*)malloc(sizeof(CIPHER_PARAMS));
*        params->cipher = EVP_aes_128_cbc();
*        // You can specify the key and iv by changing the values, 
*        // or randomly assign values to them by leaving them NULL.
*
*        int ciphertext_len = encrypt_data(plaintext, strlen(plaintext), ciphertext, params);
*        printf("Cipher text: --%s--\n", ciphertext);
*
*        int decrypted_len = decrypt_data(ciphertext, ciphertext_len, decrypted, params);
*        printf("Plain text: --%s--\n", decrypted);
*
* AUTHOR: Sidh Suchdev
*/

#include "crypto.h"

int encrypt_data(char *plaintext, int plaintext_len, char *ciphertext, CIPHER_PARAMS *params)
{
    // Initialize cipher context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    // If allocation failed, return
    if (ctx == NULL) {
        return -1;
    }
    EVP_CIPHER_CTX_init(ctx);

    // If params are not initialized, return
    if (params == NULL) {
        return -1;
    }

    // If the cipher is not specified, return
    if (params->cipher == NULL) {
        return -1;
    }

    // Determine key and iv length based on cipher type
    int key_length = EVP_CIPHER_key_length(params->cipher);
    int iv_length = EVP_CIPHER_iv_length(params->cipher);

    // If iv is not specified, randomly assign one
    if (params->iv == NULL) {
        params->iv = (char *)malloc(sizeof(unsigned char) * iv_length);
        if (params->iv == NULL) {
            return -1;
        }
        RAND_bytes(params->iv, iv_length);
    }
    // If key is not specified, randomly specify one
    if (params->key == NULL) {
        params->key = (char *)malloc(sizeof(unsigned char) * key_length);
        if (params->key == NULL) {
            return -1;
        }
        RAND_bytes(params->key, key_length);
    }

    // Configure context to encrypt based on cipher
    EVP_EncryptInit(ctx, params->cipher, params->key, params->iv);

    // Encrypt data and finding ciphertext length
    int outl;
    int ciphertext_len;
    EVP_EncryptUpdate(ctx, ciphertext, &outl, plaintext, plaintext_len);
    ciphertext_len = outl;
    EVP_EncryptFinal(ctx, ciphertext + outl, &outl);
    ciphertext_len += outl;

    // Freeing context
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt_data(char *ciphertext, int ciphertext_len, char *plaintext, CIPHER_PARAMS *params)
{
    // Initialize cipher variables
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        return -1;
    }
    EVP_CIPHER_CTX_init(ctx);

    // If the parameters aren't specified, return
    if (params == NULL || params->cipher == NULL || params->iv == NULL || params->key == NULL) {
        return -1;
    }

    // Configure context to decrypt based on cipher
    EVP_DecryptInit(ctx, params->cipher, params->key, params->iv);

    // Decrypt data and determine plaintext length
    int outl;
    int plaintext_len;
    EVP_DecryptUpdate(ctx, plaintext, &outl, ciphertext, ciphertext_len);
    plaintext_len = outl;
    EVP_DecryptFinal(ctx, plaintext + outl, &outl);
    plaintext_len += outl;

    // Free context
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
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
    obuf[SHA256_DIGEST_LENGTH] = '\0';
}

