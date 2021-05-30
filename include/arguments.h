#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <argp.h>
#include <stdbool.h>

struct arguments {
    int n_machines;
    char *points[2];
};

error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif
