#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>


struct node {
  int level;
  double val;
  struct node* l;
  struct node* r;
} nodeT;

#define PARPOINT 4
#define MAXLEVEL 18

struct node* build(int level) {
   if (level < MAXLEVEL) {
      struct node* p = (struct node*) malloc(sizeof(nodeT));
      p->level = level;
      p->val = (double) rand() / RAND_MAX;
      p->l = build(level+1);
      p->r = build(level+1);
      return p;
   } else {
      return NULL;
  }
}

int traverse(struct node* p) {
  int sum = 0;
  if (p == NULL) return 0;
  if (p->l != NULL) 
    sum += traverse(p->l);
  if (p->r != NULL) 
    sum += traverse(p->r);
  //printf("val = %f, level = %d \n", p->val, p->level);
  if (p->val < 0.5)
    sum++;
  return sum;
}

struct node* build_par(int level) {
   if (level < MAXLEVEL) {
      struct node* p = (struct node*) malloc(sizeof(nodeT));
      p->level = level;
      p->val = (double) rand() / RAND_MAX;
      int par_flag = ( (pow(2,level+1)-1) < omp_get_num_threads() );
      if (par_flag){
        #pragma omp task
        p->l = build_par(level+1);
        #pragma omp task
        p->r = build_par(level+1);
      }
      else{
        p->l = build(level+1);
        p->r = build(level+1);
      }
      return p;
   } else {
      return NULL;
  }
}

int traverse_par(struct node* p) {
  int sum = 0, x, y;
  if (p == NULL) return 0;
  int par_flag = ( (pow(2,p->level+1)-1) < omp_get_num_threads() );
  if (par_flag){
    #pragma omp task shared(x)
    x = traverse_par(p->l);
    #pragma omp task shared(y)
    y = traverse_par(p->r);
  }
  else {
    if (p->l != NULL){
      x = traverse(p->l);
    }
    if (p->r != NULL){
      y = traverse(p->r);
    }
  }
  //printf("val = %f, level = %d \n", p->val, p->level);
  #pragma omp taskwait
  sum = x + y;
  if (p->val < 0.5)
    sum++;
  return sum;
}


int main( ) {
  double elapsed = omp_get_wtime();
  int sumpar = 0;
  struct node * h_par;
  #pragma omp parallel
  {
    #pragma omp single nowait
    h_par = build_par(0);
    // #pragma omp taskwait
  }
  #pragma omp parallel
  {
    #pragma omp single nowait
    sumpar = traverse_par(h_par);
    // #pragma omp taskwait
  }
  printf("sum par = %d \n", sumpar); 
  elapsed = omp_get_wtime() - elapsed;
  printf("parallel elapsed time is %f \n", elapsed);

  elapsed = omp_get_wtime();
  struct node* h = build(0);
  int sumseq = 0;
  sumseq = traverse(h);
  printf("sum seq = %d\n", sumseq);
  elapsed = omp_get_wtime() - elapsed;
  printf("sequential elapsed time is %f \n", elapsed);

} 

