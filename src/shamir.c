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

        // To keep the random values
        // in range not too high
        // we are taking mod with a
        // prime number around 1000
        p = (rand() % 997);

        // This is to ensure we did not
        // create a polynomial consisting
        // of zeroes.
        poly[j] = p;
    }

    // Generating N points from the
    // polynomial we created
    for (int j = 1; j <= N; ++j) {
        int x = j;
        int y = calculate_Y(x, poly);

        // Points created on sharing
        points[j - 1] = {x, y};
    }
}
