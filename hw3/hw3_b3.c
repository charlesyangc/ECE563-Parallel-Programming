#include <stdio.h>
#include <omp.h>

int main(){
	int i;
	int res = 0;
	int * a = (int *) calloc(10000000, sizeof(int));
#pragma omp parallel for reduction(+:res)
	for (i = 0; i < 10000000; i++){
		res = res + a[i];
	}
	printf("result is %d", res);
}

