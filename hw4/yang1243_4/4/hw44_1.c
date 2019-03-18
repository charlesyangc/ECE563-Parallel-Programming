#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
	int i;
	#pragma omp parallel
	{
		for (i = 0; i < 10; i++){
			printf("i = %d \n", i);
		}
	}
}
