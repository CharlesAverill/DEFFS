#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/random.h>

#include "deffs.h"

const char *deffs_path_prepend(const char originalPath[], char to_prepend[]);
void random_string(char output[], int length);
int mkdir_if_not_exists(char path[], mode_t mode);
int ends_with(const char str[], const char suffix[]);
int starts_with(const char str[], const char prefix[]);

#endif
