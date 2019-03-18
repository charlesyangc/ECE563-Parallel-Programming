#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	int o1, o2;
	int a = 10;
	int b = 5;
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			o1 = a + b;
			printf("in section 1, result is %d \n", o1);
		}
		#pragma omp section
		{
			o2 = a - b;
			printf("in section 2, result is %d \n", o2);
		}
	}
}
