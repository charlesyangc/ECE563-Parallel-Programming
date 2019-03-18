#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Q {
   int* q;
   int pos;
   int size;
} Q;

struct Q* initQ(int n) {
   int i;
   struct Q *newQ = (struct Q *) malloc(sizeof(Q));   
   newQ->q = (int*) malloc(sizeof(int)*n);
   newQ->pos = -1;
   newQ->size = n-1;
   return newQ;
}

void putWork(struct Q* workQ) {
   if (workQ->pos < (workQ->size)) {
      workQ->pos++;
      workQ->q[workQ->pos] = (int) (rand( )%2*(workQ->pos/1000));
   } else printf("ERROR: attempt to add Q element%d\n", workQ->pos+1);
}

int getWork(struct Q* workQ) {
   if (workQ->pos > -1) {
      int w = workQ->q[workQ->pos];
      workQ->pos--;
      return w;
   } else printf("ERROR: attempt to get work from empty Q%d\n", workQ->pos);
}

int main(){
	int i;
	int n = 10000;
	Q* workQ = initQ(n);
	printf("pos = %d, size = %d \n", workQ->pos, workQ->size);
	double elapsed = omp_get_wtime();
	int w;
	#pragma omp parallel private(w)
	{
		while (workQ->pos < workQ->size){
			putWork(workQ);
		}
	}
	printf("pos = %d\n", workQ->pos); 
	for (i = 0; i < n; i += 500){
		workQ->pos = i;
		printf("Q[%d] = %d \n", i, getWork(workQ));
	}
	elapsed = omp_get_wtime() - elapsed;
	printf("elapsed time is %f \n", elapsed);
}
