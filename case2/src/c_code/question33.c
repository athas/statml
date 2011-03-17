#include "common_case.h"
#include <stdio.h>
#include <math.h>

/* extent layout:
 * {xmin,xmax,ymin,ymax}*/

#define NUM_SAMP 50
#define MAX_D 5000

void gplot_one2infty(double* data, int infty, char* fname){
	FILE* fp;
	fp = fopen(fname, "w");

  fprintf(fp, "#ndim:meanlen:meandist\n");
  
	for (int i=0; i<infty;i++){
    fprintf(fp, "%d\t\t%f\t\t%f\n", 
              i+1, *(data+i), *(data+infty+i));
	}
	fclose(fp);
}

static inline void fill_n_dim_vect(vect* trgt, gsl_rng* r){
  for (int i = 0; i<trgt->size; i++){
    double tmp = gsl_ran_gaussian(r, 1.0);
    gsl_vector_set(trgt, i, tmp);
  }
}

static inline mtrx* gen_n_dim(int N, int samples, double sigma, double mean){
  mtrx* out = gsl_matrix_alloc(samples, N);
  
  gsl_rng* r = gsl_rng_alloc(gsl_rng_taus);
  for(int i = 0; i < samples; i++){
    gsl_vector_view wrk = gsl_matrix_row(out, i);
    fill_n_dim_vect(&wrk.vector, r);
  }
  gsl_rng_free(r);
  return out;
}


static inline double l2norm(const mtrx* in, int n){
  double out = 0.0;
  for(int i = 0; i< in->size1; i++){
    gsl_vector_view v1 = gsl_vector_view_array(in->data+i*in->tda,n);
      out += gsl_blas_dnrm2(&v1.vector);
  }
  return out/(in->size1);
}

static inline double mean_distance(const mtrx* in, int n){
  double out = 0.0;
  vect* wrk = gsl_vector_alloc(n);
  for(int i = 0; i< in->size1; i++){
    gsl_vector_view v1 = gsl_vector_view_array(in->data+i*in->tda,n);
    for(int j = i; j < in->size1; j++){
      gsl_vector_view v2 = gsl_vector_view_array(in->data+j*in->tda,n);
      gsl_vector_memcpy(wrk, &v2.vector);
      gsl_vector_sub(wrk, &v1.vector);
      out += gsl_blas_dnrm2(wrk);
    }
  }
  gsl_vector_free(wrk);
  return out/(in->size1*in->size1-1);
}


int main(int argv, char** argc){
	printf("\n\nRunning code for question 3.3:\n\n");
  double* mean_l2 = (double*) malloc(sizeof(double)*MAX_D*2);
  double* mean_dist = mean_l2 + MAX_D;
    
  mtrx* big_m = gen_n_dim(MAX_D, NUM_SAMP, 0, 0);
  
  for (int i = 1; i <= MAX_D; i++){
    *(mean_l2+i-1) = i+1;
    *(mean_l2+i-1) = l2norm(big_m, i);
    *(mean_dist+i-1) = mean_distance(big_m, i);
  }
  
  gplot_one2infty(mean_l2, MAX_D, FIG_OUT"q33.dat");
  free(mean_l2);
  gsl_matrix_free(big_m);
  
/*  figure_ctrl plot;
  init_figure(&plot,"Many Dimmensional Vectors","pdf color");
	
  plot_x_y(&plot,n_val,mean_l2,MAX_D,"Mean vector length", plot_style2str(ps_points));
	plot_x_y(&plot,n_val,mean_dist,MAX_D,"Mean vector distance",plot_style2str(ps_points));
	
//	plot_viewbox(&plot,-1,3,-1,4);
	
	figure2file(&plot,"~/question33.pdf");*/

}
