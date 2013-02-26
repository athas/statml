#include "common_case.h"
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>

typedef struct timeval timeval;

static inline void fill_n_dim_vect(vect* trgt, gsl_rng* r){
  for (int i = 0; i<trgt->size; i++){
    double tmp = gsl_ran_gaussian(r, 1.0);
    gsl_vector_set(trgt, i, tmp);
  }
}

static inline mtrx* gen_n_dim(int N, int samples){
  mtrx* out = gsl_matrix_alloc(samples, N);
  
  gsl_rng* r = gsl_rng_alloc(gsl_rng_taus);
  for(int i = 0; i < samples; i++){
    gsl_vector_view wrk = gsl_matrix_row(out, i);
    fill_n_dim_vect(&wrk.vector, r);
  }
  gsl_rng_free(r);
  return out;
}


int main(int argv, char** argc){
  #define mtrx_dim 10000
  mtrx* big_m = gen_n_dim(mtrx_dim, mtrx_dim);
  mtrx* output = gsl_matrix_alloc(mtrx_dim,mtrx_dim);


	/*start timer*/
	timeval t;
	gettimeofday(&t, NULL);
	int64_t time_start = t.tv_sec*1000000 + t.tv_usec;
	
	gsl_blas_dgemm(CblasNoTrans,CblasTrans,
				   1, big_m,big_m,
				   0, output);
	gettimeofday(&t, NULL);
	int64_t time_end = t.tv_sec*1000000 + t.tv_usec;

	printf("dgemm time:\t%u\n",
			(unsigned int)(time_end - time_start));
  
  gsl_matrix_free(big_m);
  gsl_matrix_free(output);


}
