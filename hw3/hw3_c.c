#include <stdio.h>
#include <omp.h>

int main(){
	int i;
	double res0  = 0.0;
	for (i = 1; i <= 10000000; i++){
		res0 = res0 + 1/ ((double) i);
	}
	printf("sequentially add 1 to 10,000,000, res0 = %.20f \n", res0);

	double res1 = 0.0;;
	for (i = 10000000; i >= 1; i--){
		res1 = res1 + 1/ ((double) i);
	}
	printf("sequentially add 10,000,000 to 0, res = %.20f \n", res1);
}
