#include <stdio.h>
#include <omp.h>

int main(){
	int i;
	double res;
#pragma omp parallel for reduction(+:res)
	for (i = 1; i <= 10000000; i++){
		res = res + 1 / ((double) i);
	}
	printf("OpenMP, parallel, 1 to 10,000,000, res is %.20f \n", res);

	res = 0.0;
#pragma omp parallel for reduction(+:res)
	for (i = 10000000; i >= 1; i--){
		res = res + 1 / ((double) i);
	}
	printf("OpenMP, parallel, 10,000,000 to 1, res is %.20f \n", res);
}

