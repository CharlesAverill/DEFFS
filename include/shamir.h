#ifndef SHAMIR_H
#define SHAMIR_H

#include <sys/random.h>

typedef struct pair{
    int x;
    int y;
} pair;

int get_y(int x, int *coefficients);
void encode_fragments(int secret, int n_fragments, int n_required, struct pair *points);
int decode_fragments(struct pair *points, int num_points);

#endif
