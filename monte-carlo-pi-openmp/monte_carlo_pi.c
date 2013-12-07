/*
 * Monte Carlo method to calculate pi.
 * defined in openmp
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RNG_MOD 714025
#define sqr(a)  a*a


typedef struct {
    double result;
    unsigned long samples;
} pi_t;


int pr_random(void){
    static int state = 0;

    return (state = (1366 * state + 150889) % RNG_MOD);
}

/*
 * returns random value between range and range+1
 */
float random_range(double range) {
    return ((double) pr_random() / (double) RNG_MOD) * range;
}

pi_t *calculate_pi(pi_t *pi) {
    unsigned long hits = 0;
    double x, y;
    int i;

    // berechnung
    for (i = pi->samples; i; i--) {
        x = random_range(1.000);
        y = random_range(1.000);
        if (sqr(x) + sqr(y) < 1) {
            hits++;
        }
    }

    printf("Hits: %lu \t", hits);
    pi->result = (double)  4 * hits  / pi->samples;
    return pi;
}

int main(int argc, const char *argv[]) {
    // speicher anfordern
    pi_t *pi;
    pi = (pi_t *) malloc(sizeof(pi_t));

    if (argc == 1) {
        printf("Falsche argument anzahl.\n");
        free(pi);
        pi = NULL;
        exit(EXIT_FAILURE);
    }

    if (pi == NULL) {
        printf("Something went wrong.\n");
        exit(EXIT_FAILURE);
    } else {
        // eingabe lesen
        pi->samples = atoi(argv[1]);

        pi = calculate_pi(pi);
        printf("Pi ist %f .\n", pi->result);
        // speicher freigeben
        free(pi);
        pi = NULL;
    }
    return 0;
}
