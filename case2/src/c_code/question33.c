#include "common_case.h"
#include <stdio.h>
#include <math.h>

/* extent layout:
 * {xmin,xmax,ymin,ymax}*/

#define ABS_PATH "/Volumes/MacOS/Users/drxl/Projects/smml/statml/case2/Data/"
#define NUM_SAMP 1000
#define MAX_D 100

static inline void fill_n_dim_vect(vect* trgt, gsl_rng* r){
  for (int i = 0; i< trgt->size; i++){
    gsl_vector_set(trgt, i, gsl_ran_gaussian(r, 0));
  }
}

static inline mtrx* gen_n_dim(int N, int samples, double sigma, double mean){
  mtrx* out = gsl_matrix_alloc(samples, N);
  
  gsl_rng* r = gsl_rng_alloc(gsl_rng_taus);
  for(int i = 0; i < samples; i++){
    gsl_vector_view wrk = gsl_matrix_row(out, i);
    fill_n_dim_vect(&wrk.vector, r);
  }
  return out;
}


int main(int argv, char** argc){
	printf("\n\nRunning code for question 3.3:\n\n");
  double * l2norm = (double*) malloc(sizeof(double)*MAX_D*3);
    
  mtrx* big_m = gen_n_dim(MAX_D, NUM_SAMP, 0, 0);
  
  for (int i = 0; i < MAX_D; i++){
    
  }
}
