#include <stdio.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
	int i;
	printf("number of processers is %d \n", omp_get_num_procs( ));

#pragma omp parallel for
	for (i = 0; i < omp_get_num_procs( ); i++){
		printf("thread id is %d \n", omp_get_thread_num());
	}

#pragma omp single
	printf("single thread id is %d \n", omp_get_thread_num());

#pragma omp master
	printf("master thread id is %d \n", omp_get_thread_num());

	return 0;
}
