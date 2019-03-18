#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

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


	int blocksize = N / numP; //block size to be blocksize * blocksize

	double elapsed = -MPI_Wtime();
	double * a = (double *) calloc(blocksize * N, sizeof(double)); //a of pid stores N/numP rows of A
	double * b = (double *) calloc(blocksize * N, sizeof(double)); //b of pid stores N/numP cols of B
	double ** c = (double **) malloc(numP * sizeof(double *)); //c stores the resulting matrix
	for (i = 0; i < numP; i++){
		c[i] = (double *) calloc(blocksize * blocksize, sizeof(double)); 
	}

	// 1, initialize block of A and B.
	// at the start, processor pid (pid_rind, pid_cind) generate (rind, (cind-rind+sqrtP)%sqrtP ) block of A
	// and ((rind - cind + sqrtP) % sqrtP, cind) block of B matrix
	for (i = 0; i < blocksize * N; i++){
		a[i] = 1.0;
	}
	for (i = 0; i < blocksize * N; i++){
		b[i] = 1.0;
	}
	gmm(a, b, c[pid], blocksize, N, blocksize); 

	// 2, send  B to left.
	MPI_Status status;

	for (i = 0; i < numP - 1; i++){ //only need to send (sqrP - 1) times
		int dest_B = pid - 1;
		if (dest_B < 0)
			dest_B = numP - 1;
		int source_B = pid + 1;
		if (source_B > numP - 1)
			source_B = 0;

		int sendtag_B = 1;
		int recvtag_B = 1;
		// MPI_Status status;
		MPI_Sendrecv_replace(b, N * blocksize, MPI_DOUBLE, dest_B, sendtag_B, source_B, recvtag_B, MPI_COMM_WORLD, &status);

		int block_for_result = pid + i + 1;
		if (block_for_result > numP - 1)
			block_for_result = 0;
		gmm(a, b, c[block_for_result], blocksize, N, blocksize);

		MPI_Barrier(MPI_COMM_WORLD);
	}

	elapsed += MPI_Wtime();

	// 3, print out the results:
	// int j;
	// printf("pid = %d, blocksize = %d \n", pid, blocksize);
	// for (i = 0; i < numP; i++){
	// 	for (j = 0; j < blocksize * blocksize; j++){
	// 		printf("pid = %d, c[%d][%d] = %f, ", pid, i, j, c[i][j]);
	// 	}
	// }
	MPI_Barrier(MPI_COMM_WORLD);
	if (pid == 0){
		printf("task1, numP = %d, elapsed time = %f \n", numP, elapsed);
	}

	MPI_Finalize();

	return 0;
}