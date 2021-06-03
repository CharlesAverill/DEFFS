/*
* FILENAME: utils.c
*
* DESCRIPTION: Necessary utilities for DEFFS
*
* AUTHOR: Charles Averill
*/

#include "utils.h"

void split_into_shards(char *in, char *out[], int n)
{
    // Fill out with n chunks of characters from in
    int shard_len = (strlen(in) / n) + 1;
    for (int i = 0; i < n; i++) {
        int offset = shard_len * i;
        out[i]     = malloc(shard_len);
        strncpy(out[i], in + offset,
                offset + shard_len < strlen(in) ? shard_len : strlen(in) - offset);
        out[i][shard_len] = '\0';
    }
}

const char *deffs_path_prepend(const char originalPath[], char to_prepend[])
{
    // Prepend to_prepend to originalPath
    char *pch;
    pch = strstr(originalPath, to_prepend);
    if (pch == NULL) {
        char newPath[strlen(originalPath) + strlen(to_prepend)];
        strcpy(newPath, to_prepend);
        strcat(newPath, originalPath);

        originalPath = newPath;
    }
}

void random_string(char output[], int length)
{
    // Fill output with random string of len length
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-";

    unsigned int tmp_rand;

    for (int i = 0; i < length; i++) {
        getrandom(&tmp_rand, sizeof(unsigned int), GRND_NONBLOCK);
        output[i] = charset[tmp_rand % (sizeof(charset) - 1)];
    }

    output[length] = '\0';
}

int mkdir_if_not_exists(char path[], mode_t mode)
{
    // Might add checks to this later
    return mkdir(path, mode);
    ;
}

int ends_with(const char str[], const char suffix[])
{
    size_t str_len    = strlen(str);
    size_t suffix_len = strlen(suffix);

    return (str_len >= suffix_len) && (!memcmp(str + str_len - suffix_len, suffix, suffix_len));
}

int starts_with(const char str[], const char prefix[])
{
    size_t str_len    = strlen(str);
    size_t prefix_len = strlen(prefix);

    return (str_len >= prefix_len) && (!memcmp(prefix, str, prefix_len));
}
