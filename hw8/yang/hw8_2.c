#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define LB(pid, P, n)  (pid*n/P)
#define UB(pid, P, n)  (((pid+1)*n/P) - 1)
// #define BS(pid, P, n)  (UB(pid, P, n) - LB(pid, P, n) + 1)
#define OWNER(pid, P, n)  (P*(i+1)-1)/n

int main(int argc, char *argv[])
{
	int pid;
	int numP;
	int N = 1000000;
	int i;

    MPI_Init(&argc, &argv);                 /* start MPI           */
    MPI_Comm_size(MPI_COMM_WORLD, &numP);   /* get number of ranks */
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);   /* get rank            */

	int numLoop = 6;
	double averagetime = 0.0;
	int loop;
	double elapsed;
	for (loop = 0; loop < numLoop; loop++){
		MPI_Barrier(MPI_COMM_WORLD);
		elapsed = -MPI_Wtime();
		//printf("pid = %d, numP = %d\n", pid, numP);
		int lb = LB(pid, numP, N);
		int	ub = UB(pid, numP, N);
		int size = ub - lb + 1;
		int * a = (int *) malloc(size * sizeof(int));
		//printf("lb = %d, ub = %d, size = %d \n", lb, ub, size);
		for(i = 0; i < size; i++){
			a[i] = 1;
		}
		int t = 0;
		for (i = 0; i < size; i++){
			t = t + a[i];
		}

		MPI_Status Stat;
		int temp;
		// fan-in tree as MPI recurrence
		// first layer
		if (pid % 2 != 0){
			MPI_Send(&t, 1, MPI_INT, pid-1, 1, MPI_COMM_WORLD);
		}
		if (pid % 2 == 0){
			MPI_Recv(&temp, 1, MPI_INT, pid+1, 1, MPI_COMM_WORLD, &Stat);
			t = t + temp;
		}
		MPI_Barrier(MPI_COMM_WORLD);
		//second layer
		if (pid % 4 == 2){
			MPI_Send(&t, 1, MPI_INT, pid-2, 1, MPI_COMM_WORLD);
		}
		if (pid % 4 == 0){
			MPI_Recv(&temp, 1, MPI_INT, pid+2, 1, MPI_COMM_WORLD, &Stat);
			t = t + temp;
		}
		MPI_Barrier(MPI_COMM_WORLD);
		//third layer	
		if (pid % 8 == 4){
			MPI_Send(&t, 1, MPI_INT, pid-4, 1, MPI_COMM_WORLD);
		}
		if (pid % 8 == 0){
			MPI_Recv(&temp, 1, MPI_INT, pid+4, 1, MPI_COMM_WORLD, &Stat);
			t = t + temp;
		}	
		MPI_Barrier(MPI_COMM_WORLD);	
		//fourth layer	
		if (pid == 8){
			MPI_Send(&t, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
		if (pid == 0){
			MPI_Recv(&temp, 1, MPI_INT, 8, 1, MPI_COMM_WORLD, &Stat);
			t = t + temp;
		}
		MPI_Barrier(MPI_COMM_WORLD);


		if(pid == 0) printf("t = %d \n", t);
		elapsed += MPI_Wtime();
		if (pid == 0){
			if (loop != 0)
				averagetime += elapsed;
			printf("for loop %d, time is %f \n", loop, elapsed);
		}
	}
	if (pid == 0){
		averagetime = averagetime / (numLoop-1);
		printf("averagetime = %f\n", averagetime);
	}

	MPI_Finalize();
	return 0;
}