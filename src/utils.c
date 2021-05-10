/*
* FILENAME: utils.c
*
* DESCRIPTION: Necessary utilities for DEFFS
*
* AUTHOR: Charles Averill
*/

#include "utils.h"

const char *deffs_path_prepend(const char *originalPath, char *mountpoint){
    char *pch;
    pch = strstr(originalPath, mountpoint);
    if(pch == NULL){
        char newPath[strlen(originalPath) + strlen(mountpoint)];
        strcpy(newPath, mountpoint);
        strcat(newPath, originalPath);

        originalPath = newPath;
    }

    return originalPath;
}

void random_string(char *output, int length){
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-";

    for(int i = 0; i < length; i++){
        output[i] = charset[rand() % (sizeof(charset) - 1)];
    }

    output[length] = 0;
}
