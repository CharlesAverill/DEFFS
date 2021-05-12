/*
* FILENAME: arguments.c
*
* DESCRIPTION: Argparse parsing for DEFFS
*
* AUTHOR: Charles Averill
*/

#include "arguments.h"

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key) {
    case ARGP_KEY_ARG:
        // Too many arguments, if your program expects only one argument.
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
