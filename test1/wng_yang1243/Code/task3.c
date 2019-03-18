#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mkl.h"

void gmm(double *a, double *b, double *c, int m, int n, int k){
	//this function compute c = a*b, op(a) is of size m*n, op(b) is of size n*k, op(c) is of size m*k
	//all matrices are of column wise.
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, k, n, 1.0, a, m, b, n, 0.0, c, m);
}

int main(int argc, char *argv[]){
	int N = 1600;
	int i;

	struct timespec start, stop;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	double * a = (double *) calloc(N * N, sizeof(double));
	double * b = (double *) calloc(N * N, sizeof(double));
	double * c = (double *) calloc(N * N, sizeof(double));

	for (i = 0; i < N*N; i++){
		a[i] = 1.0;
		b[i] = 1.0;
	}

	gmm(a, b, c, N, N, N);

	// for (i = 0; i < N*N; i++){
	// 	printf("c[%d] = %f \n",i, c[i]);
	// }
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

	double elapsed = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3;    // in microseconds
	elapsed = elapsed * 1e-6;

	printf("task3 elapsed time = %f \n", elapsed);

	return 0;
}