/*
 * Monte Carlo method to calculate pi.
 * defined in openmp
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

static int state = 0;
#pragma omp threadprivate(state)

/* omp threads */
void omp_set_num_threads(int threads);
/*
 * random funtkion, nicht threadsafe
 */
int pr_random(void){

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
        /* threadprivate(state) macht rand thradsafe */
        x = random_range(1.0);
        y = random_range(1.0);

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

/* zeitmessung
   timespec diff(timespec *start, timespec *end) {
   timespec temp;
   }
*/

int main(int argc, const char *argv[]) {
    pi_t *pi;
    unsigned long msec;
    struct timespec start, end, tmp;

    /* start zeit | variablen werden ignoriert */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    /* fehlerhafte eingabe */
    if (argc != 3) {
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
        omp_set_num_threads(atoi(argv[2]));

        pi = calculate_pi(pi);
        printf("Pi ist %f10 .\n", pi->result);
        printf("Der Fehler beträgt: %f10\n", pi->error);

        /* speicher freigeben */
        free(pi);
        pi = NULL;
    }
    /* end zeit */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    /* zeitauswertung */
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        tmp.tv_sec = end.tv_sec - start.tv_sec - 1;
        tmp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        tmp.tv_sec = end.tv_sec - start.tv_sec;
        tmp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    /* in msec */
    msec = tmp.tv_sec * 1000;
    msec += tmp.tv_nsec / 1000000;
    /* zeit ausgeben */
    printf("Zeit %ld msec", msec);

    return EXIT_SUCCESS;
}
