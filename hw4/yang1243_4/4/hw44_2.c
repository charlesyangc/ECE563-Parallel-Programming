#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	int i;
	#pragma omp parallel for
	for (i = 0; i < 10; i++)
		printf("i = %d \n", i);
}
