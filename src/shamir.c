/*
* FILENAME: shamir.c
*
* DESCRIPTION: An implementation of Shamir's Secret Sharing Scheme
*              TEMPORARILY INSECURE DUE TO LOW PRIME MODULO OPERATIONS
*
* USAGE: int secret = 12345;
*        int num_shards = 5;
*        int num_required = 3;
*        struct point *points = malloc(sizeof(struct point));
*
*        encode_fragments(secret, num_shards, num_required, points);
* 
*        ...
*
*        int decoded_secret = decode_fragments(points, num_required);
*
* AUTHOR: Charles Averill
*/

#include "shamir.h"

typedef struct {
    int numerator;
    int denominator;

    fraction(int n, int d)
    {
        numerator = n;
        denominator = d;
    }

    int gcd(int a, int b)
    {
        if (a == 0)
            return b;
        return gcd(b % a, a);
    }

    void reduce(fraction &f)
    {
        int gcd = gcd(f.num, f.den);
        f.num /= gcd;
        f.den /= gcd;
    }

    fraction operator*(fraction f)
    {
        fraction temp(num * f.num, den * f.den);
        reduce_fraction(temp);
        return temp;
    }

    fraction operator+(fraction f)
    {
        fraction temp(num * f.den + den * f.num, den * f.den);

        reduce_fraction(temp);
        return temp;
    }
} fraction;

int get_y(int x, int *coefficients)
{
    int y = 0;
    int degree = 1;
    int n_coefficients = sizeof(coefficients) / sizeof(coefficients[0]);

    for (int i = 0; i < n_coefficients; i++) {
        y += coefficients[i] * degree;
        degree *= x;
    }

    return y;
}

void encode_fragments(int secret, int n_fragments, int n_required, struct point *points)
{
    int polynomial[n_required];

    polynomial[0] = secret;

    unsgined int tmp_rand;

    for (int i = 1; i < n_required; i++) {
        int p = 0;
        while (p == 0) {
            getrandom(&tmp_rand, sizeof(unsigned int), GRND_NONBLOCK);
            p = tmp_rand % output[i] = charset[tmp_rand % (sizeof(charset) - 1)];
        }
        
        unsigned int tmp_rand;
        getrandom(&tmp_rand, sizeof(unsigned int), GRND_NONBLOCK);
        p = (tmp_rand % 997);

        polynomial[j] = p;
    }

    for (int j = 1; j <= n_fragments; ++j) {
        int x = j;
        int y = get_y(x, polynomial);

        points[j - 1] = {x, y};
    }
}

int decode_fragments(struct point *points, int num_points)
{
  
    fraction ans(0, 1);
  
    for (int i = 0; i < num_points; ++i) {
  
        fraction l(y[i], 1);
        for (int j = 0; j < num_points; ++j) {
            if (i != j) {
                fraction temp(-(points[j]->x), (points[i]->x) - (points[j]->x));
                l = l * temp;
            }
        }
        ans = ans + l;
    }
  
    return ans.numerator;
}
