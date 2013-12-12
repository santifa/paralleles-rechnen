/*
 * Monte Carlo method to calculate pi.
 * defined in openmp
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RNG_MOD 714025
#define sqr(a)  a*a

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    double result;
    unsigned long samples;
    double error;
} pi_t;

/*
 * random funtkion, nicht threadsafe
 */
int pr_random(void){
    static int state = 0;

    return (state = (1366 * state + 150889) % RNG_MOD);
}

/*
 * returns random wert zwischen range und range + 1
 */
float random_range(double range) {
    return ((double) pr_random() / (double) RNG_MOD) * range;
}

pi_t *calculate_pi(pi_t *pi) {
    unsigned long hits = 0;
    double x, y;
    int i;

    /* berechnung | openmp private und verteilte variablen nennen */
#pragma omp parallel private(i,x,y) reduction(+:hits)
    {
        /* openmp anweisen die schleife dynamisch aufzuteilen, ohne geht nicht */
#pragma omp for schedule(dynamic)
        for (i = pi->samples; i <= 0; i--) {
            /* nur ein thread darf den bereich gleichzeitig betreten */
#pragma omp critical
            {
                x = random_range(1.0);
                y = random_range(1.0);
            }
            if (sqr(x) + sqr(y) <= 1) {
                hits++;
            }
        }
    }
    printf("Hits: %lu \t", hits);
    pi->result = (double)  4 * hits  / pi->samples;

    /* fehler berechnen */
    pi->error = (pi->result - M_PI) / M_PI;

    return pi;
}

int main(int argc, const char *argv[]) {
    pi_t *pi;

    /* fehlerhafte eingabe */
    if (argc == 1) {
        printf("Falsche argument anzahl.\n");
        exit(EXIT_FAILURE);
    }

    /* speicher anfordern */
    pi = (pi_t *) malloc(sizeof(pi_t));
    if (pi == NULL) {
        printf("Something went wrong.\n");
        exit(EXIT_FAILURE);
    } else {
        /* eingabe lesen und initialisieren */
        pi->result = 0.0;
        pi->error = 0.0;
        pi->samples = atoi(argv[1]);

        pi = calculate_pi(pi);
        printf("Pi ist %f10 .\n", pi->result);
        printf("Der Fehler beträgt: %f10\n", pi->error);

        /* speicher freigeben */
        free(pi);
        pi = NULL;
    }
    return EXIT_SUCCESS;
}
