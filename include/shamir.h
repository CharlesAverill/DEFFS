#ifndef SHAMIR_H
#define SHAMIR_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

typedef struct pair {
	unsigned long long int a;
	unsigned long long int b;
} pair;

void get_shares(unsigned long long int secret, int n_shares, int n_required, struct pair shares[]);
unsigned long long int get_secret(struct pair shares[], int n_shares);

#endif
