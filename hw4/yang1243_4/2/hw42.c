#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
	#pragma omp parallel
	{
		for (int i = 0; i < 10; i++){
			printf("i = %d \n", i);
		}
		// change below line to critical, master or single will give different result
		#pragma omp single
		printf("different line \n");
	}
}
