/*
* FILENAME: shamir.c
*
* DESCRIPTION: An implementation of Shamir's Secret Sharing Scheme
*              TEMPORARILY INSECURE DUE TO LOW PRIME MODULO OPERATIONS
*
* USAGE: int secret = 12345;
*        int num_shards = 5;
*        int num_required = 3;
*        struct pair *points = malloc(sizeof(struct pair));
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

int gcd(int a, int b)
{
    if (a == 0)
        return b;
    return gcd(b % a, a);
}

void reduce(struct pair f)
{
    int _gcd = gcd(f.x, f.y);
    f.x /= _gcd;
    f.y /= _gcd;
}

struct pair multiply(struct pair first, struct pair second)
{
    pair temp;
    temp.x = first.x * second.x;
    temp.y = first.y * second.y;

    reduce(temp);
    return temp;
}

struct pair add(struct pair first, struct pair second)
{
    pair temp;
    temp.x = first.x * second.y + first.y * second.x;
    temp.y = first.y * second.y;

    reduce(temp);
    return temp;
}

int get_y(int x, int *coefficients)
{
    int y              = 0;
    int degree         = 1;
    int n_coefficients = sizeof(coefficients) / sizeof(coefficients[0]);

    for (int i = 0; i < n_coefficients; i++) {
        y += coefficients[i] * degree;
        degree *= x;
    }

    return y;
}

void encode_fragments(int secret, int n_fragments, int n_required, struct pair *points)
{
    int polynomial[n_required];

    polynomial[0] = secret;

    unsigned int tmp_rand;

    for (int i = 1; i < n_required; i++) {
        int p = 0;
        while (p == 0) {
            unsigned int tmp_rand;
            getrandom(&tmp_rand, sizeof(unsigned int), GRND_NONBLOCK);
            p = (tmp_rand % 997);
        }

        polynomial[i] = p;
    }

    for (int i = 1; i <= n_fragments; i++) {
        int x = i;
        int y = get_y(x, polynomial);

        points[i - 1].x = x;
        points[i - 1].y = y;
    }
}

int decode_fragments(struct pair *points, int num_points)
{

    pair ans;
    ans.x = 0;
    ans.y = 1;

    for (int i = 0; i < num_points; ++i) {

        pair l;
        l.x = points[i].y;
        l.y = 1;
        for (int j = 0; j < num_points; ++j) {
            if (i != j) {
                pair temp;
                temp.x = -1 * points[j].x;
                temp.y = points[i].x - points[j].x;

                l = multiply(l, temp);
            }
        }
        ans = add(ans, l);
    }

    return ans.x;
}
