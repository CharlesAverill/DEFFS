/*
* FILENAME: arguments.c
*
* DESCRIPTION: Argparse parsing for DEFFS
*
* AUTHOR: Charles Averill
*/

#include "arguments.h"
#include "deffs.h"

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key) {
    case 'n':
        arguments->n_machines = (int)strtol(arg, NULL, 10);
        if (arguments->n_machines < 1)
            argp_error(state, "n_machines=%d is invalid, must be greater than 0",
                       arguments->n_machines);
        break;
    case 'p':
        arguments->port = (int)strtol(arg, NULL, 10);
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num > 2)
            argp_usage(state);
        arguments->points[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 2)
            argp_usage(state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}
