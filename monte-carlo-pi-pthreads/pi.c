/*Monaco */
#define RNG_MOD 714025
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#define M_PI 3.14159265358979323846

typedef struct 
{
	unsigned long iter, value;

}data;

int pr_random(void){
    static int state = 0;

    return (state = (1366 * state + 150889) % RNG_MOD);
}

double pr_random_f(double range){
	return ((double) pr_random() / (double) RNG_MOD) * range;
}

static void *piGenerator(void *arg){
	data *f=(data *)arg;
	unsigned long i ;

	for ( i = 0; i < f->iter; ++i){
		
		double x = pr_random_f(1.000);
		double y = pr_random_f(1.000);
		
		
		if (x*x + y*y <= 1.0){
			f->value++;
		}
	}
	return arg;
}

int main(int argc, const char *argv[]){	
	unsigned long thcount = atoi(argv[1]); 
	unsigned long stepcount = atoi(argv[2]);
	unsigned long i;
	pthread_t th[thcount];
	data *ret[thcount];
	double erg = 0.0;
	if (argc != 3){
		printf("%s\n", "Falsche Parameter anzahl");
	}
	else {

   		for  ( i = 0; i < thcount; i++) {

      		ret[i] = (data*)malloc(sizeof(data));
      		if(ret[i] == NULL) {
         		printf("Konnte keinen Speicher reservieren ...!\n");
         		exit(EXIT_FAILURE);
      		}
      		

      		ret[i]->iter = stepcount / thcount;
      		if (i == 0){
      			ret[i]->iter = ret[i]->iter + (stepcount % thcount);
      		}

      		if(pthread_create(&th[i], NULL, &piGenerator, ret[i]) != 0) {
         		fprintf (stderr, "Konnte Thread nicht erzeugen\n");
         		exit (EXIT_FAILURE);
      		}
   		}
   		
		
		for( i=0; i < thcount; i++){
      		pthread_join(th[i], (void**) &ret[i]);
   			erg = erg + ret[i]->value;
   		}
   		
		erg = 4*(erg/stepcount);
   		printf("Ergebnis: %f\n", erg);
   		printf("relativer Fehler : %f\n", (erg - M_PI) / M_PI);
	}
	return 0;
}
