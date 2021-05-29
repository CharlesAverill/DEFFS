/*
* FILENAME: shamir.c
*
* DESCRIPTION: Implementation of Shamir's Secret Sharing Scheme based on
*              Wikipedia's Python implementation: https://en.wikipedia.org/wiki/Shamir%27s_Secret_Sharing#Python_example
*
* USAGE: unsigned long long int secret = 65;
*        int num_shards                = 4;
*        int num_required              = 3;
*
*        struct pair shares[num_shards];
*        get_shares(secret, num_shards, num_required, shares);
*
*        for (int i = 0; i < num_shards; i++) {
*            printf("X: %lld, Y: %lld\n", shares[i].a, shares[i].b);
*        }
*
*        unsigned long long int recovered_secret = get_secret(shares, num_shards);
*
*        printf("Secret: %lld\nRecovered Secret: %lld\n", secret, recovered_secret);
*
* AUTHOR: Charles Averill
*/

#include "shamir.h"

unsigned long long int _PRIME = 2305843009213693951; // 2 ^ 61 - 1

unsigned long long int _get_y(unsigned long long int x, unsigned long long int coefficients[],
                              int n_coefficients)
{
    unsigned long long int y = 0;

    for (int i = n_coefficients - 1; i >= 0; i--) {
        y *= x;
        y += coefficients[i];
        y %= _PRIME;
    }

    return y;
}

unsigned long long int _extended_gcd(unsigned long long int a, unsigned long long int b)
{
    unsigned long long int x      = 0;
    unsigned long long int y      = 1;
    unsigned long long int last_x = 1;
    unsigned long long int last_y = 0;

    while (b != 0) {
        // clang-format off
        long long int quotient = a / b;
        printf("%lld %lld %lld\n", a, b, quotient);
        
        unsigned long long int temp = b;
        b = a % b;
        a = temp;
        
        temp = x;
        x = last_x - quotient * x;
        last_x = temp;
        
        temp = y;
        y = last_y - quotient * y;
        last_y = temp;
        // clang-format on
    }

    return last_x;
}

unsigned long long int _divmod(unsigned long long int numerator, unsigned long long int denominator)
{
    unsigned long long int inverse = _extended_gcd(denominator, _PRIME);
    return inverse * numerator;
}

unsigned long long int _product_of_inputs(unsigned long long int values[], int n_values)
{
    unsigned long long int product = 1;
    for (int i = 0; i < n_values; i++) {
        product *= values[i];
    }
    return product;
}

unsigned long long int _lagrange_interpolate(unsigned long long int x, struct pair points[], int k)
{
    unsigned long long int numerators[k];
    unsigned long long int denominators[k];

    for (int i = 0; i < k; i++) {
        unsigned long long int temp_numerators[k - 1];
        unsigned long long int temp_denominators[k - 1];

        for (int j = 0; j < k - 1; j++) {
            int pop_j            = j < i ? j : j + 1;
            temp_numerators[j]   = x - points[pop_j].a;
            temp_denominators[j] = points[i].a - points[pop_j].a;
        }

        numerators[i]   = _product_of_inputs(temp_numerators, k - 1);
        denominators[i] = _product_of_inputs(temp_denominators, k - 1);
    }

    unsigned long long int denominator = _product_of_inputs(denominators, k);
    unsigned long long int numerator   = 0;
    for (int i = 0; i < k; i++) {
        numerator += _divmod(numerators[i] * denominator * points[i].b % _PRIME, denominators[i]);
    }

    return (_divmod(numerator, denominator) + _PRIME) % _PRIME;
}

void get_shares(unsigned long long int secret, int n_shares, int n_required, struct pair shares[])
{
    if (n_shares < n_required) {
        printf("n_shares must be >= n_required\n");
        exit(1);
    }

    unsigned long long int coefficients[n_required];
    coefficients[0] = secret;

    unsigned long long int tmp_rand;
    for (int i = 1; i < n_required; i++) {
        getrandom(&tmp_rand, sizeof(unsigned long long int), GRND_NONBLOCK);
        tmp_rand %= _PRIME - 1;

        coefficients[i] = tmp_rand;
    }

    for (int i = 1; i < n_shares + 1; i++) {
        shares[i - 1].a = i;
        shares[i - 1].b = _get_y(i, coefficients, n_required);
    }
}

unsigned long long int get_secret(struct pair shares[], int n_shares)
{
    return _lagrange_interpolate(0, shares, n_shares);
}
