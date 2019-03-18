#include <stdio.h>
#include <omp.h>

int main(){
	int i;
	int nt = omp_get_num_procs();
	int* res = (int *) calloc(nt, sizeof(int));
	int * a = (int *) calloc(10000000, sizeof(int));
#pragma omp parallel for
	for (i = 0; i < 10000000; i++){
		res[omp_get_thread_num()] += a[i];
	}
	int res_fin = 0;
	for (i = 0; i < nt; i++){
		res_fin += res[i];
	}
	printf("result is %d \n", res_fin);
}
