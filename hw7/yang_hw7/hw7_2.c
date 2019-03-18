#include <stdio.h>
#include <math.h>

void block_cyclic_one_thread(int pid, int numP, int B, int N){
	int i;
	int element;
	int b = 0;
	int flag = 1;
	fprintf(stdout, "at thread %d, elements include", pid);
	while (flag){
		for (i = 0; i < B; i++){
			element = b * B * numP + pid * B + i;
			if (element < N){
				fprintf(stdout, " %d,", element);
			}
			else
				flag = 0;
		}
		b++;
	}
	fprintf(stdout, "\n");
}

void block_cyclic(int numP, int B, int N){
	int i;
	for (i = 0; i < numP; i++)
		block_cyclic_one_thread(i, numP, B, N);
}

int main(int argc, char const *argv[])
{
	fprintf(stdout, "For 8 procs, 5 block size, 200 array size \n");
	block_cyclic(8, 5, 200);
	fprintf(stdout, "For 7 procs, 3 block size, 101 array size \n");
	block_cyclic(7, 3, 101);
}