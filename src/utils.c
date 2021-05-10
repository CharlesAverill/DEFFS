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
