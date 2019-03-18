#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void doWork(int t) {
   sleep(t);
}

int* initWork(int n) {
   int i;
   double r;
   int* wA = (int *) malloc(sizeof(int)*n);   
   for (i = 0; i < n; i++) {
      wA[i] = (int) rand( )%2*i/(n/10);
   }
   return wA;
}

int main (int argc, char *argv[]) {
	int i;
	int *w = initWork(100);
	double elapsed = omp_get_wtime();
	#pragma omp parallel sections private(i)
	{
		#pragma omp section
		{
			for (i = 0; i < 20; i++){
				doWork(w[i]);
			}
		}
		#pragma omp section
		{
			for (i = 20; i < 40; i++){
				doWork(w[i]);
			}
		}
		#pragma omp section
		{
			for (i = 40; i < 60; i++){
				doWork(w[i]);
			}
		}
		#pragma omp section
		{
			for (i = 60; i < 80; i++){
				doWork(w[i]);
			}
		}
		#pragma omp section
		{
			for (i = 80; i < 100; i++){
				doWork(w[i]);
			}
		}
	}
	elapsed = omp_get_wtime() - elapsed;
	printf("parallel elapsed time is %f \n", elapsed);
	elapsed = omp_get_wtime();
	for (i = 0; i < 100; i++){
		doWork(w[i]);
	}
	elapsed = omp_get_wtime() - elapsed;
	printf("sequential elapsed time is %f \n", elapsed);
}

