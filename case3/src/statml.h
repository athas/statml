#ifndef STATML_H_P9E7R7HG
#define STATML_H_P9E7R7HG

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	double* x;
	double* y;
} xy_range_t;

double* range_x(double, double, int);

xy_range_t range_xy(double, double, int, double (*)(double));

double matrix_determinant(gsl_matrix* input);
	
//compute most likely mean	
gsl_vector* sample_mean(gsl_matrix* samples);
	
gsl_matrix* sample_cov(gsl_matrix* samples, gsl_vector* meanML);

gsl_matrix* inverse_matrix(gsl_matrix* input);

void print_mtrx(gsl_matrix* src);
	
void print_vec(gsl_vector* src);
	
void mtrx2tex(gsl_matrix*,char* cmd_name, FILE* fp);

void vect2tex_trans(gsl_vector* src,char* cmd_name, FILE* fp);

void vect2tex(gsl_vector* src,char* cmd_name, FILE* fp);

void norm_vwise(gsl_matrix* trgt);

double RMS(double* train, double* actual, int length);

  
#ifdef __cplusplus
}
#endif

#endif /* end of include guard: STATML_H_P9E7R7HG */
