#include <stdio.h>
#include <stdlib.h>

int main(){
	int n = 10000000;
	int* a = (int *)calloc(n,sizeof(int));
	int i;
	int sum = 0;
	for (i = 0; i < n; i++){
		sum += a[i];
	}
	printf("using sequential, sum = %d \n", sum);
}
