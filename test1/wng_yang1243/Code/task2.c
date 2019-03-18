#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define PID_RIND(pid, sqrtP)  (pid%sqrtP)
#define PID_CIND(pid, sqrtP)  (pid/sqrtP)

void gmm(double * a, double * b, double * c, int m, int n, int k){
	//this function perform c = c + a * b, a is of size m * n, b is of size n * k, c is of size m * k;
	// matrix is column wise.
	int i, j, l;
	for (i = 0; i < m; i++){
		for (j = 0; j < k; j++){
			for (l = 0; l < n; l++){
				c[i + j * m] = c[i + j * m] + a[i + l * m] * b[l + j * n];
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int pid;
	int numP;
	int N = 1600; // matrix size to be 32 * 32
	int i;

    MPI_Init(&argc, &argv);                 /* start MPI           */
    MPI_Comm_size(MPI_COMM_WORLD, &numP);   /* get number of ranks */
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);   /* get rank            */


	int sqrtP = (int) sqrt(numP);
	int blocksize = N / sqrtP; //block size to be blocksize * blocksize

	double elapsed = -MPI_Wtime();
	double * a = (double *) calloc(blocksize * blocksize, sizeof(double));
	double * b = (double *) calloc(blocksize * blocksize, sizeof(double));
	double * c = (double *) calloc(blocksize * blocksize, sizeof(double)); //c stores the resulting matrix

	// 1, initialize block of A and B.
	// at the start, processor pid (pid_rind, pid_cind) generate (rind, (cind-rind+sqrtP)%sqrtP ) block of A
	// and ((rind - cind + sqrtP) % sqrtP, cind) block of B matrix
	for (i = 0; i < blocksize * blocksize; i++){
		a[i] = 1.0;
	}
	for (i = 0; i < blocksize * blocksize; i++){
		b[i] = 1.0;
	}
	gmm(a, b, c, blocksize, blocksize, blocksize); 

	// 2, send A to left and B to up.
	int pid_rind = PID_RIND(pid, sqrtP);
	int pid_cind = PID_CIND(pid, sqrtP);

	for (i = 0; i < sqrtP - 1; i++){ //only need to send (sqrP - 1) times
		//move a one left 
		int dest_A_cind = pid_cind - 1;
		if (dest_A_cind < 0)
			dest_A_cind = sqrtP - 1;
		int source_A_cind = pid_cind + 1;
		if (source_A_cind > sqrtP - 1)
			source_A_cind = 0;
		int dest_A = pid_rind + dest_A_cind * sqrtP;
		int source_A = pid_rind + source_A_cind * sqrtP;
		int sendtag_A = 0;
		int recvtag_A = 0;
		MPI_Status status;
		MPI_Sendrecv_replace(a, blocksize * blocksize, MPI_DOUBLE, dest_A, sendtag_A, source_A, recvtag_A, MPI_COMM_WORLD, &status);

		//move b one up
		int dest_B_rind = pid_rind - 1;
		if (dest_B_rind < 0)
			dest_B_rind = sqrtP - 1;
		int source_B_rind = pid_rind + 1;
		if (source_B_rind > sqrtP - 1)
			source_B_rind = 0;
		int dest_B = dest_B_rind + pid_cind * sqrtP;
		int source_B = source_B_rind + pid_cind * sqrtP;
		int sendtag_B = 1;
		int recvtag_B = 1;
		// MPI_Status status;
		MPI_Sendrecv_replace(b, blocksize * blocksize, MPI_DOUBLE, dest_B, sendtag_B, source_B, recvtag_B, MPI_COMM_WORLD, &status);

		gmm(a, b, c, blocksize, blocksize, blocksize);

		MPI_Barrier(MPI_COMM_WORLD);
	}

	elapsed += MPI_Wtime();

	// 3, print out the results:
	// printf("pid = %d, pid_rind = %d, pid_cind = %d, blocksize = %d \n", pid, pid_rind, pid_cind, blocksize);
	// for (i = 0; i < blocksize * blocksize; i++){
	// 	printf("pid = %d, c[%d] = %f, ", pid, i, c[i]);
	// }
	MPI_Barrier(MPI_COMM_WORLD);
	if (pid == 0)
		printf("task2, numP = %d, elapsed time = %f \n", numP, elapsed);

	MPI_Finalize();

	return 0;
}