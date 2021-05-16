#ifndef SHAMIR_H
#define SHAMIR_H

typedef struct {
    int x;
    int y;
} point;

int get_y(int x, int *coefficients);
void encode_fragments(int secret, int n_fragments, int n_required, struct point *points);
int decode_fragments(struct point *points);

#endif
