/*
 * Monte Carlo method to calculate pi.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define sqr(a)  a*a

/* calculate pi */
double pi(double tolerance, unsigned long samples) {

    double x, y, value, error;
    unsigned long sampled = 0, hit = 0, i;

    do {
        /* max loops are samples, no error checking everytime we loop */
        for (i = samples; i; i--, sampled ++) {
            x = rand() / (RAND_MAX + 1.0);
            y = rand() / (RAND_MAX + 1.0);
            if (sqr(x) + sqr(y) < 1) {
                hit++;
            }
        }

        value = (double) hit / sampled;
        error = value * sqrt(value * (1 - value) / sampled) * 4;
        value *= 4;

        fprintf(stderr, "Pi = %f +/- %5.3e at %ldM samples.\r",
                value, error, sampled / samples);

        /* if hit is fails rand() is broken */
    } while (!hit || error > tolerance);
    return value;
}

int main() {
    double tolerance = 2e-6;
    unsigned long samples = 1000000;
    printf("Pi is %f\n ,with a tolerance of %f.", pi(tolerance, samples), tolerance);
    return 0;
}
