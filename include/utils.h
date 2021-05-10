#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const char *deffs_path_prepend(const char *originalPath, char *mountpoint);

void random_string(char *output, int length);

#endif
