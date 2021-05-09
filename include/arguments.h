#include <argp.h>
#include <stdbool.h>

struct arguments {
    char *points[2];
};

error_t parse_opt(int key, char *arg, struct argp_state *state);
